#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 5
#define CLI_BUFSIZE 10

void error_handling(char *message) {
    perror(message);
    exit(1);
}

int client(SOCKET sock, char *message, struct sockaddr_in serv_addr) {
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

    closesocket(sock);
    WSACleanup();
    return 0;
}

int server(SOCKET sock, struct sockaddr_in serv_addr, struct sockaddr_in clnt_addr, char *message) {
    int serv_bind;
    int str_len, num = 0;

    int clnt_addr_size;

    serv_bind = bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (serv_bind == SOCKET_ERROR)
        error_handling("bind() error");

    printf("waiting for messages\n");

    Sleep(1000);

    while (1) {
        printf("-----[%d]번째 통신-----\n", num);
        clnt_addr_size = sizeof(clnt_addr);
        Sleep(1000);
        str_len = recvfrom(sock, message, BUFSIZE, 0, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (str_len == SOCKET_ERROR) {
            printf("recvfrom() error: %d\n", WSAGetLastError());
            continue;
        }
        message[str_len - 1] = '\0';
        printf("received %d bytes data : %s\n", str_len, message);

        num++;
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}

int main(int argc, char **argv) {
    WSADATA wsaData;
    SOCKET sock;
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

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET)
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
