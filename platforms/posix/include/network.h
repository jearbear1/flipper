/* network.h - Define and implement the network endpoint. */

#ifndef __lf_network_h__
#define __lf_network_h__

/* POSIX network includes. */
#include <arpa/inet.h>
#include <netdb.h>

/* The default port over which FMR can be accessed. */
#define LF_UDP_PORT 3258

struct _lf_network_context {
    int fd;
    char host[64];
    /*
     device is used on the client to target FVM.
     Keep device for outbound targeting logic (client).
    */
    struct sockaddr_in device;
    /*
     addr is used on the server (FVM) to reply to the sender of a UDP packet.
     Use addr as the dynamic return path (server).
     */
    struct sockaddr_in addr;
};

int lf_network_read(struct _lf_device *device, void *dst, uint32_t length);
int lf_network_write(struct _lf_device *device, void *src, uint32_t length);
int lf_network_release(void *device);

struct _lf_device *lf_network_device_for_hostname(char *hostname);

int lf_network_connect(struct _lf_device *device, const char *host, uint16_t port);

#endif
