#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

/*
 * 실습 10. 메시지 큐 기반 텍스트 채팅
 */

struct msgbuf {
    long mtype;
    char mtext[128];
};

int main() {
    key_t key = ftok(".", 'M');
    int qid = msgget(key, IPC_CREAT | 0666);
    pid_t pid = fork();

    if (pid == 0) {
        struct msgbuf msg;
        while (1) {
            msgrcv(qid, &msg, sizeof(msg.mtext), 1, 0);
            printf("[Child] %s\n", msg.mtext);

            msg.mtype = 2;
            sprintf(msg.mtext, "Child echo => %s", msg.mtext);
            msgsnd(qid, &msg, strlen(msg.mtext) + 1, 0);
        }
    } else {
        struct msgbuf msg;
        while (1) {
            printf("Parent> ");
            fgets(msg.mtext, sizeof(msg.mtext), stdin);

            msg.mtype = 1;
            msgsnd(qid, &msg, strlen(msg.mtext) + 1, 0);

            msgrcv(qid, &msg, sizeof(msg.mtext), 2, 0);
            printf("[Parent] %s\n", msg.mtext);
        }
    }
    return 0;
}
