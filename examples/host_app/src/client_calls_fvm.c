#include <flipper.h>
#include <libflipper.h>
#include <network.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int running = 1;

void handle_sigint(int);

void handle_sigint(int sig) {
    running = 0;
}

int main(int argc, char *argv[]) {
    const char *module_name = NULL;

    int port = -1;
    int port_from_file = 0;

    FILE *pf = fopen("/tmp/fvm.port", "r");
    if (pf) {
        fscanf(pf, "%d", &port);
        fclose(pf);
        port_from_file = 1;
    }

    if (argc > 1) {
        module_name = argv[1];
        printf("[client] Using module name: %s\n", module_name);
    } else {
        fprintf(stderr, "[client] ERROR: Missing required module name argument.\n");
        fprintf(stderr, "Usage: %s [port] <module_name>\n", argv[0]);
        return 1;
    }
        
    if (argc > 2) {
        port = atoi(argv[2]);
        printf("[client] Using provided port: %d\n", port);
    } else if (port_from_file) {
        printf("[client] Auto-loaded port: %d\n", port);
    } else {
        port = LF_UDP_PORT;
        printf("[client] Using default port: %d\n", port);
    }

    struct _lf_device *fvm = lf_device_create_named("fvm", lf_network_read, lf_network_write, lf_network_release);
    if (!fvm) {
        printf("Failed to create FVM device.\n");
        return 1;
    }

    fvm->_ep_ctx = calloc(1, sizeof(struct _lf_network_context));
    if (!fvm->_ep_ctx) {
        printf("Failed to allocate network context.\n");
        lf_device_release(fvm);
        return 1;
    }

    struct _lf_network_context *ctx = (struct _lf_network_context *)fvm->_ep_ctx;

    if (lf_network_connect(fvm, "127.0.0.1", port) != lf_success) {
        printf("Failed to connect to FVM at localhost:%d\n", port);
        free(fvm->_ep_ctx);
        lf_device_release(fvm);
        return 1;
    }

    fprintf(stderr, "[client] Connected to FVM with fd=%d on device '%s'.\n", ctx->fd, fvm->name);
    fprintf(stderr, "[client] Writing to %s:%d\n", inet_ntoa(ctx->addr.sin_addr), ntohs(ctx->addr.sin_port));

    if (lf_select(fvm) < 0) {
        printf("Failed to select FVM device.\n");
        free(fvm->_ep_ctx);
        lf_device_release(fvm);
        return 1;
    }

    if (strlen(fvm->name) == 0) {
        fprintf(stderr, "[client] device->name was empty! Re-setting to 'fvm'.\n");
        strncpy(fvm->name, "fvm", sizeof(fvm->name) - 1);
        fvm->name[sizeof(fvm->name) - 1] = '\0';
    }

    fprintf(stderr, "[client] Looking up module '%s'...\n", module_name);

    uint16_t idx;
    if (lf_dyld_dyn_sym_lookup(fvm, module_name, &idx) != lf_success) {
        fprintf(stderr, "[client] DYLD lookup failed for module '%s'.\n", module_name);
        free(fvm->_ep_ctx);
        lf_device_release(fvm);
        return 1;
    }

    fprintf(stderr, "[client] Module index for '%s': %d\n", module_name, idx);
    fprintf(stderr, "[client] About to invoke '%s' to %s:%d\n",
            module_name, inet_ntoa(ctx->addr.sin_addr), ntohs(ctx->addr.sin_port));

    lf_return_t ret;
    fprintf(stderr, "[client] Sending RPC packet to invoke '%s' function 0\n", module_name);
    int result = lf_invoke_by_index(fvm, module_name, 0, lf_void_t, &ret, NULL);

    if (result == lf_success) {
        fprintf(stderr, "[client] Successfully sent RPC packet, awaiting response\n");
        char buffer[BUFFER_SIZE];
        struct sockaddr_in server_addr;
        socklen_t addr_len = sizeof(server_addr);
        
        // Non-blocking read for additional response
        ssize_t bytes_received = recvfrom(ctx->fd, buffer, BUFFER_SIZE - 1, MSG_DONTWAIT,
                                          (struct sockaddr *)&server_addr, &addr_len);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            int printable = 1;
            for (ssize_t i = 0; i < bytes_received; i++) {
                if (buffer[i] < 32 || buffer[i] > 126) {
                    printable = 0;
                    break;
                }
            }

            if (printable) {
                printf("[client] Raw message from server: \"%s\"\n", buffer);
            } else {
                printf("[client] Received binary response (%zd bytes).\n", bytes_received);
            }
        } else if (bytes_received < 0 && errno != EWOULDBLOCK) {
            fprintf(stderr, "[client] Failed to receive response from server: %s\n", strerror(errno));
        }

        printf("[client] %s() returned: %llu\n", module_name, ret);
    }

    if (result != lf_success) {
        fprintf(stderr, "[client] Failed to invoke %s: error %d\n", module_name, result);
        free(fvm->_ep_ctx);
        lf_device_release(fvm);
        return 1;
    }

    printf("%s() invoked successfully.\n", module_name);

    free(fvm->_ep_ctx);
    lf_device_release(fvm);
    return 0;
}
