#ifndef RPC_COMMON_H
#define RPC_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Define RPC command types
typedef enum
{
    CMD_ADD,
    CMD_SUB,
    CMD_MUL,
    CMD_DIV,
    CMD_UNKNOWN
} rpc_command_t;

// Request structure
typedef struct
{
    rpc_command_t cmd;
    double a;
    double b;
} rpc_request_t;

// Response structure
typedef struct
{
    int success;       // 1 for success, 0 for error
    double result;     // valid if success == 1
    char message[100]; // error or info
} rpc_response_t;

#endif
