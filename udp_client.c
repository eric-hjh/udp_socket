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
#define BUFSIZE 10


void error_handling(char *message) {
    perror(message);
    exit(1);
}

int main (int argc, char **argv) {
  int sock;
  int str_len;
  char message[BUFSIZE];

  
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
        fgets(message, sizeof(message), stdin);

        if (strcmp(message, "q\n") == 0)
            break;
        if(strlen(message) < 5){
          str_len = sendto(sock, message, strlen(message), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        } else{
          printf("메시지가 서버 버퍼 크기(5)를 초과했습니다. 현재 메시지 (%lu). 다시 입력하세요.\n", strlen(message));
        }
        
    }

  close(sock);
  return 0;

}
