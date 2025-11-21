#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

/*
 * 실습 8. 프로세스 간 통신 함수 연습
 * pipe() + fork() + exec()
 */

int main() {
    int fd[2];
    pid_t pid;
    char msg[] = "Hello from parent";
    char buf[128];

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    else if (pid == 0) {
        close(fd[1]);
        read(fd[0], buf, sizeof(buf));
        printf("[Child] Received: %s\n", buf);

        printf("[Child] exec: ls -l\n");
        execl("/bin/ls", "ls", "-l", NULL);

        perror("execl");
        exit(1);
    }
    else {
        close(fd[0]);
        write(fd[1], msg, strlen(msg) + 1);
        close(fd[1]);

        wait(NULL);
        printf("[Parent] child finished.\n");
    }

    return 0;
}
