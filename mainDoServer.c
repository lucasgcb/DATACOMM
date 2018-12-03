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
#include <fcntl.h>
#include "aesArduino.h"
#include <curl/curl.h>
#include "sender.h"
#define PORTA 8888
//#define TAMANHO_PAYLOAD 16


int main(int argc, char *argv[]) {
     int sockfd, newsockfd, portno = PORTA, clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;

	    pacoteFrame dado;
	    pacoteFrame dadoConfig;
	    pacoteFrame dadoSensor;
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
    memset(&dadoSensor, 0, sizeof(dadoSensor));
    memset(&dadoConfig, 0, sizeof(dadoConfig));
     while ( 1 ) 
	{

	memset(&dado, 0, sizeof(dado));
        printf( "waiting for new client...\n" );
        if ( ( newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, (socklen_t*) &clilen) ) < 0 )
            error2( (char *)("ERROR on accept") );

        printf( "opened new communication with client\n" );
             //---- wait for a number from client ---
             data =  read(newsockfd, &dado, sizeof(dado));
	
             printf( "got %d\n", data );
	     printf("Resposta: ID: %d / COMMAND: %c / SIZE: %d / Payload: %s \n", dado.id,dado.command,dado.size,dado.payload);

//	send2ThingSpeak(descriptografarPacote(dado.payload,TAMANHO_PAYLOAD));
	send2Display(descriptografarPacote(dado.payload,TAMANHO_PAYLOAD));
 	if(dado.command=='a')
        {
	        memset(&dadoConfig, 0, sizeof(dadoConfig));
            	dadoConfig=dado;
            	write(newsockfd, &dadoSensor, sizeof(dadoSensor));
         	 	     printf("\n *****\nDADO ENVIADO: ID: %d / COMMAND: %c / SIZE: %d / Payload: %s \n", dadoSensor.id,dadoSensor.command,dadoSensor.size,dadoSensor.payload);
            	printf("Atualizado para %s\n", dadoConfig.payload);
            	printf("uns short : %d\n", sizeof(unsigned short));
                close( newsockfd );

        }
	if(dado.command=='s')
	{
                memset(&dadoSensor, 0, sizeof(dadoSensor));		
		char kk = 'k';
      	    	write(newsockfd, &kk, 1);
                close( newsockfd );

	}
	
             if ( data < 0 ) 
                printf("no response");
                
             

             //--- send new data back --- 
             printf( "sending back %d\n", dado );
//	     write( newsockfd, &dado, sizeof(dado) );
	     sleep(3);

        }

     return 0; 
}
