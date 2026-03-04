#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*
 * Function that reads a file descriptor until the file ends or the end_seq gets added to the data.
 * RETURNS 0 if file ended and -1 on error.
*/
int my_read(int fd, char** buff, ssize_t *buff_size, const char *end_seq);
