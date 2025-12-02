#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv;

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&serv, sizeof(serv));

    printf("Connected to server.\n");

    char msg[256];
    while (1) {
        printf("Input: ");
        fgets(msg, 256, stdin);

        write(sock, msg, strlen(msg));

        memset(msg, 0, 256);
        read(sock, msg, 256);

        printf("Echo: %s\n", msg);
    }

    close(sock);
    return 0;
}
