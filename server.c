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

  char *ip = "10.0.2.15";
  int port = 10000;

  int sock;
  struct sockaddr_in addr;
  socklen_t addr_size;
  char buffer[LENGTH];
  char revbuf[LENGTH];
  int n;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    perror("[Server]Socket error");
    exit(1);
  }
  printf("[Server]TCP server socket created.\n");

  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = port;
  addr.sin_addr.s_addr = inet_addr(ip);

  connect(sock, (struct sockaddr*)&addr, sizeof(addr));
  printf("[Server]Connection Establish.\n");

  bzero(buffer, LENGTH);
  strcpy(buffer, "Welcome.");
  printf("Server: %s\n", buffer);
  send(sock, buffer, strlen(buffer), 0);

  bzero(buffer, LENGTH);
  recv(sock, buffer, sizeof(buffer), 0);
  printf("Client: %s\n", buffer); 

  //Sending file
  char* fs_name = "/home/zkelley/Downloads/key.txt"; //Choosing file to be sent
  char sdbuf[LENGTH]; // Send buffer
  printf("[Server] Sending %s to the Client...", fs_name);
  FILE *fs = fopen(fs_name, "r");
  if(fs == NULL) {
  fprintf(stderr, "ERROR: File %s not found on server. (errno = %d)\n", fs_name, errno);
  exit(1);
}
  bzero(sdbuf, LENGTH); 
  int fs_block_sz; 
  //using send command to send file
  while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs))>0) {
  if(send(sock, sdbuf, fs_block_sz, 0) < 0) {
  fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", fs_name, errno);
  exit(1);
}
  bzero(sdbuf, LENGTH);
}
  printf("[Server]Ok sent to client!\n");
  while(waitpid(-1, NULL, WNOHANG) > 0);


  close(sock);
  printf("[Server]Disconnected from the Client.\n");
  //Call Script to start new socket connection
  system("cd /home/zkelley/Downloads ; chmod +x newSock.sh ; ./newSock.sh");
  return 0;

}
