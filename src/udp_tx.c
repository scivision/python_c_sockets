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

#include "myerr.h"

static const size_t BUFSIZE=8192;
static const bool VERBOSE=true;


static void serv(int s, int Nloop) {

struct sockaddr_in6 cliadd;
char clistr[INET6_ADDRSTRLEN];

int last=0;
uint32_t Nel=BUFSIZE/4; // float is 4 bytes

char* buf = malloc(BUFSIZE*sizeof(char));
if (!buf)
    error("producer: allocating memory for buf", 0);

float* array = malloc(Nel*sizeof(float));
if (!array)
    error("producer: allocating memory for array", 0);

socklen_t clientlen = sizeof(cliadd);

printf("producer: starting Unicast TX loop, waiting for newline from receiver to send data loop.\n");

bool first=true;

char nel_buf[sizeof(Nel)];
memcpy(nel_buf, &Nel, sizeof(Nel));

char *arr_buf = malloc(Nel * sizeof(float));
if (!arr_buf)
    error("producer: allocating memory for arr_buf", 0);

// loop: echo UDP packets back to client
for (int i=0; i<Nloop; ++i) {

  memset(buf, 0, BUFSIZE);
  if (VERBOSE)
      printf("producer: set buffer size %zu \n", BUFSIZE);

  ssize_t ret = recvfrom(s, buf, BUFSIZE, 0, (struct sockaddr *) &cliadd, &clientlen);
  if (ret < 0)
      error("producer: recvfrom",s);
  if(first){
      printf("producer: sending now: %zu \n", ret);
      first=false;
  }

  inet_ntop(AF_INET6, &(cliadd.sin6_addr), clistr, INET6_ADDRSTRLEN);

  if (VERBOSE)
    printf("producer: received %zu/%zd bytes: %s\n", strlen(buf), ret, buf);

  // generate dummy data stream of float32
  for (size_t i=0; i<Nel; ++i)
    array[i] = last+i;

  last+=Nel;

  //if (VERBOSE)
  //    printf("%d \n",Nel);
  //send length of float array first
  ret = sendto(s, nel_buf, sizeof(nel_buf), 0, (struct sockaddr *) &cliadd, clientlen);
  if (ret < 0)
      error("producer: sendto (data length)",s);

  // then send float32 array
  memcpy(array, arr_buf, Nel*sizeof(float));
  ret = sendto(s, arr_buf, Nel*sizeof(float), 0, (struct sockaddr *) &cliadd, clientlen);
  if (ret < 0)
    error("producer: sendto (data)",s);

} // for

free(buf);
free(array);
free(arr_buf);
}

int main(int argc, char **argv)
{

#ifdef _WIN32
  // Initialize Winsock
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    fprintf(stderr, "producer: WSAStartup failed\n");
    return EXIT_FAILURE;
  }

  printf("producer: Winsock initialized\n");
#endif

// setup
struct sockaddr_in6 serveraddr;
// user mode setting
unsigned char ttl=1; //default

int port=2001;
if (argc>1)
  port = atoi(argv[1]);

int Nloop=5;
if (argc>2)
  Nloop = atoi(argv[2]);

// create socket
int s = socket(AF_INET6, SOCK_DGRAM, 0);
if (s < 0)
  error("producer: open socket",s);

printf("producer: socket %d on port %d \n", s, port);
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
  error("producer: bind",s);

if (VERBOSE)
  printf("producer: socket bound %d \n", s);

// do work
serv(s, Nloop);

#ifdef _WIN32
  closesocket(s);
  WSACleanup();
#else
  close(s);
#endif

return EXIT_SUCCESS;

}
