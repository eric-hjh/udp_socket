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
  int sock;
  char message[BUFSIZE]; 
  int str_len;
  int i = 0;
  socklen_t addr_size;
  
  char MSG1[] = "Good" ;
  char MSG2[] = "Evening";
  char MSG3[] = "Everybody!";

  struct sockaddr_in serv_addr; 
  struct sockaddr_in from_addr;

  if(argc!=3){
    printf ("Usage: %s <IP><port>\n",argv[0]);
    exit(1);
  }

  sock = socket (AF_INET, SOCK_DGRAM, 0) ;
  if (sock == -1)
    error_handling("UDP 소켓 생성 오류");
  
  memset(&serv_addr, 0 , sizeof(serv_addr)) ;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi (argv[2]));
  
  while (1)
  {
    sendto(sock, MSG1, strlen(MSG1), 0, (struct sockaddr*)&serv_addr, sizeof (serv_addr)) ;

    addr_size = sizeof (from_addr) ;
    str_len = recvfrom(sock, message, BUFSIZE, 0, (struct sockaddr*)&from_addr, &addr_size);
    message[str_len]=0;
    printf("서버로부터 수신된 %d차 메시지: %s \n",i,message);
    i++;
  }
  return 0;

}
