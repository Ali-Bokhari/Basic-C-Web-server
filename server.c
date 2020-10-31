#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 20498
#define RSIZE 8192
#define BSIZE 4096

int main() {
  int soc, soc_in, ret, fsize;
  struct sockaddr_in server, client;
  unsigned int client_size = sizeof(client);
  FILE *fp;
  char buffer[RSIZE] = {0};
  char fbuffer[BSIZE];
  char file[128];

  char *find;
  char *f404 = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 18\n\n404 Page Not Found";

  client.sin_family = AF_INET;
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = INADDR_ANY;
  memset(&(server.sin_zero), 0, 8);

  if ((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  int on=1;
  if (setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, (const char *) &on, sizeof(on)) < 0) {
    perror("setsockopt");
    exit(1);
  }

  if (bind(soc, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("bind");
    exit(1);
  }

  if (listen(soc, 10) < 0) {
    perror("listen");
    exit(1);
  }

  while(1) {
    if ((soc_in = accept(soc, (struct sockaddr *)&client, &client_size)) < 0 ) {
      perror("accept");
      continue;
    }
    //printf("\nconnection!\n");
    /*while ((ret = read(soc_in, buffer, BSIZE-1)) > 0) {
      buffer[ret] = '\0';
      printf("%s", buffer);
      //fflush(stdout);
    }*/
    read(soc_in, buffer, RSIZE);
    if (sscanf(buffer, "GET %s HTTP/1.1\n%*s", file) == 0) {
      write (soc_in, f404, strlen(f404));
      close(soc_in);
      continue;
    }
    if (strcmp("/", file) == 0) {
      strcpy(file, "/test.html");
    }
    if ((fp = fopen(file+1, "r")) == NULL) {
      write (soc_in, f404, strlen(f404));
      close(soc_in);
      continue;
    }
    find = strchr(file, '.');
    if(strcmp(".js", find) == 0) {
      strcpy(find, ".javascript");
    }
    //printf("%s\n", buffer);
    //printf("%s\n", file);
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    sprintf(fbuffer, "HTTP/1.1 200 OK\nContent-Type: text/%s\nContent-Length: %d\n\n", find+1 ,fsize);
    write (soc_in, fbuffer, strlen(fbuffer));
    while (fgets(fbuffer, BSIZE, fp) != NULL) {
      write (soc_in, fbuffer, strlen(fbuffer));
    }
    close(soc_in);
  }

  return 0;
}
