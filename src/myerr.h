#ifndef MYERR_H
#define MYERR_H

#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <stddef.h>  // for ptrdiff_t

#ifndef ssize_t
#ifdef _WIN32
#include <BaseTsd.h>
#define ssize_t SSIZE_T
#else
#define ssize_t ptrdiff_t
#endif
#endif

#if __has_c_attribute(noreturn)
[[ noreturn ]]
#endif
static void error(char *msg, int sock) {

#ifdef _WIN32
  // https://learn.microsoft.com/en-us/windows/win32/winsock/windows-sockets-error-codes-2
  fprintf(stderr, "%s Error code: %d\n", msg, WSAGetLastError());
  closesocket(sock);
#else
  perror(msg);
  close(sock);
#endif

  exit(EXIT_FAILURE);
}

#endif // MYERR_H
