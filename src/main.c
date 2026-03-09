#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROGRAM_NAME "tcp-server"

void print_help() {
   printf("%s is an evolving TCP server written in C.\n", PROGRAM_NAME);
   printf("Usage: %s --ip 127.0.0.1 --port 3000\n\n", PROGRAM_NAME);
   printf("Arguments:\n");
   printf("\t<IP> - ip address owned by the host\n");
   printf("\t<PORT> - port number [1-65535]\n");
   printf("\nOptions:\n");
   printf("\t--help\t\tPrints this message\n");
   printf("\t--ip <IP>\tNetwork interface address to listen on\n");
   printf("\t--port <PORT>\tA port number on which the server should listen\n");
}

int main(int argc, char *argv[]) {
   char *ip_arg = NULL;
   char *port_arg = NULL;

   for (int i = 1; i < argc; i++) {
      char *curr = argv[i];
      if (strcmp(curr, "--help") == 0) {
         print_help();
         exit(EXIT_SUCCESS);
      }
      else if (strcmp(curr, "--ip") == 0 && i + 1 < argc) ip_arg = argv[++i];
      else if (strcmp(curr, "--port") == 0 && i + 1 < argc) port_arg = argv[++i];
   }

   if (ip_arg == 0 || port_arg == 0) {
      fprintf(stderr, "%s: Missing options\n", PROGRAM_NAME);
      fprintf(stderr, "Try '%s --help'\n", PROGRAM_NAME);
      exit(EXIT_FAILURE);
   }

   // Define server constants
   const char* const SERVER_IP = ip_arg;

   errno = 0;
   char *end;
   const long SERVER_PORT = strtol(port_arg, &end, 10);
   if (errno != 0 || *end != '\0' || SERVER_PORT < 1 || SERVER_PORT > 65535) {
      fprintf(stderr, "%s: Invalid <PORT>\n", PROGRAM_NAME);
      fprintf(stderr, "Try '%s --help'\n", PROGRAM_NAME);
      exit(EXIT_FAILURE);
   }
}
