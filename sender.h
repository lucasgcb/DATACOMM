#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#define API_KEY "DP33DR83R67WG8FE"
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
   // error2( (char *)( "ERROR writing to socket") );
	return;
  buffer[n] = '\0';
}

int getData( int sockfd ) {
  char buffer[32];
  int n;
  if ( (n = read(sockfd,buffer,31) ) < 0 )
//    error2( (char *)( "ERROR reading from socket") );
	return -1;
  buffer[n] = '\0';
  return atoi( buffer );
}


int send2Display(int payload)
{
    pacoteFrame sensor;
	sensor.id=0;
	sensor.command='s';
	sensor.size=sizeof(sensor);
	sprintf(sensor.payload, "%d", payload);
	printf("payload: %d", payload);
    int sockfd, portno = 10050, n;
    char serverIp[] = "172.16.153.1";
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    int data;

      // error( const_cast<char *>( "usage myClient2 hostname port\n" ) );
      printf( "contacting %s on port %d\n", serverIp, portno );
      // exit(0);
 
    if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 )
	return -1;
        //error( const_cast<char *>( "ERROR opening socket") );

    if ( ( server = gethostbyname( serverIp ) ) == NULL ) 
	return -1;
        //error( const_cast<char *>("ERROR, no such host\n") );
    
    bzero( (char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if ( connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
	return -1;
       // error( const_cast<char *>( "ERROR connecting") );

	send(sockfd, &sensor,sizeof(sensor),0);
//    sendData( sockfd, 's');
  //    sendData( sockfd, payload );
      data = getData( sockfd );
      printf("%d ->  %d\n",n, payload );
    
//    sendData( sockfd, -2 );

    close( sockfd );
    return 0;
}

int send2ThingSpeak(int payload )
{
  CURL *curl;
  CURLcode res;

         printf("Attempting to curl");
         char field1[TAMANHO_PAYLOAD+8];
         char field2[TAMANHO_PAYLOAD+8];
         char field3[TAMANHO_PAYLOAD+8];
        int fieldCount=1;
        sprintf(field1,"&field%d=%ld", fieldCount++, payload);
        sprintf(field2,"&field%d=%ld", fieldCount++, payload);
        sprintf(field3,"&field%d=%ld", fieldCount++, payload);
        printf("\n %s \n",field1);
  curl = curl_easy_init();
  if(curl) {
         printf("Curling");
    char destinationUrl[255];
    strcat(destinationUrl,"https://api.thingspeak.com/update?api_key=");
    strcat(destinationUrl,API_KEY);
    strcat(destinationUrl,field1);
/*    strcat(destinationUrl,field2);
    strcat(destinationUrl,field3);*/
        printf("%s\n",destinationUrl);
    curl_easy_setopt(curl, CURLOPT_URL, destinationUrl);
    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
           curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
         else printf("Curl failed!");
	return 0;

}

