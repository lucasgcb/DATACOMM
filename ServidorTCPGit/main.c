#include <winsock.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define PORTA 10050
#define TAMANHO_PAYLOAD 16
int main(int argc, char *argv[])
{
	WSADATA wsInformacao;
	SOCKET socketServidor,socketCliente;
	struct sockaddr_in service, cliente;
	int iResult,tamamnhoEndereco,bytes;
	char bufferEntrada[101],bufferSaida[101];
	typedef struct frame
	{
        unsigned short id;
        unsigned char command;
        unsigned char size;
        unsigned char payload[TAMANHO_PAYLOAD];
	}pacoteFrame;

    typedef struct estruturaHttp
	{
	    unsigned char httpHeader[146];
        unsigned char msg[108];
	}Http;
    pacoteFrame dado;
    pacoteFrame dadoConfig;
    Http GETZao ;
    int i;
	// inicializa Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsInformacao);
	if (iResult != NO_ERROR) {
		printf("Erro na chamada da funcao WSAStartup().\n");
	}


    socketServidor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketServidor == INVALID_SOCKET) {
		printf("Erro na chamada da funcao socket(): %ld.\n", WSAGetLastError());
		WSACleanup();
		return 0;
	}


	// asocia a uma porta o servidor


	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("172.16.153.1");
	service.sin_port = htons(PORTA);

	if (bind(socketServidor, (SOCKADDR *) &service, sizeof(service)) == SOCKET_ERROR ) {
		printf("Erro na chamada da funcao bind().\n");
		closesocket(socketServidor);
	    return 0;
	}

	// configura a fila de conexões
	if (listen(socketServidor, 1) == SOCKET_ERROR) {
		printf("Erro na chamada da funcao listen().\n");
	}



	tamamnhoEndereco = sizeof(cliente);
	printf("Aguardando...\n");

    memset(&dadoConfig, 0, sizeof(dadoConfig));
    memset(&dado, 0, sizeof(dado));
    int dadoSensor = 0;
	while (1) {
        long c=10050;

        memset(&GETZao, 0, sizeof(GETZao));
        socketCliente = SOCKET_ERROR;
		if ((socketCliente = accept(socketServidor, NULL, NULL)) == SOCKET_ERROR) {
			continue;
		}
        char command;
		bytes = recv(socketCliente, &dado, sizeof(dado), 0);
		printf("Resposta: ID: %d / COMMAND: %c / SIZE: %d / Payload: %s \n", dado.id,dado.command,dado.size,dado.payload);
		if(dado.command=='s')
            dadoSensor = atoi(dado.payload);

        printf("DadoSensor: %d",dadoSensor);
        bufferEntrada[bytes] = '\0';

        printf("SocketCliente: %d\n",socketCliente);

            unsigned char httpHeader[] = "HTTP/1.1 200 OK\r\nContent-Type:text/html\r\nConnection:Keep-Alive\r\nKeep-Alive:timeout=5,max=100\r\nRefresh:5\r\n\r\n";
            unsigned char temp[15];
            strcat(GETZao.httpHeader, httpHeader);
            //int random = rand()*10;
            sprintf(temp, "%d", dadoSensor);

            strcat(GETZao.msg, "<!DOCTYPE HTML><html>Dado no servidor meu chapa: ");
            strcat(GETZao.msg, temp);
            strcat(GETZao.msg, " </html>\r\n");


            send(socketCliente, &GETZao, sizeof(GETZao), 0);


      /*  send(socketCliente, dadoConfig.id, sizeof(dadoConfig.id), 0);
        send(socketCliente, dadoConfig.command, sizeof(dadoConfig.command), 0);
        send(socketCliente, dadoConfig.size, sizeof(dadoConfig.size), 0);
        for(i=0;i<TAMANHO_PAYLOAD;i++)
        {
          send(socket94Cliente, dadoConfig.payload[i], sizeof(dado.payload[0]), 0);
        }

      */

        Sleep(1000);
		closesocket(socketCliente);

	}

	return 0;
}
