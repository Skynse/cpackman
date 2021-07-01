#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "linux/if_link.h"
#include <iostream>
#include <string.h>
#include <array>


std::array<unsigned int, 2> bytes_downloaded() {
    struct ifaddrs *ifaddr, *ifa;
    int family, s, n;
    char host[NI_MAXHOST];
    std::array<unsigned int, 2> bytes;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }
    for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
        //cycle through the interfaces //
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        if (family == AF_PACKET && ifa->ifa_data != NULL && strcmp(ifa->ifa_name, "wlan0")==0) {
            struct rtnl_link_stats *stats = (struct rtnl_link_stats *)ifa->ifa_data;
            bytes[0] = stats->tx_bytes;
            bytes[1] = stats->rx_bytes;
        }
    }

    freeifaddrs(ifaddr);
    return bytes;
}
