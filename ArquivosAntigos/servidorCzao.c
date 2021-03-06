#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#define PORTA 8888
#define TAMANHO_PAYLOAD 16
typedef struct frame
	{
        unsigned short id;
        unsigned char command;
        unsigned char size;
        unsigned char payload[TAMANHO_PAYLOAD];
	}pacoteFrame;


void error2( char *msg ) {
  perror(  msg );
  exit(1);
}

int func( int a ) {
   return 2 * a;
}

void sendData( int sockfd, int x ) {
  int n;

  char buffer[32];
  sprintf( buffer, "%d\n", x );
  if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 )
    error2( (char *)( "ERROR writing to socket") );
  buffer[n] = '\0';
}

int getData( int sockfd ) {
  char buffer[32];
  int n;
  if ( (n = read(sockfd,buffer,31) ) < 0 )
    error2( (char *)( "ERROR reading from socket") );
  buffer[n] = '\0';
  return atoi( buffer );
}

int main(int argc, char *argv[]) {
     int sockfd, newsockfd, portno = PORTA, clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;

	    pacoteFrame dado;
	    pacoteFrame dadoConfig;
     int n;
     int data;

     printf( "using port #%d\n", portno );
    
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
         error2( (char *)("ERROR opening socket") );
     bzero((char *) &serv_addr, sizeof(serv_addr));

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons( portno );
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
       error2( (char *)( "ERROR on binding" ) );
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
  
     //--- infinite wait on a connection ---
        
    memset(&dadoConfig, 0, sizeof(dadoConfig));
	
     while ( 1 ) {

	memset(&dado, 0, sizeof(dado));
        printf( "waiting for new client...\n" );
        if ( ( newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, (socklen_t*) &clilen) ) < 0 )
            error2( (char *)("ERROR on accept") );
        printf( "opened new communication with client\n" );
        while ( 1 ) {
             //---- wait for a number from client ---
             data =  read(newsockfd, &dado, sizeof(dado));
             printf( "got %d\n", data );
	     printf("Resposta: ID: %d / COMMAND: %c / SIZE: %d / Payload: %s \n", dado.id,dado.command,dado.size,dado.payload);
             if ( data < 0 ) 
                break;
                
             data = func( data );

             //--- send new data back --- 
             printf( "sending back %d\n", data );
             sendData( newsockfd, data );
	     sleep(1);
        }
        close( newsockfd );

        //--- if -2 sent by client, we can quit ---
        if ( data == -2 )
          break;
     }
     return 0; 
}
