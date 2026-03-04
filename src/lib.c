#include <stdio.h>
#include <string.h>
#include <tcp-server/lib.h>

int my_read(int fd, char **buff, ssize_t *buff_size, const char *end_seq) {
   *buff_size = 1024;
   ssize_t buff_len = 0;
   *buff = malloc(*buff_size);
   memset(*buff, 0, *buff_size);

   char chunk[1024];
   ssize_t bytes_read;
   while (strstr(*buff, end_seq) == NULL) {
      if ((bytes_read = read(fd, chunk, sizeof(chunk) - 1)) == -1) {
         free(*buff);
         *buff = NULL;
         *buff_size = 0;
         return -1;
      } else if (bytes_read == 0) break;

      int new_size = buff_len + bytes_read + 1;
      if (new_size > *buff_size) {
         char *tmp = realloc(*buff, new_size);
         if (!tmp) {
            free(*buff);
            *buff = NULL;
            *buff_size = 0;
            return -1;
         }
         *buff = tmp;
         *buff_size = new_size;
      }

      chunk[bytes_read] = '\0';
      memcpy(*buff + buff_len, chunk, bytes_read + 1);
      buff_len += bytes_read;
   }

   return 0;
}
