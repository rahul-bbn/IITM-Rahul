#include "rpc_common.h"

int main()
{
    int sock;
    struct sockaddr_in serv_addr;
    rpc_request_t req;
    rpc_response_t res;
    char cmd[10];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect");
        exit(1);
    }

    // Input from user
    printf("Enter command (ADD/SUB/MUL/DIV a b): ");
    scanf("%s %lf %lf", cmd, &req.a, &req.b);

    if (strcmp(cmd, "ADD") == 0)
        req.cmd = CMD_ADD;
    else if (strcmp(cmd, "SUB") == 0)
        req.cmd = CMD_SUB;
    else if (strcmp(cmd, "MUL") == 0)
        req.cmd = CMD_MUL;
    else if (strcmp(cmd, "DIV") == 0)
        req.cmd = CMD_DIV;
    else
        req.cmd = CMD_UNKNOWN;

    // Send request
    write(sock, &req, sizeof(req));

    // Receive response
    read(sock, &res, sizeof(res));

    // Display result
    if (res.success)
        printf("Result = %.2lf\n", res.result);
    else
        printf("Error: %s\n", res.message);

    close(sock);
    return 0;
}
