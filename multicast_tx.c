/*
modifed from the original at
http://web.cs.wpi.edu/~claypool/courses/4514-B99/samples/multicast.c

UDP multicast IPv6 transmit in C

Michael Hirsch

*/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv)
{

// user mode setting
char mcgroup[39]="ff08::1";
int mcport=2000;
if (argc>1) {
    memcpy(mcgroup,argv[1],15);
}
if (argc>2) {
    mcport  = atoi(argv[2]);
}

   printf("sending on %s port %d \n",mcgroup,mcport);
   struct sockaddr_in6 group;
   int sock;

   /* set up socket */
   sock = socket(AF_INET6, SOCK_DGRAM, 0);
   if (sock < 0) {
     perror("socket");
     exit(1);
   }
// instant restart capability
  int optval = 1;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
	     (const void *)&optval , sizeof(int));
// multicast group config
   group.sin6_family = AF_INET6;
   group.sin6_port = htons(mcport);
   group.sin6_addr = in6addr_any;

// reference 
// https://docs.oracle.com/cd/E19455-01/806-1017/auto1/index.html

// default is to loopback (loop=1) normally leave this alone.
//    uint_t loop=1;
//    setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &loop, sizeof(loop)) 

// control number of hops within scope, default is 1 hop (single subnet)
//    uint_l hops=1;
//    setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &hops,sizeof(hops))


     int cnt;
     char message[100];
     int addrlen=sizeof(group);
      while (true) {
	 time_t t = time(0);
	 sprintf(message, "%-24.24s", ctime(&t));
	 cnt = sendto(sock, message, sizeof(message), 0,
		      (struct sockaddr *) &group, addrlen);
	 if (cnt < 0) {
 	    perror("sendto");
	    exit(1);
	 }
	 sleep(1);
      }
}
