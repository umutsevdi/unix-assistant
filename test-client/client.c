#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#define PORT 8081
#define MAXLINE 256

/**
 * gets the IP address of the given host name
 * @param String representation of the local name ex: pi.local, fedora
 * @return IP address as string ex: 192.168.1.1
 *
 */
char *get_ip_by_hostname(char *hostname) {
  struct hostent *host_entry = gethostbyname(hostname);
  if (host_entry == NULL) {
    return NULL;
  }
  return inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0]));
}

/**
 * sends a message to the given socket and returns whether
 * there have been an error while sending the message
 * @param sockfd socket
 * @param buffer to write on
 * @return whether connection should be kept or not
 *
 */
bool send_message(int sockfd, char *buffer) {
  fgets(buffer, MAXLINE, stdin);
  int len = strnlen(buffer, MAXLINE);
  int n = send(sockfd, buffer, len, 0);
  // !(n<0 || strncmp == 0)
  // Keep connection as long as the size is more than equal to 0
  // or the message is not "exit"
  return n >= 0 && strncmp(buffer, "exit", 4);
}

int main(int argc, char *argv[]) {
  int i = 0, sockfd;
  struct sockaddr_in address;
  struct hostent *serv_name;
  char ip[16];
  char hostname[256];
  printf("hostname: ");
  fgets(hostname, 255, stdin);
  int found = 0;
  while (!found && i < 256) {
    if (hostname[i] == '\n') {
      hostname[i] = '\0';
      found = 1;
    }
    i++;
  }
  char *host_ip = get_ip_by_hostname(hostname);
  if (host_ip == NULL) {
    printf("Error: Hostname %s, were not found\n", hostname);
    exit(0);
  }
  strncpy(ip, host_ip, 15);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("Error: socket creation failed");
    exit(0);
  }

  address.sin_family = AF_INET;
  address.sin_port = htons(PORT);
  address.sin_addr.s_addr = inet_addr(ip);

  printf("Connecting to %s:%d\n", ip, PORT);
  // Binding newly created socket to given IP and verification

  if (connect(sockfd, (struct sockaddr *)&address, sizeof(address)) != 0) {
    perror("Error: connection failed");
    exit(0);
  }

  char buffer[MAXLINE];
  bool connected = true;
  while (connected) {
    printf("%s$> ", hostname);
    connected = send_message(sockfd, buffer);
  }
  close(sockfd);
  return 0;
}
