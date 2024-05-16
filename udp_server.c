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
#include <errno.h>

#define BUFSIZE 5
void error_handling(char *message) {
    perror(message);
    exit(1);
}


int main (int argc, char **argv) {
  int serv_sock;
  char message[BUFSIZE]; 
  int str_len, num = 0;
  int serv_bind;

  struct sockaddr_in serv_addr; 
  struct sockaddr_in clnt_addr; 
  socklen_t clnt_addr_size;

  
  if(argc!=2){
    printf("Usage: %s <port>\n", argv[0]);
    exit (1);
  }

  serv_sock = socket(AF_INET, SOCK_DGRAM, 0); 
  if(serv_sock == -1) 
    error_handling("socket() error");

  memset(&serv_addr, 0, sizeof (serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
  serv_addr.sin_port = htons(atoi(argv[1])); 

  

  serv_bind = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if(serv_bind == -1)
    error_handling("bind() error");

  printf("waiting for messages\n");

  sleep (1) ;

  while (1){
    printf("-----[%d]번째 통신-----", num);
    clnt_addr_size = sizeof(clnt_addr);
    sleep (1);
    str_len = recvfrom(serv_sock, message, BUFSIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size) ;
    if(str_len == -1){ 
      perror("recvfrom() error ");
       if (errno == EAGAIN || errno == EWOULDBLOCK) {
               printf("[EAGAIN] or [EWOULDBLOCK] \n");
            } else if (errno == EINTR) {
                printf("[EINTR]\n");
            } else if(errno == ENOTCONN){
                printf("[ENOTCONN]\n");
            } else if(errno == ENOTSOCK){
                printf("[ENOTSOCK]\n");
            } else if(errno == ETIMEDOUT){
                printf("[ETIMEDOUT]\n");
            } else if(errno == EIO){
                printf("[EIO]\n");
            } else if(errno == ENOBUFS){
                printf("[ENOBUFS]\n");
            } else if(errno == ENOMEM){
                printf("[ENOMEM]\n");
            }
            continue;
    }
    message[str_len] = '\0';

    printf("\nsocket : %d serv_bind : %d\n", serv_sock, serv_bind);
    printf("received %d bytes data : %s\n",str_len, message);
 
    num++;
  }
  close(serv_sock);
  return 0;
}

