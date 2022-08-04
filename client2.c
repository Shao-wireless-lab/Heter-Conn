#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>          
#include <arpa/inet.h>
#include <netdb.h>
#define BACKLOG 5
#define LENGTH 1024

int main(){
  char *ip = "10.0.2.17";
  int port = 6000;
  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;
  char buffer[1024];
  char revbuf[LENGTH];
  int n;

  server_sock = socket(AF_INET, SOCK_STREAM, 0);
if (server_sock < 0){
    perror("[-]Socket error");
    exit(1);
  }
  printf("[+]TCP socket created.\n");

  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);
  setsockopt(server_sock , SOL_SOCKET , SO_BINDTODEVICE , "enp0s8" , strlen("enp0s8")); //    setting the NIC data should be sent through
  n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
if (n < 0){
    perror("[Client]Bind error");
    exit(1);
  }
  printf("[Client]Bind to the port number: %d\n", port);

  listen(server_sock, 5);
  printf("[Client]Listening...\n");

while(1){
  addr_size = sizeof(client_addr);
  client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
  printf("[Client]Server connected.\n");

  //Sending file
  char* fs_name = "/home/zkelley/Downloads/file.txt"; //Choosing file to be sent
  char sdbuf[LENGTH]; // Send buffer
  printf("[Client] Sending %s to the Server...", fs_name);
  FILE *fs = fopen(fs_name, "r");
if(fs == NULL) {
  fprintf(stderr, "ERROR: File %s not found on server. (errno = %d)\n", fs_name, errno);
  exit(1);
}
  bzero(sdbuf, LENGTH); 
  int fs_block_sz; 
//using send command to send file
while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs))>0) {
if(send(client_sock, sdbuf, fs_block_sz, 0) < 0) {
  fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", fs_name, errno);
  exit(1);
}
  bzero(sdbuf, LENGTH);
}
  printf("Ok sent to server!\n");
  while(waitpid(-1, NULL, WNOHANG) > 0);
  printf("[Client]Server disconnected.\n\n");
  close(client_sock);
}
  return 0;
}
