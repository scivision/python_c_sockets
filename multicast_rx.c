/*
modifed from the original at
http://web.cs.wpi.edu/~claypool/courses/4514-B99/samples/multicast.c

UDP multicast IPv6 receive in C

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
#include <time.h>
#include <stdio.h>

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

   printf("listening on %s port %d \n",mcgroup,mcport);
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

    //receiver multicast
      struct ipv6_mreq mreq;
      inet_pton(AF_INET6, mcgroup, &(mreq.ipv6mr_multiaddr));
      mreq.ipv6mr_interface = INADDR_ANY;   // any interface
      if (setsockopt(sock,IPPROTO_IPV6,IPV6_JOIN_GROUP, &mreq,sizeof(mreq)) < 0) {
	     perror("setsockopt mreq");
	     exit(1);
      }

      if (bind(sock, (struct sockaddr *) &group, sizeof(group)) < 0) {
         perror("bind");
	    exit(1);
      }


     char message[100];
     int cnt;
     int addrlen=sizeof(group);
     char serverstr[INET6_ADDRSTRLEN];


      while (true) {
 	 cnt = recvfrom(sock, message, sizeof(message), 0,
			(struct sockaddr *) &group, &addrlen);
	 if (cnt < 0) {
	    perror("recvfrom");
	    exit(1);
	 } else if (cnt == 0) {
 	    break;
	 }
	 printf("%s: message = \"%s\"\n",
             inet_ntop(AF_INET6, &group.sin6_addr, serverstr,
                       INET6_ADDRSTRLEN), message);
        }
}


