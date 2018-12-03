#include <winsock.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define PORTA 8888
#define TAMANHO_PAYLOAD 16
typedef struct frame
{
    unsigned short id;
    unsigned char command;
    unsigned char size;
    unsigned char payload[TAMANHO_PAYLOAD];
}pacoteFrame;

int main(int argc, char *argv[])
{
    WSADATA wsInformacao;
    struct sockaddr_in servidor;
    int iResult,tamamnhoEndereco,bytes;
	SOCKET socketCliente;
	struct sockaddr_in cliente;
    struct recebe;
    char buff[TAMANHO_PAYLOAD];
    float a;
    int b;


	// inicializa Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsInformacao);
	if (iResult != NO_ERROR) {
		printf("Erro na chadama da funcao WSAStartup()\n");
	}

    socketCliente = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socketCliente == INVALID_SOCKET) {
		printf("Erro na chadama da funcao socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return 0;
	}


	// se conecta ao servidor
	tamamnhoEndereco = sizeof(servidor);
	servidor.sin_family = AF_INET;
	servidor.sin_addr.s_addr = inet_addr("172.16.157.33");
	servidor.sin_port = htons(PORTA);


    while(connect(socketCliente, (SOCKADDR*) &servidor, sizeof(cliente)) == SOCKET_ERROR) {
        printf("Erro na chadama da funcao connect().\n");
    }

    pacoteFrame dadoEnviar;
    pacoteFrame dado;
    dadoEnviar.id = 69;
    dadoEnviar.command='a';
    printf("Digite o novo delay:");
    fgets(dadoEnviar.payload, TAMANHO_PAYLOAD, stdin);
    dadoEnviar.size=sizeof(dadoEnviar);
    send(socketCliente, &dadoEnviar, sizeof(dadoEnviar), 0);
	bytes = recv(socketCliente, &dado, sizeof(dado), 0);
    printf("Resposta: ID: %d / COMMAND: %c / SIZE: %d / Payload: %s \n", dado.id,dado.command,dado.size,dado.payload);

  closesocket(socketCliente);
  WSACleanup();

  system("PAUSE");
  return 0;
}
