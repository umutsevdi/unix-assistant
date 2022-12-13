#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#define PORT 8081

#define HOSTNAME "pi2.local"
#define MAXLINE 256

/**
 * Gets the IP address of the given host name
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

int main(int argc, char *argv[]) {

  int sockfd, opt;
  int argCnt = 0, nRead, nWrite;
  struct sockaddr_in servAddr;
  struct hostent *servName;
  char ip[16];

  char *sv_ip_address = get_ip_by_hostname(HOSTNAME);
  strncpy(ip, sv_ip_address, 15);

  if (sv_ip_address == NULL) {
    perror("Error: Hostname " HOSTNAME ", were not found\n");
    exit(0);
  }

  char buffer[MAXLINE];
  char screen[MAXLINE];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("Error: socket creation failed");
    exit(0);
  }

  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(PORT);
  servAddr.sin_addr.s_addr = inet_addr(ip);

  printf("Connecting to %s:%d\n", ip, PORT);
  // Binding newly created socket to given IP and verification

  if (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) != 0) {
    perror("Error: connection failed");
    exit(0);
  }

  int connected = 1;
  while (printf("waiting for a command : ") && fgets(buffer, MAXLINE, stdin) &&
         connected) {
    nWrite = send(sockfd, buffer, strlen(buffer), 0);
    if (nWrite < 0) {
      perror("Error: send failed...\n");
      connected = 0;
    }
    if (strncpy(buffer, "exit", 4) == 0) {
      connected = 0;
    }
  }
  close(sockfd);
  return 0;
}
