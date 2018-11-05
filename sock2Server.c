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

	int sockfd, ret;
	 struct sockaddr_in local;

	int newSocket;
	struct sockaddr_in remoto;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);		//cria novo socket()
	if(sockfd < 0){
		printf("Erro na conexao.\n");
		exit(1);
	}
	printf("Servidor Socket criado.\n");

	memset(&local, '\0', sizeof(local));			//server e cliente tem a msm porta, familia...
	local.sin_family = AF_INET;
	local.sin_port = htons(PORT);
	local.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&local, sizeof(local));			//bind() associa o socket a um endereço socket e uma porta
	if(ret < 0){
		printf("Erro no binding.\n");
		exit(1);
	}
	printf("Bind para a porta %d\n", 4444);

	if(listen(sockfd, 10) == 0){			//listen() para aguardar conexao
		printf("Aguardando conexao....\n");
	}else{
		printf("Erro no binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&remoto, &addr_size);			//usa a funcao accept() para comecar a comunicacao
		if(newSocket < 0){
			exit(1);
		}
		printf("Conexao aceita de %s:%d\n", inet_ntoa(remoto.sin_addr), ntohs(remoto.sin_port));		//ip e a porta

		if((childpid = fork()) == 0){			//se cria processo filho, fecha o socket
			close(sockfd);

			while(1){
				recv(newSocket, buffer, 1024, 0);			//recebe msg atraves da funcao recv()
				if(strcmp(buffer, ":exit") == 0){
					printf("Desconectado de %s:%d\n", inet_ntoa(remoto.sin_addr), ntohs(remoto.sin_port));
					break;
				}else{
					printf("Client:%d: %s\n",ntohs(remoto.sin_port),buffer);
					send(newSocket, buffer, strlen(buffer), 0);			//envia msg atrasves da funcao send()
					bzero(buffer, sizeof(buffer));
				}
			}
		}

	}

	close(newSocket);

	return 0;
}
