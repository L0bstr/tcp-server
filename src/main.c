#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <tcp-server/lib.h>

void print_help() {
   printf("server is a minmal tcp server made in c.\n");
   printf("Usage: server --port <PORT>\n\n");
   printf("Arguments:\n");
   printf("\t<PORT> - port number [0-65535]\n");
   printf("\nOptions:\n");
   printf("\t--help       \tPrints this messages\n");
   printf("\t--port <PORT>\tA port number on which the server should run\n");
}

int main(int argc, char *argv[]) {
   const char *SERVER_IP = "127.0.0.1";
   char *port_arg = 0;

   for (int i = 1; i < argc; i++) {
      char *curr = argv[i];
      if (strstr(curr, "--help")) {
         print_help();
         return 0;
      } else if (strstr(curr, "--port") && i + 1 < argc) port_arg = argv[i + 1];
   }
   if (port_arg == 0) {
      printf("Wrong usage\n");
      print_help();
      exit(1);
   }

   // Convert port argument to a number
   errno = 0;
   char *end;
   const int SERVER_PORT = strtol(port_arg, &end, 10);
   if (errno != 0 || end == port_arg || SERVER_PORT < 0 || SERVER_PORT > 65535) {
      printf("Invalid port number\n");
      print_help();
      exit(EXIT_FAILURE);
   }

   // Create server socket
   int server_socket = socket(AF_INET, SOCK_STREAM, 0);
   if (server_socket == -1) {
      printf("Failed to create server socket\n");
      exit(EXIT_FAILURE);
   }

   // Create server address
   struct sockaddr_in server_address;
   server_address.sin_family = AF_INET;
   server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
   server_address.sin_port = htons(SERVER_PORT);

   // -- allow to use port while TIME_WAIT
   int opt = 1;
   setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

   // Bind address to socket
   if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
      perror("Failed to bind server address to socket");
      close(server_socket);
      exit(EXIT_FAILURE);
   }

   // Listen to connections
   const int MAX_QUEUE = 10;
   if (listen(server_socket, MAX_QUEUE) == -1) {
      perror("Failed to listen for connections");
      close(server_socket);
      exit(EXIT_FAILURE);
   }

   // prepare client data
   int client_socket;
   struct sockaddr_in client_address;
   socklen_t client_address_length = sizeof(client_address);

   while (1) {
      printf("Listening on http://%s:%d\n", SERVER_IP, SERVER_PORT);

      // Accept connection
      client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_length);
      if (client_socket == -1) {
         perror("Failed to accpet client connection");
         close(client_socket);
         close(server_socket);
         exit(EXIT_FAILURE);
      }

      // Get client info
      char *client_ip = inet_ntoa(client_address.sin_addr);
      int client_port = ntohs(client_address.sin_port);
      printf("-> Client connected: %s:%d\n", client_ip, client_port);

      // Read client message
      char *message;
      ssize_t message_size;
      my_read(client_socket, &message, &message_size, "\r\n\r\n");
      printf("-> Message:\n");
      printf("%s", message);

      const char *response =
         "HTTP/1.1 200 OK\r\n"
         "Content-Type: text/plain\r\n"
         "Content-Length: 31\r\n"
         "\r\n"
         "Hello, from the Lobster server!";

      // Send response to client
      if (send(client_socket, response, strlen(response), 0) == -1) {
         perror("Failed to send response to client");
         close(client_socket);
         close(server_socket);
         exit(EXIT_FAILURE);
      }
      printf("-> Message sent to client\n");

      close(client_socket);
      printf("-> Client connection closed\n");
      printf("---------------------------\n");
   }

   close(server_socket);
   return 0;
}
