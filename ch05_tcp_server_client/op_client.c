#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE        1024

void error_handling(char* message);

int main(int argc, char* argv[]) {
    int sock;
    struct sockaddr_in serv_adr;
    
    char msg[BUF_SIZE];
    int res;

    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error");
    else
        puts("connected...");
    
    printf("operand count: ");
    scanf("%d", (int*)msg);
    for (int i = 0; i < *((int*)msg); i++) {
        printf("operand %d: ", i + 1);
        scanf("%d", (int*)(msg + sizeof(int) * (i + 1)));
    }
    printf("operator: ");
    scanf(" %c", msg + sizeof(int) * (*((int*)msg) + 1));

    write(sock, msg, sizeof(int) * (*((int*)msg) + 1) + 1);
    
    if (read(sock, &res, sizeof(res)) == -1)
        error_handling("read() error");
    printf("result: %d\n", res);

    close(sock);
    return 0;
}

void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}