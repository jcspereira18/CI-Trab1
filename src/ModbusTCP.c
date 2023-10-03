
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

#include"../include/ModbusTCP.h"

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
    
int send_modbus_request( char *server_add, __uint8_t *APDU, __uint16_t APDUlen, __uint8_t *APDU_R)
{
    int sockfd = create_socket();
    
    static int trans_id = 0;
    u_int16_t MBAP[MBAP_LEN];
    int APDU_R_len = 0;

    //PDU = APDU(SDU) + MBAP;    
    //write (sockfd, PDU, PDUlen) // sends Modbus TCP PDU
    //read ( sockfd, PDU_R, PDU_Rlen)

    //Build MBAP Header 
    trans_id ++;
    MBAP[0] = (__uint8_t) trans_id >> 8; //Transaction Identifier
    MBAP[1] = (__uint8_t) trans_id & 0xFF; //Transaction Identifier
    MBAP[2] = (__uint8_t) 0x00; //Protocol Identifier
    MBAP[3] = (__uint8_t) 0x00; //Protocol Identifier
    MBAP[4] = (__uint8_t) APDUlen >> 8; //Length
    MBAP[5] = (__uint8_t) APDUlen & 0xFF; //Length
    MBAP[6] = (__uint8_t) 0x01; //Unit Identifier

    //Build PDU
    int PDUlen = APDUlen + MBAP_LEN;
    __uint8_t PDU[PDUlen];

    for( int i = 0; i < MBAP_LEN; i++ ){
        PDU[i] = MBAP[i];
    }
    for( int i = 0; i < APDUlen; i++ ){
        PDU[i + MBAP_LEN] = APDU[i];
    }

    //Send PDU
    int send_PDU = write( sockfd, PDU, PDUlen );
    if( send_PDU < 0 ){
        perror( "Send PDU failed" );
        close_socket( sockfd );
        return -1;
    }
    
    // Read PDU
    int read_PDU = read( sockfd, PDU, PDUlen );
    
    if (read_PDU > 0 && APDU_R != NULL) {
        for( int i = 0; i < APDUlen; i++ ){
            APDU_R[i] = PDU[i + MBAP_LEN];
        }
       APDU_R_len = read_PDU - MBAP_LEN;
    }

    close_socket( sockfd );

    if( read_PDU < 0 ){
        perror( "Read PDU failed: timeout" );
        return -1;
    }

    if (APDU_R == NULL)
        return -1;
    else 
        return APDU_R_len;
    
}