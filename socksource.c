// simple IPv6 udp array source
// Michael Hirsch
// based on https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/udpserver.c
//
// usage:
// make socksource.c
//
// ./socksource 1 // will return float array of sequential values length BUFSIZE/4, upon receiving a newline
// ./socksource 0 // will echo back your text
// nc -u ::1 2000    // assuming you selected the server to be on port 2000 below

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define PORT 2000
#define BUFSIZE 8192

void error(char *msg) {
  perror(msg);
  exit(1);
}


int main(int argc, char **argv)
{
// setup
void serv();
struct sockaddr_in6 serveraddr;
// user mode setting
int dotalk=0;
if (argc>1) {
    dotalk = atoi(argv[1]);
}


// create socket
int s = socket(AF_INET6, SOCK_DGRAM, 0);
 if (s < 0) 
    error("ERROR opening socket");
// instant restart capability
  int optval = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));
// server IP
  memset((char *) &serveraddr, 0, sizeof(serveraddr));
  serveraddr.sin6_family = AF_INET6;
  serveraddr.sin6_addr = in6addr_any;
  serveraddr.sin6_port = htons((unsigned short)PORT);
// bind socket to port
  if (bind(s, (struct sockaddr *) &serveraddr, 
	   sizeof(serveraddr)) < 0) 
    error("ERROR on binding");

// do work
    serv(s,dotalk);

}



void serv(int s,int dotalk){

    struct sockaddr_in6 cliadd;
    char clistr[INET6_ADDRSTRLEN];
    struct hostent *hostp;

    int i;
    int last=0;
    int ret;
    int Nel=BUFSIZE/4; // float is 4 bytes
    char buf[BUFSIZE];
    float array[BUFSIZE];
    int clientlen = sizeof(cliadd);

    // loop: echo UDP packets back to client
    while (1) 
    {
        memset(buf, 0,BUFSIZE);
        ret = recvfrom(s, buf, BUFSIZE, 0, (struct sockaddr *) &cliadd, &clientlen);
        //if (ret < 0)    error("ERROR in recvfrom");

         // gethostbyaddr: determine who sent the datagram
        hostp = gethostbyaddr((const char *)&cliadd.sin6_addr, 
                                 sizeof(cliadd.sin6_addr), AF_INET6);

        //if (hostp == NULL)   error("ERROR on gethostbyaddr");
         inet_ntop(AF_INET6,&(cliadd.sin6_addr), clistr, INET6_ADDRSTRLEN);

        //if (clistr == NULL)  error("ERROR on inet_ntop\n");
        //printf("server received datagram from %s (%s)\n", hostp->h_name, clistr);
        //printf("server received %d/%d bytes: %s\n", strlen(buf), ret, buf);
        
    if (dotalk==1){ // blast float32 array mode
    	// generate dummy data stream of float32
        for (i=0; i<Nel; ++i)  array[i] = last+i;
        last+=Nel;
        
        //send length of float array first
        ret = sendto(s, &Nel,   sizeof(Nel),   0, (struct sockaddr *) &cliadd, clientlen);
        // then send float32 array
        ret = sendto(s, array, sizeof(array), 0, (struct sockaddr *) &cliadd, clientlen);
    }
    else{ // echo parrot mode
        ret = sendto(s, buf, strlen(buf), 0, (struct sockaddr *) &cliadd, clientlen);
        //if (ret < 0)  error("ERROR in sendto");
    }

    }

}



