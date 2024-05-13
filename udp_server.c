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

#define BUFSIZE 5
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

  // 실행할 때 입력값 검사
  if(argc!=2){
    printf("Usage: %s <port>\n", argv[0]);
    exit (1);
  }

  serv_sock = socket(AF_INET, SOCK_DGRAM, 0); // 성공하면 파일 디스크립터, 실패하면 -1 
  if(serv_sock == -1) 
    error_handling("socket() error");

  memset(&serv_addr, 0, sizeof (serv_addr)); // 메모리 블록을 특정 값으로 설정하는 함수, 쓰레기값이 남아있을 수 있으니 초기화를 해야한다

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
  serv_addr.sin_port = htons(atoi(argv[1])); // atoi : 문자열 -> 정수

  
  if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))
    error_handling("bind() error");

  printf("waiting for messages\n");

  sleep (5) ;

  while (1){
    clnt_addr_size = sizeof(clnt_addr);
    sleep (1);
    str_len = recvfrom(serv_sock, message, BUFSIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size) ;
    if(str_len < 0){ 
      error_handling("recvfrom() error "); 
    }
    message[str_len] = '\0';

    printf("ip : %s\n", inet_ntoa(clnt_addr.sin_addr));
    printf("received %d bytes data : %s\n\n",str_len, message);
 
    sendto(serv_sock, message, strlen(message), 0, (struct sockaddr*)&clnt_addr, sizeof (clnt_addr)) ;
  }
  close(serv_sock);
  return 0;
}
