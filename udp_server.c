#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 30
void error_handling(char *message) {
    perror(message);
    exit(1);
}


int main (int argc, char **argv) {
  int serv_sock;
  char message[BUFSIZE]; 
  int str_len, num = 0;

  struct sockaddr_in serv_addr; 
  struct sockaddr_in clnt_addr; 
  socklen_t clnt_addr_size;

  if(argc!=2){
    printf("Usage: %s <port>\n", argv[0]);
    exit (1);
  }

  serv_sock = socket (PF_INET, SOCK_DGRAM, 0) ;

  if(serv_sock == -1)
    error_handling("socket() error");

  memset(&serv_addr, 0, sizeof (serv_addr)) ;
  serv_addr.sin_family = AF_INET;
  serv_addr. sin_addr.s_addr = htonl(INADDR_ANY) ;
  serv_addr.sin_port = htons(atoi(argv [1])) ;

  
  if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof (serv_addr)) == -1)
    error_handling("bind() error");

  sleep (5) ;

  while (1){
    clnt_addr_size = sizeof(clnt_addr);
    sleep (1) ;
    str_len = recvfrom(serv_sock, message, BUFSIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size) ;

    printf("수신 번호: %d 1n", num++) ;
    sendto (serv_sock, message, str_len, 0,
      (struct sockaddr*)&clnt_addr, sizeof (clnt_addr)) ;
  }
  return 0;
}