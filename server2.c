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

  int sock;
  struct sockaddr_in addr;
  socklen_t addr_size;
  char buffer[1024];
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
  setsockopt(sock , SOL_SOCKET , SO_BINDTODEVICE , "enp0s8" , strlen("enp0s8")); //setting the NIC data should be sent through
  connect(sock, (struct sockaddr*)&addr, sizeof(addr));
  printf("Connected to the server.\n");

// Receive File from Client
printf("[Server] Receiveing file from client and saving it as final.txt...");
char* fr_name = "/home/zkelley/Downloads/final.txt"; //choosing file to save data
FILE *fr = fopen(fr_name, "a"); //reading recieved file

//error recording recieving file
if(fr == NULL)
  printf("File %s Cannot be opened.\n", fr_name);
else {
  bzero(revbuf, LENGTH); 
  int fr_block_sz = 0;
//using recieve command to get file
while((fr_block_sz = recv(sock, revbuf, LENGTH, 0)) > 0){
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
  printf("[Client]Ok received from client!\n");
  fclose(fr);
}
  close(sock);
  printf("[Server]Disconnected from the Client.\n");

  return 0;

}
