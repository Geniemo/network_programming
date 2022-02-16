#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void error_handling(char* message);

int main(int argc, char* argv[]) {
    int fd;
    char buf[] = "justweon\n";
    // 아무것도 저장되어있지 않은(O_TRUNC) 새로운 파일이 생성(O_CREAT)되어 쓰기만 가능(O_WRONLY)하게
    fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC);
    if (fd == -1)
        error_handling("open() error");
    
    printf("file descriptor: %d\n", fd);

    // fd에 저장된 파일 디스크립터에 해당하는 파일에 buf에 저장된 데이터 전송
    if (write(fd, buf, sizeof(buf)) == -1)
        error_handling("write() error");
    
    close(fd);
    return 0;
}

void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}