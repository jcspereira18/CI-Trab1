
#include "../include/ModbusTCP.h"
#include "../include/ModbusAP.h"

//#define perror(...)


#define IP_add "128.0.0.1"
#define WRITE_R 6

int main(int argc, char *argv[])
{
    int read, write;
    __uint16_t val[2] ={3, 2};
    __uint16_t soma = val[0] + val[1];
    __uint16_t x[10];
    
    //(void)signal(SIGALRM, alarme);

    read = read_h_regs( IP_add, 0, 5, x);

    if( read != 0){
        printf( "Result = %d\n", read );
        return -1;
    }


    write = write_multiple_regs( IP_add, WRITE_R - 1, 1, &soma );
    if( write != 0){
        printf( "Result = %d\n", write );
        return -1;
    }
    
    printf("Result = 0\n");
    return 0;
}