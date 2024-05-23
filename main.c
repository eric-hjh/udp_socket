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
#define CLI_BUFSIZE 10


void error_handling(char *message) {
    perror(message);
    exit(1);
}

int client(int sock, char *message, struct sockaddr_in serv_addr) {
    while (1) {
        printf("서버로 전송 (종료 q) : ");
        fgets(message, CLI_BUFSIZE, stdin);

        if (strcmp(message, "q\n") == 0)
            break;
        if (strlen(message) < 5) {
            sendto(sock, message, strlen(message), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        } else {
            printf("메시지가 서버 버퍼 크기(5)를 초과했습니다. 현재 메시지 (%lu). 다시 입력하세요.\n", strlen(message));
        }
    }

    close(sock);
    return 0;
}

int server(int sock, struct sockaddr_in serv_addr, struct sockaddr_in clnt_addr, char *message) {
    int serv_bind;
    int str_len, num = 0;

    socklen_t clnt_addr_size;

    serv_bind = bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (serv_bind == -1)
        error_handling("bind() error");

    printf("waiting for messages\n");

    sleep(1);

    while (1) {
        printf("-----[%d]번째 통신-----\n", num);
        clnt_addr_size = sizeof(clnt_addr);
        sleep(1);
        str_len = recvfrom(sock, message, BUFSIZE, 0, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (str_len == -1) {
            perror("recvfrom() error ");
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                printf("[EAGAIN] or [EWOULDBLOCK] \n");
            } else if (errno == EINTR) {
                printf("[EINTR]\n");
            } else if (errno == ENOTCONN) {
                printf("[ENOTCONN]\n");
            } else if (errno == ENOTSOCK) {
                printf("[ENOTSOCK]\n");
            } else if (errno == ETIMEDOUT) {
                printf("[ETIMEDOUT]\n");
            } else if (errno == EIO) {
                printf("[EIO]\n");
            } else if (errno == ENOBUFS) {
                printf("[ENOBUFS]\n");
            } else if (errno == ENOMEM) {
                printf("[ENOMEM]\n");
            }
            continue;
        }
        message[str_len - 1] = '\0';
        printf("received %d bytes data : %s\n", str_len, message);

        num++;
    }

    close(sock);
    return 0;
}

int main(int argc, char **argv) {
    int sock;
    char message[BUFSIZE];
    char cli_message[CLI_BUFSIZE];

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;

    if (argc != 3 && argc != 2) {
        printf("Usage:\n");
        printf("For client: %s <IP> <port>\n", argv[0]);
        printf("For server: %s <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error_handling("UDP 소켓 생성 오류");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (argc == 3) {
        serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
        serv_addr.sin_port = htons(atoi(argv[2]));
        return client(sock, message, serv_addr);
    } else if (argc == 2) {
        return server(sock, serv_addr, clnt_addr, message);
    }

    return 0;
}

