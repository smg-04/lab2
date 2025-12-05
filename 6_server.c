// chat_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 12345
#define MAX_CLIENTS  FD_SETSIZE
#define BUF_SIZE 1024

int main() {
    int listenfd, connfd, maxfd, nready;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;
    int client[MAX_CLIENTS];
    fd_set allset, rset;
    char buf[BUF_SIZE];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1; setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET; servaddr.sin_addr.s_addr = htonl(INADDR_ANY); servaddr.sin_port = htons(PORT);
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, 10);

    for (int i=0;i<MAX_CLIENTS;i++) client[i] = -1;
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    maxfd = listenfd;
    printf("chat server listening on %d\n", PORT);

    while (1) {
        rset = allset;
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        if (FD_ISSET(listenfd, &rset)) {
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
            // add to client array
            int i;
            for (i=0;i<MAX_CLIENTS;i++) if (client[i] < 0) { client[i] = connfd; break; }
            if (i==MAX_CLIENTS) { close(connfd); }
            FD_SET(connfd, &allset);
            if (connfd > maxfd) maxfd = connfd;
            if (--nready <= 0) continue;
        }
        for (int i=0;i<MAX_CLIENTS;i++) {
            int sock = client[i];
            if (sock < 0) continue;
            if (FD_ISSET(sock, &rset)) {
                ssize_t n = read(sock, buf, sizeof(buf));
                if (n <= 0) {
                    // closed
                    close(sock);
                    FD_CLR(sock, &allset);
                    client[i] = -1;
                } else {
                    // broadcast to others
                    for (int j=0;j<MAX_CLIENTS;j++) {
                        if (client[j] >=0 && client[j] != sock) {
                            write(client[j], buf, n);
                        }
                    }
                }
                if (--nready <= 0) break;
            }
        }
    }
    return 0;
}
