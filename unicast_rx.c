/* unicast IPv6 UDP array transmit
 Michael Hirsch
 based on https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/udpserver.c
*/

#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUFSIZE 8192

void error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {

    
    int s, ret, Nel;
    socklen_t serverlen;
    struct sockaddr_in6 serveraddr;
    struct addrinfo *server;

    char *hostname;
    hostname="::1";
    int port=2000;

    char buf[1]="\n";
    float array[BUFSIZE];

    if (argc>1) 
        hostname = argv[1];
    if (argc>2)
        port = atoi(argv[2]);

    /* socket: create the socket */
    s = socket(AF_INET6, SOCK_DGRAM, 0);
    if (s < 0) 
        perror("ERROR opening socket");

    ret = getaddrinfo(hostname,NULL,NULL,&server);

    /* build the server's Internet address */
    memset((char *) &serveraddr,0, sizeof(serveraddr));
    serveraddr.sin6_family = AF_INET6;
    serveraddr.sin6_port = htons(port);

    while (true)
    {
    // ask server for data (demo server expects simply a line return)
    serverlen = sizeof(serveraddr);
    ret = sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &serveraddr, serverlen);
    if (ret < 0) 
      perror("ERROR in sendto");
    
    // get the length of data
    ret = recvfrom(s, &Nel, sizeof(Nel), 0, (struct sockaddr*) &serveraddr, &serverlen);
    if (ret < 0) 
      error("ERROR in recvfrom");
    

    // get the data
    ret = recvfrom(s, array, BUFSIZE, 0, (struct sockaddr*) &serveraddr, &serverlen);
    if (ret < 0) 
      error("ERROR in recvfrom");
    }


    return (EXIT_SUCCESS);
}

