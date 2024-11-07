#include "socket.h"
#include <arpa/inet.h>  // htons, inet_addr
#include <fcntl.h>      // fcntl
#include <netdb.h>      // addrinfo, getaddrinfo
#include <netinet/in.h> // sockaddr_in
#include <stdio.h>
#include <string.h>     // strcmp
#include <sys/socket.h> // socket, connect
#include <unistd.h>     // close

void print_ai_connected(struct sockaddr* addr, int family)
{
    char ipstr[INET6_ADDRSTRLEN];
    void* ip_addr;
    int port;
    if(family == AF_INET) { // IPv4
        struct sockaddr_in* ipv4 = (struct sockaddr_in*)addr;
        ip_addr = &(ipv4->sin_addr);
        port = ntohs(ipv4->sin_port);
    } else { // IPv6
        struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)addr;
        ip_addr = &(ipv6->sin6_addr);
        port = ntohs(ipv6->sin6_port);
    }
    inet_ntop(family, ip_addr, ipstr, sizeof ipstr);
    printf("Connected to %s:%d\n", ipstr, port);
}

// Makes UDP socket, connects to node:service, and returns socket file
// descriptor
// Kill with close(fd)
// Does not bind to specific port, lets OS
// choose.
// uses connect() to set destination address, you can send to same
// address with send()
//
// node is IP or hostname
// service is port number or service name (e.g. "http")
int make_async_connected_send_socket(char* node, char* service)
{
    // --- GET LINKED LIST servinfo OF addrinfo STRUCTS CONTAINING
    // --- ADDRESS INFORMATION FOR node:service ---

    struct addrinfo hints, *servinfo;

    // fill in hints struct
    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM;  // UDP sockets

    int status; // return code for getaddrinfo
    // set linked-list of addrinfo structs to servinfo
    status = getaddrinfo(node, service, &hints, &servinfo);
    if(status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return GETADDRINFO_ERROR;
    }

    // --- LOOP THROUGH servinfo AND CONNECT TO FIRST SUCCESSFUL RESULT ---

    int sockfd;         // socket file descriptor
    struct addrinfo* p; // pointer to iterate through servinfo
    // loop through all the results and connect to the first we can
    // begin with head of servinfo linked list
    int connect_status; // status of connect() call
    for(p = servinfo; p != NULL; p = p->ai_next) {
        // attempt to create socket
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        // if socket creation fails, print error and continue to next node of
        // servinfo
        if(sockfd == -1) {
            perror("client: socket");
            continue;
        }

        // set socket to non-blocking mode
        int flags = fcntl(sockfd, F_GETFL, 0);
        if(flags == -1) {
            perror("fcntl F_GETFL");
            close(sockfd);
            continue;
        }
        if(fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
            perror("fcntl F_SETFL O_NONBLOCK");
            close(sockfd);
            continue;
        }

        connect_status = connect(sockfd, p->ai_addr, p->ai_addrlen);
        // if connection fails, print error, close socket and continue to next
        // node of servinfo
        if(connect_status == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        print_ai_connected(p->ai_addr, p->ai_family);

        // if we get here, we must have connected successfully, so stop looping
        // through servinfo
        break;
    }

    // free the linked-list now that we have connected socket
    freeaddrinfo(servinfo);

    // if p is NULL, then we have looped through all servinfo nodes and failed
    if(p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    // now a socket description exists. If it is not bound to a specific port
    // using bind() before its first send() or sendto() call, then the OS will
    // assign a random port number.

    return sockfd;
}