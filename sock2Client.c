//IPC - socket 10 com 10  cliente/servidor

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444

int main(){

	int clientSocket, ret;
	struct sockaddr_in local;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);		//funcao socket() para criar o socket, ver sock1Server.c
	if(clientSocket < 0){
		printf("Erro na conexao.\n");
		exit(1);
	}
	printf("Client Socket criado\n");

	memset(&local, '\0', sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = htons(PORT);
	local.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&local, sizeof(local));	//se conecta ao servidor atraves de connect() e em casso de erro, sai
	if(ret < 0){
		printf("Erro na conexao.\n");
		exit(1);
	}
	printf("Conectado ao servidor.\n");

	while(1){
		printf("Client:\t");
		scanf("%s", &buffer[0]);
		send(clientSocket, buffer, strlen(buffer), 0);		//envia a msg atraves de send()

		if(strcmp(buffer, ":exit") == 0){				//checa se o cliente quer sair do server
			close(clientSocket);
			printf("Disconectado do server.\n");
			exit(1);
		}

		if(recv(clientSocket, buffer, 1024, 0) < 0){
			printf("Erro no recebimento de dados.\n");
		}
	}

	return 0;
}
