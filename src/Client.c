#include <stdio.h>

#include "../include/ModbusAP.h"


int main(int argc, char *argv[])
{
    __uint16_t val[2] ={0, 0};
    int read, write;
    
    read = read_h_regs( "127.0.0.1", 0, 7);

    if( read < 0){
        perror( "[Client]:Read failed" );
        return -1;
    }

    __uint16_t soma = val[0] + val[1];
    write = write_multiple_regs( "127.0.0.1", 1, 1, &soma );

    if( write < 0){
        perror( "[Client]:Write failed" );
        return -1;
    }

    return 0;
}