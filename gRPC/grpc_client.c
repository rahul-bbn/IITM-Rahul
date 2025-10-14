#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <grpc-c/grpc-c.h>
#include "calc.grpc-c.h"
#include "calc.pb-c.h"

int main()
{
    grpc_c_init(GRPC_THREADS_DEFAULT);

    grpc_c_client_t *client = grpc_c_client_create("localhost:8080", NULL);
    Calc__Calculator_Client *calculator = calc__calculator__client_new(client);

    char operation[10];
    double a, b;

    printf("Enter command (e.g., ADD 10 20): ");
    scanf("%s %lf %lf", operation, &a, &b);

    Calc__CalcRequest req = CALC__CALC_REQUEST__INIT;
    req.operation = operation;
    req.a = a;
    req.b = b;

    Calc__CalcResponse *resp = calc__calculator__compute(calculator, NULL, &req);

    printf("Server reply: %s (Result: %.2f)\n", resp->message, resp->result);

    calc__calc_response__free_unpacked(resp, NULL);
    grpc_c_client_destroy(client);
    grpc_c_shutdown();

    return 0;
}
