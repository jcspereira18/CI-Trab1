
#include "../include/ModbusTCP.h"
#include "../include/ModbusAP.h"

#define IP_add "127.0.0.1"
#define IP_host "10.227.113.1"
#define PORT 502


int main(int argc, char *argv[])
{
  int read, write; 
  __uint8_t APDU[MAX_APDU];
  __uint16_t reg = 0x41; 
  __uint16_t A[4];
  __uint16_t B = 0, C = 0;
  __uint16_t D[3] = {1,2,3}; 

  int sockfd = connect_socket( IP_add, PORT);

  write = write_multiple_regs( sockfd, 115, 3, D );
  if( write != 0){
      printf( "Result = %d\n", write );
      return -1;
  }


  //1
  write = write_multiple_regs( sockfd, 120, 1, &reg );
  if( write != 0){
      printf( "Result = %d\n", write );
      return -1;
  }

  //2
  read = read_h_regs( sockfd, 121, 4, A);
  if( read != 0){
     printf( "Result = %d\n", read );
     return -1;
  }
  for( int i = 0; i < 4; i++)
      printf("A[%d] = %d\n", i, A[i]) ;

  //3
  read = read_h_regs( sockfd, 125, 1, &B);
  if( read != 0){
     printf( "Result = %d\n", read );
     return -1;
  } 
  printf("B = %d\n", B); 
  
  //4
  if(B != 0)
    C = A[0]+A[3];
  else
    C = 9999;
  printf("C = %d\n", C);

  write = write_multiple_regs( sockfd, 126, 1, &C );
  if( write != 0){
      printf( "Result = %d\n", write );
      return -1;
  }
  close_socket( sockfd );

  //sockfd = connect_socket( IP_host );
//
  ////5
  //write = write_multiple_regs( sockfd, 127, 1, &C );
  //if( write != 0){
  //    printf( "Result = %d\n", write );
  //    return -1;
  //}
//
  //close_socket( sockfd );
  

  printf("Result = 0\n");
  return 0;

}