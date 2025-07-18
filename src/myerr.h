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
    perror(msg);
    close(sock);
    exit(EXIT_FAILURE);
}

#endif // MYERR_H
