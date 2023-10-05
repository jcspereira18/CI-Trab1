
#include"../include/ModbusTCP.h"

int connect_socket(char *server_add)
{
    struct sockaddr_in server_addr;
    
    //bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); 
    inet_aton(server_add, &server_addr.sin_addr);

    //Create the socket
    int sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( sockfd < 0 ) {
        perror("[TCP]: Creating socket failed");
        return -1;
    }
    printf("[TCP]: Socket created\n");
   
    // Connect to the server
    if ( connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 ) {
        perror("[TCP]: Connection failed");
        return -1;
    }
    printf("[TCP]: Connected to the server\n");

    return sockfd;
}

int close_socket( int sockfd ){
    return close( sockfd );
}
    
int send_modbus_request( char *server_add, __uint8_t *APDU, __uint16_t APDUlen, __uint8_t *APDU_R)
{
    int sockfd = connect_socket(server_add);
    
    static int trans_id = 0;
    __uint8_t MBAP[MBAP_LEN];
    __uint8_t MBAP_R[MBAP_LEN];


    //Build MBAP Header 
    trans_id ++;
    MBAP[0] = (__uint8_t) trans_id >> 8; //Transaction Identifier
    MBAP[1] = (__uint8_t) trans_id & 0xFF; //Transaction Identifier
    MBAP[2] = (__uint8_t) 0x00; //Protocol Identifier
    MBAP[3] = (__uint8_t) 0x00; //Protocol Identifier
    MBAP[4] = (__uint8_t) (APDUlen + 1) >> 8; //Length
    MBAP[5] = (__uint8_t) (APDUlen + 1) & 0xFF; //Length
    MBAP[6] = (__uint8_t) 0x01; //Unit Identifier

    //Build PDU
    int PDUlen = APDUlen + MBAP_LEN;
    __uint8_t PDU[PDUlen];

    for( int i = 0; i < MBAP_LEN; i++ )
        PDU[i] = MBAP[i];
    
    for( int i = 0; i < APDUlen; i++ )
        PDU[i + MBAP_LEN] = APDU[i];
    

    //Send PDU
    int send_PDU = write( sockfd, PDU, PDUlen );
    if( send_PDU < 0 ){
        perror( "[TCP]: Send PDU failed" );
        close_socket( sockfd );
        return -1;
    }

    printf( "[TCP]: PDU sent: \n" );
    for( int i = 0; i < PDUlen; i++ )
        printf( "%.2x ", PDU[i] );


    //Read MBAP
    int read_MBAP = read( sockfd, MBAP_R, MBAP_LEN );
    if( read_MBAP < 0 ){
        perror( "[TCP]: Read MBAP failed" );
        close_socket( sockfd );
        return -1;
    }

    printf( "\n[TCP]: MBAP received: \n" );
    for( int i = 0; i < MBAP_LEN; i++ )
        printf( "%.2x ", MBAP_R[i] );

    __uint16_t APDU_R_len = (MBAP_R[4] << 8) + MBAP_R[5] - 1;
    printf( "\n[TCP]: n_APDU: %d\n", APDU_R_len );

    __uint8_t PDU_R[APDU_R_len];
    
    // Read PDU
    int read_PDU = read( sockfd, PDU_R, APDU_R_len);
    if( read_PDU < 0 ){
        perror( "[TCP]: Read PDU failed: timeout" );
        close_socket( sockfd );
        return -1;
    }

    printf( "[TCP]: PDU received: \n" );
    for( int i = 0; i < APDU_R_len; i++ ) 
        printf( "%.2x ", PDU[i] );
    printf("\n");
        
    //Copy APDU_R from PDU
    for( int i = 0; i < APDU_R_len; i++ )
        APDU_R[i] = PDU_R[i];
 
    close_socket( sockfd );
    return 0;
    
}

