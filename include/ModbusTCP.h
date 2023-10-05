
#ifndef MODBUS_TCP
#define MODBUS_TCP

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#define PORT 502
#define MBAP_LEN 7

int send_modbus_request( char *server_add, __uint8_t *APDU, __uint16_t APDUlen, __uint8_t *APDU_R );

#endif