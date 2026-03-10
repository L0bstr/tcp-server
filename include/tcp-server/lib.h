#pragma once

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

/*
 * Receives data from a socket until a terminator sequence is found.
 * Returns the length of data in buffer, or -1 on error (errno set).
*/
ssize_t recv_until(int socket, char **buffer, int recv_flags, const char *terminator);

/*
 * Returns the bigger number.
*/
static inline ssize_t ssize_max(ssize_t a, ssize_t b) { return a > b ? a : b; }
