#include "rpc_common.h"

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    rpc_request_t req;
    rpc_response_t res;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket");
        exit(1);
    }
    // allow address reuse
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Bind
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind");
        exit(1);
    }

    // Listen
    listen(server_fd, 3);
    printf("RPC Server running on port %d...\n", PORT);

    // Accept client
    client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    if (client_fd < 0)
    {
        perror("accept");
        exit(1);
    }
    printf("Client connected.\n");

    // Read request
    read(client_fd, &req, sizeof(req));

    // Process request
    res.success = 1;
    switch (req.cmd)
    {
    case CMD_ADD:
        res.result = req.a + req.b;
        break;
    case CMD_SUB:
        res.result = req.a - req.b;
        break;
    case CMD_MUL:
        res.result = req.a * req.b;
        break;
    case CMD_DIV:
        if (req.b == 0)
        {
            res.success = 0;
            strcpy(res.message, "Division by zero");
        }
        else
            res.result = req.a / req.b;
        break;
    default:
        res.success = 0;
        strcpy(res.message, "Unknown command");
    }

    // Send response
    write(client_fd, &res, sizeof(res));

    close(client_fd);
    close(server_fd);
    return 0;
}
