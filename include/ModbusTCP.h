
#ifndef MODBUS_TCP
#define MODBUS_TCP

#include<unistd.h>


int send_modbus_request( char *server_add, __uint8_t *APDU, __uint16_t *APDUlen, __uint8_t *APDU_R );