
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void error() {
  perror("ERROR");
  exit(errno);
}

void post(char *buf, char *out, int out_size) {

  printf("POST CALLED\n");

  char *substring = strstr(buf, "\r\n\r\n");

  if (substring == NULL) {
    error();
  }

  char *body = substring + 4;

  printf("BODY %s\n", body);

  char *name = strstr(body, "name");

  if (name == NULL) {
    error();
  }

  char *nameVal = name + 5;

  snprintf(out, out_size, "HTTP/1.1 200 OK\r\n\r\nHello %s", nameVal);
}

void get(char *buf, char *out) { out = "HTTP/1.1 200 OK\r\n\r\nHello"; }

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd == -1) {
    error();
  }

  struct in_addr ip = {.s_addr = htonl(INADDR_LOOPBACK)};

  struct sockaddr_in addr = {
      .sin_family = AF_INET,
      .sin_port = htons(8080),
      .sin_addr = ip,
  };

  struct sockaddr *genAddr = (struct sockaddr *)&addr;

  int bindRes = bind(sockfd, genAddr, sizeof(*genAddr));

  if (bindRes != 0) {
    error();
  }

  int listenRes = listen(sockfd, 10);

  if (listenRes == -1) {
    error();
  }

  while (1) {
    int clientfd = accept(sockfd, NULL, NULL);

    if (clientfd == -1) {
      error();
    }

    char buf[4096];
    ssize_t n = read(clientfd, buf, sizeof(buf) - 1);
    buf[n] = '\0';

    if (n == -1) {
      error();
    }

    printf("%s\n", buf);

    char out[256];

    if (strstr(buf, "POST") != NULL) {
      post(buf, out, sizeof(out));
    }

    if (strstr(buf, "GET") != NULL) {
      get(buf, out);
    }

    write(clientfd, out, strlen(out));

    printf("wrote:\n\n%s\n\n to caller", out);
    close(clientfd);
  }
}
