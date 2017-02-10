#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include "libp2p/net/p2pnet.h"

#define BUF_SIZE 4096

int ipfs_ping (int argc, char **argv)
{
    int socketfd, i, count=10, tcount = 0;
    uint32_t ipv4;
    uint16_t port;
    char b[BUF_SIZE];
    size_t len;
    struct timeval time;
    long cur_time, old_time;
    double ms, total = 0;

    if (inet_pton (AF_INET, argv[2], &ipv4) == 0) {
        fprintf(stderr, "Unable to use '%s' as an IP address.\n", argv[1]);
        return 1;
    }

    if ((port = atoi(argv[3])) == 0) {
        fprintf(stderr, "Unable to use '%s' port.\n", argv[2]);
        return 1;
    }

    if ((socketfd = socket_tcp4()) <= 0) {
        perror("can't create socket");
        return 1;
    }

    if (socket_connect4(socketfd, ipv4, port) < 0) {
        perror("fail to connect");
        return 1;
    }

    fprintf(stderr, "PING %s.\n", argv[2]);

    for (i=0 ; i < count ; i++) {
        if (gettimeofday (&time, 0)) return -1;
        old_time = 1000000 * time.tv_sec + time.tv_usec;

        socket_write(socketfd, "ping", 4, 0);
        len = socket_read(socketfd, b, sizeof(b), 0);

        if (len == 4 && memcmp(b, "pong", 4) == 0) {
            if (gettimeofday (&time, 0)) return -1;
            cur_time = 1000000 * time.tv_sec + time.tv_usec;
            ms = (cur_time - old_time) / 1000.0;
            total += ms; tcount++;
            fprintf(stderr, "Pong received: time=%.2f ms\n", ms);
        }
        sleep (1);
    }
    fprintf(stderr, "Average latency: %.2fms\n", total / tcount);

    return 0;
}
