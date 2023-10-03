#include <stdio.h>

#include "../include/ModbusTCP.h"
#include "../include/ModbusAP.h"


int read_h_regs( char *server_add, __uint16_t start_addr, __uint16_t n_reg )
{

    __uint16_t APDU[MAX_APDU];
    int APDU_len = 5; // 1 byte fc + 2 bytes star_addr + 2 bytes n_reg

    // Check the parameters
    if( n_reg < MIN_QUANTITY_REG || n_reg > MAX_RD_REG ){
        perror("Invalid number of registers\n");
        return -1;
    }

    if( start_addr < MIN_START_ADDR || start_addr > MAX_START_ADDR ){
        perror("Invalid starting address\n");
        return -1;
    }

    //Build APDU Header
    APDU[0] = (__uint8_t) FC_READ; //Function Code - 3
    APDU[1] = (__uint8_t) start_addr >> 8; // High byte Start_Addr
    APDU[2] = (__uint8_t) start_addr & 0xFF; // Low byte Start_Addr
    APDU[3] = (__uint8_t) n_reg >> 8; // High byte quantity_Reg
    APDU[4] = (__uint8_t) n_reg & 0x7D; // Low byte quantity_Reg

    int send_reg = send_modbus_request( server_add, APDU, APDU_len, APDU );

    if( send_reg < 0){
        perror( "Send modbus request failed" );
        return -1;
    }
    
    return send_reg;
}

int write_multiple_regs( char *server_add, __uint16_t start_addr, __uint16_t n_reg, __uint16_t *val )
{
    __uint16_t APDU[MAX_APDU];
    int APDU_len = 0;

    // Check the parameters
    if( n_reg < MIN_QUANTITY_REG || n_reg > MAX_WR_REG ){
        perror( "Invalid number of registers\n" );
        return -1;
    }

    if( start_addr < MIN_START_ADDR || start_addr > MAX_START_ADDR ){
        perror("Invalid starting address\n");
        return -1;
    }


    //Build APDU Header
    APDU[0] = (__uint8_t) 0x10; //Function Code - 16
    APDU[1] = (__uint8_t) start_addr >> 8; // High byte Start_Addr
    APDU[2] = (__uint8_t) start_addr & 0xFF; // Low byte Start_Addr
    APDU[3] = (__uint8_t) n_reg >> 8; // High byte quantity_Reg
    APDU[4] = (__uint8_t) n_reg & 0x7B; // Low byte quantity_Reg
    APDU[5] = (__uint8_t)  2 *n_reg; // Byte Count
 
    //Values to be written int the register
    for (int i = 0; i < n_reg; i++) {
        APDU[6 + 2 * i] = (__uint8_t) val[i] >> 8; // High byte val[i]
        APDU[7 + 2 * i] = (__uint8_t) val[i] & 0xFF; // Low byte val[i]
    }

    // 6 + 2 * quantity_reg
    APDU_len = 6 + 2 * n_reg;
    
    int send_reg = send_modbus_request( server_add, APDU, APDU_len, APDU );

    if( send_reg < 0){
        perror( "Send modbus request failed" );
        return -1;
    }

    return send_reg;

}