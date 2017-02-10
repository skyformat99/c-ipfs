#ifndef DAEMON_H
    #define DAEMON_H
    #include <stdint.h>

    #define MAX 5
    #define CONNECTIONS 50

    struct null_connection_params {
        int socket;
        int *count;
    };

    struct null_listen_params {
        uint32_t ipv4;
        uint16_t port;
    };

    void *ipfs_null_connection (void *ptr);
    void *ipfs_null_listen (void *ptr);
    int ipfs_daemon (int argc, char **argv);
    int ipfs_ping (int argc, char **argv);
#endif // DAEMON_H
