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

  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;
  char buffer[LENGTH];
  char revbuf[LENGTH];
  int n;

  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0){
    perror("[Client]Socket error");
    exit(1);
  }
  printf("[Client]TCP socket created.\n");

  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

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

    bzero(buffer, LENGTH);
    recv(client_sock, buffer, sizeof(buffer), 0);
    printf("Server: %s\n", buffer);

    bzero(buffer, LENGTH);
    strcpy(buffer, "Requesting Key From Server");
    printf("Server: %s\n", buffer);
    send(client_sock, buffer, strlen(buffer), 0); 

// Receive File from Server
printf("[Client] Receiveing file from Server and saving it as key2.txt...");
char* fr_name = "/home/zkelley/Downloads/key2.txt"; //choosing file to save data
FILE *fr = fopen(fr_name, "a"); //reading recieved file

//error recording recieving file
if(fr == NULL)
  printf("File %s Cannot be opened.\n", fr_name);
else {
  bzero(revbuf, LENGTH); 
  int fr_block_sz = 0;
//using recieve command to get file
while((fr_block_sz = recv(client_sock, revbuf, LENGTH, 0)) > 0){
  int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
if(write_sz < fr_block_sz) {
  error("File write failed.\n");
}
bzero(revbuf, LENGTH);
if (fr_block_sz == 0 || fr_block_sz != 512) {
  break;
}}
if(fr_block_sz < 0) {
if (errno == EAGAIN) {
  printf("recv() timed out.\n");
}
else {
  fprintf(stderr, "recv() failed due to errno = %d\n", errno);
}}
  printf("Ok received from server!\n");
  fclose(fr);
}

    close (client_sock);

    printf("[Client]Server disconnected.\n\n");
    //Call Script to start new socket connection
    system("cd /home/zkelley/Downloads ; chmod +x addkey.sh ; ./addkey.sh");
    //Call Script to start new socket connection
    system("cd /home/zkelley/Downloads ; chmod +x newSock.sh ; ./newSock.sh");
  }

  return 0;
}
