#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 * 실습 9. 파이프 기반 양방향 텍스트 채팅 프로그램
 */

int main() {
    int p2c[2], c2p[2];
    char send_buf[128], recv_buf[128];
    pid_t pid;

    pipe(p2c);
    pipe(c2p);

    pid = fork();

    if (pid == 0) {
        close(p2c[1]);
        close(c2p[0]);

        while (1) {
            read(p2c[0], recv_buf, sizeof(recv_buf));
            printf("[Child] Received: %s\n", recv_buf);

            sprintf(send_buf, "Child echo => %s", recv_buf);
            write(c2p[1], send_buf, strlen(send_buf) + 1);
        }
    } else {
        close(p2c[0]);
        close(c2p[1]);

        while (1) {
            printf("Parent> ");
            fgets(send_buf, sizeof(send_buf), stdin);

            write(p2c[1], send_buf, strlen(send_buf) + 1);
            read(c2p[0], recv_buf, sizeof(recv_buf));

            printf("[Parent] Received: %s\n", recv_buf);
        }
    }
    return 0;
}
