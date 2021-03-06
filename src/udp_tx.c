/* unicast IPv6 UDP array transmit
 Michael Hirsch
 based on https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/udpserver.c

 usage:
 cc unicast_tx.c
 ./unicast_tx port

 nc -u ::1 2000    // assuming you selected the server to be on port 2000
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>

#ifdef _WIN32
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#ifdef __MINGW32__
#include <unistd.h>
#endif

static const size_t BUFSIZE=8192;
static const bool VERBOSE=true;

void error(char *msg, int sock) {
perror(msg);
close(sock);
exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
// setup
void serv();
struct sockaddr_in6 serveraddr;
// user mode setting
unsigned char ttl=1; //default

int port=2000;

if (argc>1) port = atoi(argv[1]);

// create socket
int s = socket(AF_INET6, SOCK_DGRAM, 0);
if (s < 0)
  error("ERROR opening socket",s);
if (VERBOSE)
  printf("opened socket %d \n", s);

printf("Using socket on port %d \n",port);
// instant restart capability
int optval = 1;
setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

// server IP
memset((char *) &serveraddr, 0, sizeof(serveraddr));
serveraddr.sin6_family = AF_INET6;
serveraddr.sin6_addr = in6addr_any;
serveraddr.sin6_port = htons((unsigned short)port);
// TTL
setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
// bind socket to port
if (bind(s, (struct sockaddr *) &serveraddr,  sizeof(serveraddr)) < 0)
  error("ERROR on binding",s);

if (VERBOSE)
  printf("socket bound %d \n", s);

// do work
serv(s);

return EXIT_SUCCESS;

}



void serv(int s) {

struct sockaddr_in6 cliadd;
char clistr[INET6_ADDRSTRLEN];

int last=0;
uint32_t Nel=BUFSIZE/4; // float is 4 bytes

char* buf = malloc(BUFSIZE*sizeof(char));

float* array = malloc(Nel*sizeof(float));
socklen_t clientlen = sizeof(cliadd);

if (VERBOSE)
    printf("starting Unicast TX loop, waiting for newline from receiver to send reply. \n");

bool first=false;

    // loop: echo UDP packets back to client
while (true){
  memset(buf, 0, BUFSIZE);
  // if (VERBOSE)
  //     printf("set buffer size %lu \n", BUFSIZE);
  ssize_t ret = recvfrom(s, buf, BUFSIZE, 0, (struct sockaddr *) &cliadd, &clientlen);
  if (ret < 0)
      error("ERROR in recvfrom",s);
  if(!first){
      printf("sending now: %lu \n", ret);
      first=true;
  }

  inet_ntop(AF_INET6, &(cliadd.sin6_addr), clistr, INET6_ADDRSTRLEN);

  //if (VERBOSE)
  //  printf("server received %lu/%zd bytes: %s\n", strlen(buf), ret, buf);

  // generate dummy data stream of float32
  for (size_t i=0; i<Nel; ++i)  array[i] = last+i;
  last+=Nel;

  //if (VERBOSE)
  //    printf("%d \n",Nel);
  //send length of float array first
  ret = sendto(s, &Nel, sizeof(Nel), 0, (struct sockaddr *) &cliadd, clientlen);
  // then send float32 array
  ret = sendto(s, array, Nel*sizeof(float), 0, (struct sockaddr *) &cliadd, clientlen);
  if (ret < 0)
      error("ERROR in sending array sendto",s);

} //while

free(buf);
free(array);
} //main
