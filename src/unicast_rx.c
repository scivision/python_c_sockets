/* unicast IPv6 UDP array transmit
 Michael Hirsch, Ph.D.
 based on https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/udpserver.c

If using loopback, expect several Gb/sec. Otherwise speed limited by network interface

1. ./unicast_rx  (from unicast_rx.c)
2. ./unicast_tx  (from unicast_tx.c)
3. iftop -i lo  (to monitor traffic amount)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <math.h>

static const int BUFSIZE=8192;

void error(char *msg, int sock) __attribute__ ((noreturn));

void error(char *msg, int sock) {
    perror(msg);
    close(sock);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {

    ssize_t ret;
    uint32_t Nel = BUFSIZE/4; // float32
    socklen_t serverlen;
    struct sockaddr_in6 serveraddr;
    struct addrinfo *server;

    char *hostname = "::1";
    int port=2000;

    char buf[1]="\n";
    float * array;
    array = malloc(Nel*sizeof(float));

    if (argc>1)
        hostname = argv[1];
    if (argc>2)
        port = atoi(argv[2]);

    /* socket: create the socket */
    int s = socket(AF_INET6, SOCK_DGRAM, 0);
    if (s < 0)
        error("ERROR opening socket",s);

    ret = getaddrinfo(hostname,NULL,NULL,&server);

    /* build the server's Internet address */
    memset((char *) &serveraddr,0, sizeof(serveraddr));
    serveraddr.sin6_family = AF_INET6;
    serveraddr.sin6_port = htons(port);


    bool first = true;
    float last=0.;
    // loop
    while (true)
    {
    // ask server for data (demo server expects simply a line return)
    serverlen = sizeof(serveraddr);
    ret = sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &serveraddr, serverlen);
    if (ret < 0)
      error("ERROR in sendto",s);
    //printf("sent %x \r",buf);

    // get the length of data
    ret = recvfrom(s, &Nel, sizeof(Nel), 0, (struct sockaddr*) &serveraddr, &serverlen);
    if (ret < 0)
      error("ERROR in recvfrom (data length)",s);


    // get the data
    ret = recvfrom(s, array, BUFSIZE, 0, (struct sockaddr*) &serveraddr, &serverlen);
    if (ret < 0)
      error("ERROR in recvfrom (payload)",s);

    // check the data
    if (first) {
        first = false;
        last = array[0]-1;
        printf("Initial last: %f\n",last);
    }

    if (fabsf(array[0]-(float)1.)<0.99){
        printf("last: %f data: %f\n",last,array[0]-1);
        printf("may be wrapping in float32");
        return EXIT_FAILURE;
    }


    last = array[Nel-1];

    }


free(array);
return EXIT_SUCCESS;
}
