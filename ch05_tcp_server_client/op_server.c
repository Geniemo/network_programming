#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE    1024

void error_handling(char* message);

int calculate(int op_cnt, int operands[], char op);

int main(int argc, char* argv[]) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_sz;

    char msg[BUF_SIZE];
    int recv_len;
    int res;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    
    clnt_addr_sz = sizeof(clnt_addr);
    for (int i = 0; i < 5; i++) {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
        if (clnt_sock == -1)
            error_handling("accept() error");
        else
            printf("Connected client %d\n", i + 1);
        
        read(clnt_sock, msg, sizeof(int)); // 피연산자 수 read
        recv_len = 0;
        while ((*((int*)msg) * sizeof(int)) + 1 > recv_len) { // 피연산자와 연산자 read
            recv_len += read(clnt_sock, msg + sizeof(int) + recv_len, BUF_SIZE - sizeof(int));
        }
        
        res = calculate(*((int*)msg), (int*)(msg + sizeof(int)), msg[sizeof(int) + (*((int*)msg)) * sizeof(int)]);
        write(clnt_sock, &res, sizeof(res));

        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}

void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int calculate(int op_cnt, int operands[], char op) {
    int result = operands[0];
    switch (op) {
    case '+':
        for (int i = 1; i < op_cnt; i++) {
            result += operands[i];
        }
        break;
    case '-':
        for (int i = 1; i < op_cnt; i++) {
            result -= operands[i];
        }
        break;
    case '*':
        for (int i = 1; i < op_cnt; i++) {
            result *= operands[i];
        }
        break;
    case '/':
        for (int i = 1; i < op_cnt; i++) {
            result /= operands[i];
        }
        break;
    }
    return result;
}