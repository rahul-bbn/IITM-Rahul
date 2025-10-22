#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/uds_kv_demo.sock"
#define BUF_SIZE 512

void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(void) {
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1) die("socket");

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        die("connect");

    char buf[BUF_SIZE];
    if (read(sock, buf, sizeof(buf)) > 0)
        printf("%s", buf);

    while (1) {
        printf("kv> ");
        fflush(stdout);
        if (!fgets(buf, sizeof(buf), stdin))
            break;

        write(sock, buf, strlen(buf));

        if (strncmp(buf, "EXIT", 4) == 0)
            break;

        ssize_t n = read(sock, buf, sizeof(buf) - 1);
        if (n <= 0) break;
        buf[n] = '\0';
        printf("%s", buf);
    }

    close(sock);
    return 0;
}
