#include "libflipper.h"
#include "network.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>

struct _lf_device *lf_network_device_for_hostname(char *hostname) {
    struct _lf_network_context *context = NULL;
    struct _lf_device *device = lf_device_create(lf_network_read, lf_network_write, lf_network_release);
    lf_assert(device, E_ENDPOINT, "Failed to create device");

    device->_ep_ctx = calloc(1, sizeof(struct _lf_network_context));
    context = (struct _lf_network_context *)device->_ep_ctx;
    lf_assert(context, E_NULL, "Failed to allocate memory for context");

    strncpy(context->host, hostname, sizeof(context->host));
    context->host[sizeof(context->host) - 1] = '\0';

    return device;

fail:
    return NULL;
}

int lf_network_connect(struct _lf_device *device, const char *host, uint16_t port) {
    struct _lf_network_context *ctx = (struct _lf_network_context *)device->_ep_ctx;
    lf_assert(ctx, E_NULL, "No endpoint context.");

    ctx->fd = socket(AF_INET, SOCK_DGRAM, 0);
    lf_assert(ctx->fd > 0, E_SOCKET, "Failed to create UDP socket.");

    // BIND the socket to an ephemeral local port for receiving replies
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    client_addr.sin_port = htons(0);  // let OS choose

    int b = bind(ctx->fd, (struct sockaddr *)&client_addr, sizeof(client_addr));
    lf_assert(b == 0, E_SOCKET, "Failed to bind client socket.");

    memset(&ctx->addr, 0, sizeof(ctx->addr));
    ctx->addr.sin_family = AF_INET;
    ctx->addr.sin_port = htons(port);

    // Handle "localhost" if passed
    const char *resolved_host = (strcmp(host, "localhost") == 0) ? "127.0.0.1" : host;

    in_addr_t ip = inet_addr(resolved_host);
    lf_assert(ip != INADDR_NONE, E_COMMUNICATION, "Invalid IP address string: %s", resolved_host);
    ctx->addr.sin_addr.s_addr = ip;

    strncpy(ctx->host, resolved_host, sizeof(ctx->host));
    ctx->host[sizeof(ctx->host) - 1] = '\0';

    fprintf(stderr, "[lf_network_connect] Stored %s (%u)\n", resolved_host, ip);
    fprintf(stderr, "[lf_network_connect] ctx->addr.sin_addr = %s\n", inet_ntoa(ctx->addr.sin_addr));
    fprintf(stderr, "[lf_network_connect] Target = %s:%d\n",
            inet_ntoa(ctx->addr.sin_addr), ntohs(ctx->addr.sin_port));

    return lf_success;

fail:
    if (ctx->fd > 0) close(ctx->fd);
    return lf_error;
}

int lf_network_read(struct _lf_device *device, void *dst, uint32_t length) {
    lf_assert(device, E_NULL, "invalid device");

    struct _lf_network_context *ctx = (struct _lf_network_context *)device->_ep_ctx;
    lf_assert(ctx, E_NULL, "invalid context");

    fprintf(stderr, "[lf_network_read] Called\n");

    struct sockaddr_in src_addr;
    socklen_t addr_len = sizeof(src_addr);

    fprintf(stderr, "[debug] Trying to read from fd=%d\n", ctx->fd);
    
    // Retry recvfrom to handle transient errors
    int retries = 5;
    int delay_ms = 100;
    ssize_t e;
    do {
        e = recvfrom(ctx->fd, dst, length, 0, (struct sockaddr *)&src_addr, &addr_len);
        if (e >= 0) {
            break;
        }
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
            fprintf(stderr, "[lf_network_read] recvfrom retrying (errno=%d)\n", errno);
            usleep(delay_ms * 1000);
        } else {
            fprintf(stderr, "[lf_network_read] recvfrom failed: %s\n", strerror(errno));
            goto fail;
        }
    } while (--retries > 0);

    if (e < 0) {
        fprintf(stderr, "[lf_network_read] recvfrom failed after %d retries: %s\n", 5, strerror(errno));
        goto fail;
    }

    // Log raw response bytes
    fprintf(stderr, "[lf_network_read] Raw response (%zd bytes): ", e);
    for (ssize_t i = 0; i < e; i++) {
        fprintf(stderr, "0x%02x ", ((uint8_t *)dst)[i]);
    }
    fprintf(stderr, "\n");

    // Always update ctx->addr to the responding sender
    fprintf(stderr, "[lf_network_read] Updating ctx->addr from received packet.\n");
    memcpy(&ctx->addr, &src_addr, sizeof(struct sockaddr_in));
    fprintf(stderr, "[lf_network_read] ctx->addr updated to %s:%d\n",
        inet_ntoa(ctx->addr.sin_addr), ntohs(ctx->addr.sin_port));

    fprintf(stderr, "[lf_network_read] Received %zd bytes from %s:%d\n",
            e, inet_ntoa(src_addr.sin_addr), ntohs(src_addr.sin_port));

    return lf_success;

fail:
    return lf_error;
}

int lf_network_write(struct _lf_device *device, void *src, uint32_t length) {
    lf_assert(device, E_NULL, "invalid device");

    struct _lf_network_context *ctx = (struct _lf_network_context *)device->_ep_ctx;
    lf_assert(ctx, E_NULL, "invalid context");

    if (ctx->addr.sin_addr.s_addr == 0 || ctx->addr.sin_port == 0) {
        fprintf(stderr, "[lf_network_write] Cannot send to uninitialized addr (0.0.0.0:0)\n");
        return E_COMMUNICATION;
    }

    fprintf(stderr, "[lf_network_write] Sending %u bytes to %s:%d\n",
            length, inet_ntoa(ctx->addr.sin_addr), ntohs(ctx->addr.sin_port));

    ssize_t e = sendto(ctx->fd, src, length, 0, (struct sockaddr *)&ctx->addr,
                       sizeof(struct sockaddr_in));
    lf_assert(e > 0, E_COMMUNICATION,
              "Failed to send data to '%s' at '%s'.",
              ctx->host, inet_ntoa(ctx->addr.sin_addr));
    return lf_success;

fail:
    return lf_error;
}

int lf_network_release(void *_device) {
    struct _lf_device *device = _device;
    lf_assert(device, E_NULL, "invalid device");

    struct _lf_network_context *ctx = device->_ep_ctx;
    lf_assert(ctx, E_NULL, "invalid context");
    close(ctx->fd);
    return lf_success;

fail:
    return lf_error;
}
