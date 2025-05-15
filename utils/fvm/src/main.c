#include <arpa/inet.h>
#include <flipper.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define _GNU_SOURCE
#include <flipper/platforms/posix/network.h>
#include <dlfcn.h>
#include <flipper/ll.h>

/* fvm - Creates a local server that acts as a virtual flipper device. */

typedef void (*fmr_plugin_register_t)(struct _lf_device *fvm);

int main(int argc, char *argv[]) {
    int e;
    int sd;
    struct sockaddr_in addr;
    struct _lf_device *fvm;

    if (argc > 1 && !strcmp(argv[1], "--version")) {
        printf("FVM version: %s\n\n", lf_get_git_hash());
        return EXIT_SUCCESS;
    }

    lf_set_debug_level(LF_DEBUG_LEVEL_ALL);

    /* Create UDP socket */
    sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    lf_assert(sd, E_UNIMPLEMENTED, "failed to open socket");

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(0);  // Let OS choose a port
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    e = bind(sd, (struct sockaddr *)&addr, sizeof(addr));
    lf_assert(e == 0, E_UNIMPLEMENTED, "failed to bind socket");

    // Retrieve actual port
    socklen_t len = sizeof(addr);
    getsockname(sd, (struct sockaddr *)&addr, &len);
    int actual_port = ntohs(addr.sin_port);

    // Print and persist the chosen port
    printf("Flipper Virtual Machine (FVM) v0.1.0\nFVM listening on port %d\n", actual_port);
    
    FILE *portfile = fopen("/tmp/fvm.port", "w");
    if (portfile) {
        fprintf(portfile, "%d\n", actual_port);
        fclose(portfile);
    }

    fvm = lf_device_create_named("fvm", lf_network_read, lf_network_write, lf_network_release);
    lf_assert(fvm, E_ENDPOINT, "failed to create device for virtual machine.");

    fvm->modules = lf_ll_create();

    fmr_init();

    fvm->_ep_ctx = calloc(1, sizeof(struct _lf_network_context));
    struct _lf_network_context *context = (struct _lf_network_context *)fvm->_ep_ctx;
    lf_assert(context, E_NULL, "failed to allocate memory for context");
    context->fd = sd;


    if (argc > 1) {
        char *lib = argv[1];
        char *module, **modules = &argv[2];

        void *dlm_global = dlopen(lib, RTLD_LAZY);
        if (!dlm_global) {
            fprintf(stderr, "Warning: failed to load '%s' for plugin registration.\n", lib);
        } else {
            fmr_plugin_register_t register_plugin = dlsym(dlm_global, "fmr_plugin_register");
            if (register_plugin) {
                fprintf(stderr, "[main] Found fmr_plugin_register, registering plugin dispatchers.\n");
                register_plugin(fvm);
            } else {
                fprintf(stderr, "[main] No fmr_plugin_register symbol found in '%s'.\n", lib);
            }
        }

        while ((module = *modules++)) {
            printf("Loading module '%s' from '%s'.", module, lib);
            void *dlm = dlopen(lib, RTLD_LAZY);
            lf_assert(dlm, E_NULL, "failed to open module '%s'.", lib);

            char module_sym[32];
            snprintf(module_sym, sizeof(module_sym), "_%s_module", module);

            struct _lf_module *m = dlsym(dlm, module_sym);
            lf_assert(m, E_NULL, "failed to read symbol '%s' from '%s'.", module_sym, lib);
            printf("Successfully loaded symbol '%s'.", module_sym);

            if (!m->name || strlen(m->name) == 0) {
                strncpy((char *)m->name, module, sizeof(m->name) - 1);
                ((char *)m->name)[sizeof(m->name) - 1] = '\0';
            }

            fprintf(stderr, "[main] Registered module name = '%s'\n", m->name);
            fprintf(stderr, "[main] m->interface[0] = %p\n", m->interface ? m->interface[0] : NULL);
            fprintf(stderr, "[main] m->table        = %p\n", (void *)m->table);
            fprintf(stderr, "[main] m->length       = %zu\n", m->length);

            e = dyld_register(fvm, m);
            lf_assert(e == lf_success, E_NULL, "failed to register module '%s'.", m->name);
            printf("Successfully registered module '%s'.", module);
        }
        printf("\n");
    }

    print_module_list(fvm->modules);

    while (1) {
        struct _fmr_packet packet;
        socklen_t len = sizeof(context->addr);

        printf("[fvm] Awaiting packet...\n");

        ssize_t n = recvfrom(context->fd, &packet, sizeof(packet), 0,
                             (struct sockaddr *)&context->addr, &len);
        if (n < 0) {
            perror("[fvm] recvfrom failed");
            continue;
        }

        fprintf(stderr, "[fvm] Received %zd bytes from %s:%d\n",
                n, inet_ntoa(context->addr.sin_addr), ntohs(context->addr.sin_port));

        printf("[fvm] Received packet header: magic=0x%02x len=%d\n", packet.hdr.magic, packet.hdr.len);
        lf_debug_packet(&packet);
        fmr_perform(fvm, &packet);
    }

    close(sd);

fail:
    return EXIT_FAILURE;
}
