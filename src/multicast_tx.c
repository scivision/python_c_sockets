/*
modifed from the original at
http://web.cs.wpi.edu/~claypool/courses/4514-B99/samples/multicast.c

UDP multicast IPv6 transmit in C

example

./multicast_tx   #uses default interface, address, and port
./multicast_tx ff02::1 50000 # sets address and port
./multicast_tx ff02::1 50000 eth0  #additionally sets interface

Michael Hirsch

ref: http://tldp.org/HOWTO/Multicast-HOWTO-6.html
*/

#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <iphlpapi.h>
#else
#include <unistd.h>
#include <sys/socket.h> // for socket()
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#endif

#ifdef __MINGW32__
#include <unistd.h>
#elif defined(_MSC_VER)
#include <crtdbg.h>
#endif

#include "myerr.h"

int main(int argc, char **argv)
{
#ifdef _MSC_VER
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

#ifdef _WIN32
  // Initialize Winsock
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    fprintf(stderr, "WSAStartup failed\n");
    return EXIT_FAILURE;
  }
#endif

// interface selection
//struct in_addr interface_addr;


// user mode setting
char mcgroup[39]="ff08::1";
int mcport=2000;
if (argc>1)
  memcpy(mcgroup,argv[1],15);
if (argc>2)
  mcport  = atoi(argv[2]);

printf("sending on %s port %d \n",mcgroup,mcport);
struct sockaddr_in6 group;
int sock;

/* set up socket */
// https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket
// https://www.man7.org/linux/man-pages/man2/socket.2.html
sock = socket(AF_INET6, SOCK_DGRAM, 0);
if (sock < 0)
  error("socket open fail", sock);

// instant restart capability
int optval = 1;
// https://www.man7.org/linux/man-pages/man3/setsockopt.3p.html
// https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-setsockopt
if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int)) != 0) {
  error("setsockopt SO_REUSEADDR failed", sock);
}
printf("socket %d opened\n", sock);
// multicast group config
group.sin6_family = AF_INET6;
group.sin6_port = htons(mcport);
if (inet_pton(AF_INET6, mcgroup, &group.sin6_addr) != 1) {
  error("invalid multicast address", sock);
}
printf("group address set to %s port %d\n", mcgroup, mcport);

// reference
// https://docs.oracle.com/cd/E19455-01/806-1017/auto1/index.html

// default is to loopback (loop=1) normally leave this alone.
//    uint_t loop=1;
//    setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &loop, sizeof(loop))

// control number of hops within scope, default is 1 hop (single subnet)
//    uint_l hops=1;
//    setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &hops,sizeof(hops))

// optional bind to interface
if (argc>3){
  printf("binding to interface %s\n",argv[3]);

  char ifname[10];
  strcpy(ifname,argv[3]);

  unsigned int ifr = if_nametoindex(ifname);

#ifdef _WIN32
  if ((setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_IF, (char *)&ifr, sizeof(ifr))) < 0)
#else
  if ((setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_IF, &ifr, sizeof(ifr))) < 0)
#endif
    error("setsockopt IPV6_MULTICAST_IF failed", sock);

  printf("bound to interface %s\n", ifname);

}

// https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-bind
char caddr[INET6_ADDRSTRLEN];
inet_ntop(AF_INET6, &group.sin6_addr, caddr, INET6_ADDRSTRLEN);

printf("binding to group address %s port %d\n", caddr, ntohs(group.sin6_port));

if(bind(sock, (struct sockaddr *)&group, sizeof(group)) != 0) {
  error("bind", sock);
}
printf("bind ok\n");

// main loop
long cnt;
char message[100];
unsigned int addrlen=sizeof(group);

while (true) {
  time_t t = time(0);
  sprintf(message, "%-24.24s", ctime(&t));
  cnt = sendto(sock, message, sizeof(message), 0, (struct sockaddr *) &group, addrlen);
  if (cnt < 0)
    error("sendto",sock);

#ifdef _MSC_VER
  Sleep(1000);
#else
  sleep(1);
#endif
}

#ifdef _WIN32
  closesocket(sock);
  WSACleanup();
#else
  close(sock);
#endif

return EXIT_SUCCESS;

}
