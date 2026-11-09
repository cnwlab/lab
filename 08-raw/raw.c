#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>

int main()
{
    int sockfd, size;
    unsigned char buffer[65536];

    // Create raw socket
    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if(sockfd < 0)
    {
        perror("Socket error");
        return 1;
    }

    printf("PACKET CAPTURING STARTED...\n");
    printf("Press Ctrl+C to stop\n");

    while(1)
    {
        size = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);

        if(size < sizeof(struct ethhdr))
            continue;

        // Ethernet Header
        struct ethhdr *eth = (struct ethhdr *)buffer;

        printf("\n-----------------------------\n");
        printf("Packet Length: %d\n", size);

        printf("Source MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
            eth->h_source[0], eth->h_source[1],
            eth->h_source[2], eth->h_source[3],
            eth->h_source[4], eth->h_source[5]);

        printf("Destination MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
            eth->h_dest[0], eth->h_dest[1],
            eth->h_dest[2], eth->h_dest[3],
            eth->h_dest[4], eth->h_dest[5]);

        printf("Ethernet Protocol: %04X\n", ntohs(eth->h_proto));

        // Check IPv4
        if(ntohs(eth->h_proto) == ETH_P_IP &&
           size >= sizeof(struct ethhdr) + sizeof(struct iphdr))
        {
            struct iphdr *ip = (struct iphdr *)(buffer + sizeof(struct ethhdr));

            int ip_len = ip->ihl * 4;
            int offset = sizeof(struct ethhdr) + ip_len;

            if(ip_len < 20 || size < offset)
                continue;

            printf("Source IP: %s\n", inet_ntoa(*(struct in_addr *)&ip->saddr));
            printf("Destination IP: %s\n", inet_ntoa(*(struct in_addr *)&ip->daddr));
            printf("TTL: %d\n", ip->ttl);
            printf("IP Protocol: %d\n", ip->protocol);

            // TCP
            if(ip->protocol == IPPROTO_TCP && size >= offset + sizeof(struct tcphdr))
            {
                struct tcphdr *tcp = (struct tcphdr *)(buffer + offset);

                printf("TCP Header\n");
                printf("Source Port: %d\n", ntohs(tcp->source));
                printf("Destination Port: %d\n", ntohs(tcp->dest));
            }

            // UDP
            else if(ip->protocol == IPPROTO_UDP && size >= offset + sizeof(struct udphdr))
            {
                struct udphdr *udp = (struct udphdr *)(buffer + offset);

                printf("UDP Header\n");
                printf("Source Port: %d\n", ntohs(udp->source));
                printf("Destination Port: %d\n", ntohs(udp->dest));
            }

            // ICMP
            else if(ip->protocol == IPPROTO_ICMP && size >= offset + sizeof(struct icmphdr))
            {
                struct icmphdr *icmp = (struct icmphdr *)(buffer + offset);

                printf("ICMP Header\n");
                printf("Type: %d\n", icmp->type);
                printf("Code: %d\n", icmp->code);
            }
        }
    }

    close(sockfd);
    return 0;
}
