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

#define _CRT_SECURE_NO_WARNINGS 
#define BUFSIZE 30

void error_handling(char *message) {
    perror(message);
    exit(1);
}

int main (int argc, char **argv) {
  int sock;
  char message[BUFSIZE]; 
  int str_len;
  char msg[BUFSIZE];

  
  struct sockaddr_in serv_addr; 

  if(argc!=3){
    printf ("Usage: %s <IP><port>\n",argv[0]);
    exit(1);
  }

  sock = socket(AF_INET, SOCK_DGRAM, 0) ;
  if (sock == -1)
    error_handling("UDP 소켓 생성 오류");
  
  memset(&serv_addr, 0 , sizeof(serv_addr)) ;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi (argv[2]));


  while (1) {
        printf("서버로 전송 (종료 q) : ");
        fgets(msg, sizeof(msg), stdin);

        if (strcmp(msg, "q\n") == 0)
            break;

        str_len = sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        printf("send message byte : %d \n", str_len);
    }

  close(sock);
  return 0;

}
