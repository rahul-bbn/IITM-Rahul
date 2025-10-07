#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/uds-demo.sock"
#define BUF_SIZE 1024

static void die(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

static ssize_t read_line(int fd, char *buf, size_t maxlen)
{
    size_t n = 0;
    while (n + 1 < maxlen)
    {
        char c;
        ssize_t r = read(fd, &c, 1);
        if (r == 0)
            break;
        if (r < 0)
        {
            if (errno == EINTR)
                continue;
            return -1;
        }
        buf[n++] = c;
        if (c == '\n')
            break;
    }
    buf[n] = '\0';
    return (ssize_t)n;
}

static int write_all(int fd, const void *buf, size_t len)
{
    const char *p = buf;
    while (len > 0)
    {
        ssize_t w = write(fd, p, len);
        if (w < 0)
        {
            if (errno == EINTR)
                continue;
            return -1;
        }
        p += w;
        len -= (size_t)w;
    }
    return 0;
}

int main(void)
{
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1)
        die("socket");

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        die("connect");

    char buf[BUF_SIZE];

    // read greeting
    if (read_line(sock, buf, sizeof(buf)) > 0)
    {
        printf("%s", buf);
    }

    //interactive prompt
    while (1)
    {
        printf("kv> ");
        fflush(stdout);

        if (!fgets(buf, sizeof(buf), stdin))
            break;

        //ensure newline at end
        if (write_all(sock, buf, strlen(buf)) < 0)
            break;

        // if user typed EXIT, stop after sending
        if (strncmp(buf, "EXIT", 4) == 0)
            break;

        // read one-line reply from server
        if (read_line(sock, buf, sizeof(buf)) <= 0)
            break;
        printf("%s", buf);
    }

    close(sock);
    return 0;
}
