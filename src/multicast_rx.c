/*
modifed from the original at
http://web.cs.wpi.edu/~claypool/courses/4514-B99/samples/multicast.c

UDP multicast IPv6 receive in C
*/

#ifdef _WIN32
#ifndef UNICODE
#define UNICODE
#endif
#define WIN32_LEAN_AND_MEAN
#endif

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdio.h>

#ifdef _WIN32
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#ifdef __MINGW32__
#include <unistd.h>
#endif

#include "myerr.h"

int main(int argc, char **argv)
{

#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
        fprintf(stderr, "consumer: WSAStartup failed\n");
        return EXIT_FAILURE;
    }
#endif

// user mode setting
char mcgroup[39]="ff08::1";
int mcport=2000;
if (argc>1)
  memcpy(mcgroup,argv[1],15);

if (argc>2)
  mcport  = atoi(argv[2]);

int Nloop=5;
if (argc>3)
  Nloop = atoi(argv[3]);

printf("consumer: listening on %s port %d \n",mcgroup,mcport);
struct sockaddr_in6 group;

/* set up socket */
int sock = socket(AF_INET6, SOCK_DGRAM, 0);
if (sock < 0)
  error("consumer: socket open failure",sock);
// instant restart capability
int optval = 1;
setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
#ifdef _WIN32
  (const char *)&optval,
#else
  (const void *)&optval,
#endif
 sizeof(int));
// multicast group config
group.sin6_family = AF_INET6;
group.sin6_port = htons(mcport);
group.sin6_addr = in6addr_any;

// reference
// https://docs.oracle.com/cd/E19455-01/806-1017/auto2/index.html


//receiver multicast
struct ipv6_mreq mreq;
inet_pton(AF_INET6, mcgroup, &(mreq.ipv6mr_multiaddr));
mreq.ipv6mr_interface = INADDR_ANY;   // any interface

// https://linux.die.net/man/3/setsockopt
// https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-setsockopt

#ifdef _WIN32
if (setsockopt(sock, IPPROTO_IPV6, IPV6_JOIN_GROUP, (char *)&mreq, sizeof(mreq)) < 0)
  error("consumer: setsockopt mreq", sock);
#else
if (setsockopt(sock, IPPROTO_IPV6, IPV6_JOIN_GROUP, &mreq, sizeof(mreq)) < 0)
  error("consumer: setsockopt mreq", sock);
#endif


if (bind(sock, (struct sockaddr *) &group, sizeof(group)) < 0)
  error("consumer: bind",sock);

char message[100];
long cnt;
unsigned int addrlen=sizeof(group);
char serverstr[INET6_ADDRSTRLEN];

for (int i = 0; i < Nloop; i++) {
  cnt = recvfrom(sock, message, sizeof(message), 0, (struct sockaddr *) &group, &addrlen);
  if (cnt < 0) {
      error("consumer: recvfrom",sock);
  }
  else if (cnt == 0) {
      break;
  }
  printf("consumer: %s: message = \"%s\"\n",
            inet_ntop(AF_INET6, &group.sin6_addr, serverstr, INET6_ADDRSTRLEN), message);
}

#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif
}
