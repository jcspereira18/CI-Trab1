
#include "../include/ModbusTCP.h"
#include "../include/ModbusAP.h"

#define perror(...)
#define printf(...)


int read_h_regs( int sockfd, __uint16_t start_addr, __uint16_t n_reg, __uint16_t *Read_reg )
{

    __uint8_t APDU[MAX_APDU];
    __uint16_t APDU_len = 5; // 1 byte fc + 2 bytes star_addr + 2 bytes n_reg

    //Check the parameters
    if( sockfd < 0 ){
        perror( "[READ]:Invalid socket" );
        return -1;
    }

    if( n_reg < MIN_QUANTITY_REG || n_reg > MAX_RD_REG ){
        perror( "[READ]:Invalid number of registers" );
        return -1;
    }

    if( start_addr < MIN_START_ADDR || start_addr > MAX_START_ADDR ){
        perror( "[READ]:Invalid starting address" );
        return -1;
    }

    //Build APDU Header
    APDU[0] = (__uint8_t) FC_READ; //Function Code - 3
    APDU[1] = (__uint8_t) (start_addr >> 8); // High byte Start_Addr
    APDU[2] = (__uint8_t) (start_addr & 0xFF); // Low byte Start_Addr
    APDU[3] = (__uint8_t) (n_reg >> 8); // High byte quantity_Reg
    APDU[4] = (__uint8_t) (n_reg & 0x7D); // Low byte quantity_Reg

    printf("\n[READ]: APDU to be sent: \n ");
    for( int i = 0; i < APDU_len; i++ )
        printf( "%.2x ", APDU[i] );
    printf("\n");

    int send_reg = send_modbus_request( sockfd, APDU, &APDU_len, APDU );

    if( send_reg < 0){
        perror( "[READ]:Send modbus request failed" );
        return -1;
    }

    //Check the error code
    if( APDU[0] & 0x80 ){
        printf( "[READ]: Error code received %d\n", APDU[1] );
        return APDU[1];
    }

    for( int i = 0; i < n_reg; i++ )
        Read_reg[i] = (APDU[2*i + 2] << 8) + APDU[2*i + 3];
    
    return 0;
}

int write_multiple_regs( int sockfd, __uint16_t start_addr, __uint16_t n_reg, __uint16_t *val )
{
    __uint8_t APDU[MAX_APDU];
    __uint16_t APDU_len = 0;

    //Check the parameters
    if(sockfd < 0){
        perror( "[WRITE]:Invalid socket" );
        return -1;
    }

    if( n_reg < MIN_QUANTITY_REG || n_reg > MAX_WR_REG ){
        perror( "[WRITE]:Invalid number of registers" );
        return -1;
    }   
    if( start_addr < MIN_START_ADDR || start_addr > MAX_START_ADDR ){
        perror("[WRITE]:Invalid starting address" );
        return -1;
    }


    //Build APDU Header
    APDU[0] = (__uint8_t) FC_WRITE; //Function Code 
    APDU[1] = (__uint8_t) (start_addr >> 8); // High byte Start_Addr
    APDU[2] = (__uint8_t) (start_addr & 0xFF); // Low byte Start_Addr
    APDU[3] = (__uint8_t) (n_reg >> 8); // High byte quantity_Reg
    APDU[4] = (__uint8_t) (n_reg & 0x7B); // Low byte quantity_Reg
    APDU[5] = (__uint8_t) (2 *n_reg); // Byte Count
 
    //Values to be written int the register
    for (int i = 0; i < n_reg; i++) {
        APDU[6 + 2 * i] = (__uint8_t) (val[i] >> 8); // High byte val[i]
        APDU[7 + 2 * i] = (__uint8_t) (val[i] & 0xFF); // Low byte val[i]
    }

    // 6 + 2 * quantity_reg
    APDU_len = 6 + 2 * n_reg;

    printf("\n[WRITE]: APDU to be sent: \n");
    for( int i = 0; i < APDU_len; i++ )
        printf( "%.2x ", APDU[i] );
    printf("\n");
    
    int send_reg = send_modbus_request( sockfd, APDU, &APDU_len, APDU );

    if( send_reg < 0){
        perror( "[WRITE]:Send modbus request failed" );
        return -1;
    }

    //Check the error code
    if( APDU[0] & 0x80 ){
        printf( "[WRITE]: Error code received %d\n", APDU[1] );
        return APDU[1];
    }

    return 0;

}