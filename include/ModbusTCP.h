
#ifndef MODBUS_TCP
#define MODBUS_TCP

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>



#define UNIT_ID 51
#define MBAP_LEN 7


int connect_socket( char *server_add, int port );
int close_socket( int sockfd );

int send_modbus_request( int sockfd, __uint8_t *APDU, __uint16_t *APDUlen, __uint8_t *APDU_R );

#endif