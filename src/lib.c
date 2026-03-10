#include <tcp-server/lib.h>

void print_help(const char *program_name) {
   printf("%s is an evolving TCP server written in C.\n", program_name);
   printf("\n");
   printf("Usage: %s --ip <IP> --port <PORT> [OPTIONS]...\n", program_name);
   printf("\n");
   printf("Arguments:\n");
   printf("\t<IP> - IPv4 address owned by the host\n");
   printf("\t<PORT> - port number [1-65535]\n");
   printf("\t<PROTOCOL> - protocol that the server supports for communication [raw/http(default)]\n");
   printf("\n");
   printf("Options:\n");
   printf("\t--help\t\t\tPrints this message\n");
   printf("\t--ip <IP>\t\tNetwork interface address to listen on\n");
   printf("\t--port <PORT>\t\tA port number on which the server should listen\n");
   printf("\t--protocol <PROTOCOL>\tA protocol that the server will use to parse request and send response\n");
   printf("\n");
   printf("PROTOCOL\n");
   printf("raw: Read and respond with raw TCP messages.\n");
   printf("     Messages must be terminated with '\\r\\n'.\n");
}

ssize_t recv_until(int socket, char **buffer, int recv_flags, const char *terminator) {
   ssize_t terminator_length = strlen(terminator);
   ssize_t buffer_size = 1024;
   ssize_t buffer_length = 0;
   *buffer = malloc(buffer_size);
   if (!*buffer) return -1;
   (*buffer)[0] = '\0';

   while (1) {
      char chunk[1024];
      ssize_t bytes_read = recv(socket, chunk, sizeof(chunk), recv_flags);

      if (bytes_read < 0) {
         free(*buffer);
         *buffer = NULL;
         return -1;
      } else if (bytes_read == 0) break;

      ssize_t old_buffer_length = buffer_length;
      ssize_t new_buffer_size = buffer_length + bytes_read + 1;
      if (new_buffer_size > buffer_size) {
         char *tmp = realloc(*buffer, new_buffer_size);
         if (!tmp) {
            free(*buffer);
            *buffer = NULL;
            return -1;
         }
         *buffer = tmp;
         buffer_size = new_buffer_size;
      }

      memcpy(*buffer + buffer_length, chunk, bytes_read);
      buffer_length += bytes_read;
      (*buffer)[buffer_length] = '\0';

      // Check for terminator
      ssize_t terminator_offset = ssize_max(0, old_buffer_length - (terminator_length - 1));
      char *terminator_position = strstr(*buffer + terminator_offset, terminator);
      if (terminator_position) {
         *(terminator_position + terminator_length) = '\0';
         buffer_length = (ssize_t)((terminator_position + terminator_length) - *buffer);
         break;
      }
   }

   return buffer_length;
}
