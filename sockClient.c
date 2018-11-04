//IPC - socket 1 com 1 do tipo -- cliente/servidor

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//headers para socket
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORTA 5000
#define LEN 4096

struct sockaddr_in remoto;

int main(){
	int sockfd;
	int slen;
	int len = sizeof(remoto);
	char buffer[4096];

 	printf("Eu sou o cliente!!\n");
	sockfd = socket(AF_INET, SOCK_STREAM,0);		//criar o socket

	if(sockfd == -1){
		perror("socket");
		exit(1);
	}else
		printf("Socket criado com sucesso!\n");

	remoto.sin_family 				=AF_INET;
	remoto.sin_port						=htons(PORTA);
	remoto.sin_addr.s_addr    =inet_addr("127.0.0.1");
	memset(remoto.sin_zero, 0x0, 8);


	if( connect(sockfd,(struct sockaddr*)&remoto, len) == -1){
		perror("connect");
		exit(1);
	}

	while(1){

		if((slen = recv(sockfd,buffer,LEN,0))>0){		//reber a msg, olhar em sockServer.c
			buffer[slen-1] = '\0';
			printf("Mensagem recebida: %s\n",buffer);
		}
		memset(buffer,0x0,LEN);
		fgets(buffer,LEN,stdin);
		send(sockfd,buffer,strlen(buffer), 0);		//enviar a msg, olhar em sockServer.c

	}

	close(sockfd);
	printf("Cliente Encerrado!\n");

return 0;

}
