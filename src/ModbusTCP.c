
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

#include"ModbusTCP.h"

#define PORT 502


int create_socket()
{
    struct sockaddr_in server_addr;
    
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); 
    inet_aton("127.0.0.1", &server_addr.sin_addr);

    //Create the socket
    int sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( sockfd < 0 ) {
        perror("Socket failed");
        return -1;
    }
   
    // Connect to the server
    if ( connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 ) {
        perror("Connection failed");
        return -1;
    }

    return sockfd;
}

int close_socket( int sockfd ){
    return close( sockfd );
}
    
int send_Modbus_request( char *server_add, __uint8_t *APDU, __uint16_t *APDUlen, __uint8_t *APDU_R)
{
    int sockfd = create_socket();
    
    int trans_id = 0;
    
    //PDU = APDU(SDU) + MBAP;    
    //write (sockfd, PDU, PDUlen) // sends Modbus TCP PDU
    //read ( sockfd, PDU_R, PDU_Rlen)



    close_socket( sockfd );
    return 0;
}