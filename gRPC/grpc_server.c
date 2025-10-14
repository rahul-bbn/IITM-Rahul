#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <grpc-c/grpc-c.h>
#include "calc.grpc-c.h"
#include "calc.pb-c.h"

// Compute handler
static void compute_handler(Calc__CalcRequest *request,
                            Calc__CalcResponse *response,
                            void *user_data)
{
    double result = 0.0;
    char msg[100] = "";

    if (strcmp(request->operation, "ADD") == 0)
        result = request->a + request->b;
    else if (strcmp(request->operation, "SUB") == 0)
        result = request->a - request->b;
    else if (strcmp(request->operation, "MUL") == 0)
        result = request->a * request->b;
    else if (strcmp(request->operation, "DIV") == 0)
    {
        if (request->b == 0)
            strcpy(msg, "Division by zero");
        else
            result = request->a / request->b;
    }
    else
        strcpy(msg, "Unknown operation");

    if (strlen(msg) == 0)
        sprintf(msg, "RESULT %.2f", result);

    response->message = strdup(msg);
    response->result = result;
}

int main()
{
    grpc_c_init(GRPC_THREADS_DEFAULT);

    grpc_c_server_t *server = grpc_c_server_create(NULL);

    // Register Calculator service
    Calc__Calculator_Service calculator_service;
    calc__calculator__service__init(&calculator_service);
    calculator_service.compute = compute_handler;

    grpc_c_server_add_service(server, &calculator_service);

    // Start server
    grpc_c_server_start(server, "0.0.0.0:8080", NULL);
    printf("gRPC Calculator Server running on port 8080...\n");

    grpc_c_server_wait(server);
    grpc_c_server_destroy(server);

    return 0;
}
