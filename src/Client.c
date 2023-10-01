
#include <stdio.h>

#include "ModbusAP.h"

int main(int argc, char *argv[])
{
    __uint16_t val[2];
    
    read = read_h_regs( " ", 0, 0, 0, val );

    if( read == 0x83 ){
        perror( "Read failed" );
        return -1;
    }

    write = write_multiple_regs( " ", 0, 0, 0, val );

    if( write == 0x90 ){
        perror( "Write failed" );
        return -1;
    }

    
}