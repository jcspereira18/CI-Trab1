
#ifndef MODBUS_APP
#define MODBUS_APP

#include<unistd.h>

#define MAX_APDU 256

#define MIN_QUANTITY_REG 1 //Min numbers of registers
#define MAX_WR_REG 123 //Max numbers of registers to write
#define MAX_RD_REG 125 //Max number of registers to read

#define MIN_START_ADDR 0x0000 //Min starting address
#define MAX_START_ADDR 0xFFFF //Max starting address

#define FC_READ 0x03 //Function Code - 3
#define FC_WRITE 0x10 //Function Code - 16

int read_h_regs( int sockfd, __uint16_t start_addr, __uint16_t n_reg, __uint16_t *Read_reg);

int write_multiple_regs( int sockfd, __uint16_t start_addr, __uint16_t n_reg, __uint16_t *val );

#endif