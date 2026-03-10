#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <tcp-server/lib.h>

#define PROGRAM_NAME "tcp-server"

#define PROTOCOL_RAW 1
#define PROTOCOL_HTTP 2

void print_help() {
   printf("%s is an evolving TCP server written in C.\n", PROGRAM_NAME);
   printf("\n");
   printf("Usage: %s --ip <IP> --port <PORT> [OPTIONS]...\n", PROGRAM_NAME);
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

int main(int argc, char *argv[]) {
   char *ip_arg = NULL;
   char *port_arg = NULL;
   char *protocol_arg = NULL;

   for (int i = 1; i < argc; i++) {
      char *curr = argv[i];
      if (strcmp(curr, "--help") == 0) {
         print_help();
         exit(EXIT_SUCCESS);
      }
      else if (strcmp(curr, "--ip") == 0 && i + 1 < argc) ip_arg = argv[++i];
      else if (strcmp(curr, "--port") == 0 && i + 1 < argc) port_arg = argv[++i];
      else if (strcmp(curr, "--protocol") == 0 && i + 1 < argc) protocol_arg = argv[++i];
      else if (curr[0] == '-') {
         fprintf(stderr, "Invalid option: '%s'\n", curr);
         fprintf(stderr, "Try '%s --help'\n", PROGRAM_NAME);
         exit(EXIT_FAILURE);
      }
   }

   if (ip_arg == NULL || port_arg == NULL) {
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

   int protocol = 0;
   if (protocol_arg == NULL || strcmp(protocol_arg, "http")) protocol = PROTOCOL_HTTP;
   else if (strcmp(protocol_arg, "raw")) protocol = PROTOCOL_RAW;

   // Create server address
   struct sockaddr_in server_address;
   server_address.sin_family = AF_INET;
   if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) == 0) {
      fprintf(stderr, "%s: Invalid <IP>\n", PROGRAM_NAME);
      fprintf(stderr, "Try '%s --help'\n", PROGRAM_NAME);
      exit(EXIT_FAILURE);
   }
   server_address.sin_port = htons(SERVER_PORT);

   // Create server socket
   int server_socket = socket(AF_INET, SOCK_STREAM, 0);
   if (server_socket == -1) {
      perror("Failed to create server socket");
      exit(EXIT_FAILURE);
   }

   // -- allow reusing a port that's in TIME_WAIT
   int opt = 1;
   setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

   // Bind server address to server socket
   if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
      perror("Failed to bind server address to server socket");
      close(server_socket);
      exit(EXIT_FAILURE);
   }

   // Listen for connections on server socket
   if (listen(server_socket, SOMAXCONN) == -1) {
      perror("Failed to listen for connections on server socket");
      close(server_socket);
      exit(EXIT_FAILURE);
   }
   printf("Server listens on %s:%ld\n", SERVER_IP, SERVER_PORT);

   // Accept client connection
   struct sockaddr_in client_address;
   socklen_t client_address_length = sizeof(client_address);
   int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_length);
   if (client_socket == -1) {
      perror("Failed to accept client connection");
      close(server_socket);
      exit(EXIT_FAILURE);
   }

   // Get client data
   char client_ip[INET_ADDRSTRLEN];
   inet_ntop(AF_INET, &client_address.sin_addr, client_ip, sizeof(client_ip));
   int client_port = ntohs(client_address.sin_port);
   printf("-> Client connected: %s:%d\n", client_ip, client_port);

   // Receive request
   char *request;
   ssize_t request_size = recv_until(client_socket, &request, 0, "\r\n\r\n");
   if (request_size < 0) {
      perror("Failed to read request");
      close(client_socket);
      close(server_socket);
      exit(EXIT_FAILURE);
   }

   printf("-> Request:\n%s\n", request);
   free(request);

   // Send response
   const char *response = 
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n"
      "Content-Length: 24\r\n"
      "\r\n"
      "Hello, from tcp-server!\n";

   if (send(client_socket, response, strlen(response), 0) == -1) {
      perror("Failed to send response");
      close(client_socket);
      close(server_socket);
      exit(EXIT_FAILURE);
   }
   printf("-> Response sent\n");

   close(client_socket);
   close(server_socket);

   return EXIT_SUCCESS;
}
