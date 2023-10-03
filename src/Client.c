
#include <stdio.h>

#include "../include/ModbusAP.h"

int main(int argc, char *argv[])
{
    __uint16_t val[2];
    int read, write;
    
    read = read_h_regs( "127.0.0.1 ", 0, 2 );

    if( read == 0x83 || read < 0){
        perror( "Read failed" );
        return -1;
    }

    __uint16_t soma = val[0] + val[1];
    write = write_multiple_regs( "127.0.0.1", 1, 1, &soma );

    if( write == 0x90 || write < 0){
        perror( "Write failed" );
        return -1;
    }

    return 0;
}