// mini_httpd.c (간단 구현)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fcntl.h>

#define PORT 8080
#define BUF 4096

// helper: send simple response
void send_response(int fd, const char *status, const char *content_type, const char *body) {
    char header[BUF];
    int len = snprintf(header, sizeof(header),
        "HTTP/1.0 %s\r\nContent-Length: %ld\r\nContent-Type: %s\r\n\r\n",
        status, strlen(body), content_type);
    write(fd, header, len);
    write(fd, body, strlen(body));
}

void handle_client(int cfd) {
    char buf[BUF];
    int n = read(cfd, buf, sizeof(buf)-1);
    if (n <= 0) { close(cfd); return; }
    buf[n]=0;
    // parse request line
    char method[16], path[1024], protocol[32];
    sscanf(buf, "%15s %1023s %31s", method, path, protocol);
    // find headers
    char *cl = strstr(buf, "Content-Length:");
    int content_len = 0;
    if (cl) sscanf(cl, "Content-Length: %d", &content_len);
    // simple GET
    if (strcmp(method, "GET") == 0) {
        if (strncmp(path, "/cgi-bin/", 9) == 0) {
            // run CGI program (no query parsing for simplicity)
            pid_t pid = fork();
            if (pid == 0) {
                // child: exec cgi script
                dup2(cfd, STDOUT_FILENO);
                setenv("REQUEST_METHOD", "GET", 1);
                execl(path+1, path+1, NULL); // path+1 to skip leading '/'
                perror("execl");
                exit(1);
            } else {
                waitpid(pid, NULL, 0);
            }
        } else {
            // serve simple page
            const char *body = "<html><body><h1>MiniHTTPD GET</h1></body></html>";
            send_response(cfd, "200 OK", "text/html", body);
        }
    } else if (strcmp(method, "POST") == 0) {
        // locate body start
        char *body = strstr(buf, "\r\n\r\n");
        if (body) body += 4;
        else body = buf + n;
        // if content not fully read, read remaining
        int received = n - (body - buf);
        while (received < content_len) {
            int r = read(cfd, buf, sizeof(buf));
            if (r<=0) break;
            received += r;
        }
        // simple echo
        send_response(cfd, "200 OK", "text/plain", "POST received\n");
    } else {
        send_response(cfd, "405 Method Not Allowed", "text/plain", "Method Not Allowed\n");
    }
    close(cfd);
}

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    int opt=1; setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in addr = { .sin_family=AF_INET, .sin_addr.s_addr=INADDR_ANY, .sin_port=htons(PORT) };
    bind(s, (struct sockaddr*)&addr, sizeof(addr));
    listen(s, 10);
    printf("mini httpd listening on %d\n", PORT);
    while (1) {
        struct sockaddr_in cli; socklen_t l = sizeof(cli);
        int c = accept(s, (struct sockaddr*)&cli, &l);
        pid_t pid = fork();
        if (pid == 0) {
            close(s);
            handle_client(c);
            exit(0);
        }
        close(c);
        waitpid(-1, NULL, WNOHANG); // reap zombies opportunistically
    }
    close(s);
    return 0;
}
