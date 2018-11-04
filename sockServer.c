//IPC - socket 1 com 1 do tipo -- cliente/servidor

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>   //tratamento de erros

//headers para socket zzzz
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORTA 5000
#define LEN 4096

struct sockaddr_in local;			//estrutura para o servidor
struct sockaddr_in remoto;		//estrutura para o cliente, guardar o ip

int main(){
	int sockfd;			//descritor socket para escrevermos/lermos os dados no socket
	int cliente;
	int slen;
	int len = sizeof(remoto);
	char buffer[4096];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);	// socket (familia, tipo (TCP), protocolo)

	if(sockfd == -1){		//se nao criado nenhum socket, fecha
		perror("socket");
		exit(1);
	}else
		printf("Socket criado com sucesso!\n");

	local.sin_family 			=AF_INET;	//local tem a msm familia do socket
	local.sin_port				=htons(PORTA);		//a porta deve-se converter para endereço de rede utilizando a funcao htons
	memset(local.sin_zero, 0x0, 8);			//setar o sin_zero para zero

	if(bind(sockfd,(struct sockaddr*)&local,sizeof(local)) == -1){		//bind() une um nome ao socket que vc abriu -- bind(socket, estrutura de conexao, tam da estrutura)
		perror("bind");
		exit(1);
	}

	listen(sockfd,1);		//listen() faz com que o socket aguarde por conexoes -- listen(socket, num max de conexoes)

	if( (cliente = accept(sockfd,(struct sockaddr*)&remoto, &len)) == -1){ // descritor do cliente e canal para se comunicar com cliente
		perror("accept");
		exit(1);
	}

	strcpy(buffer,"Welcome!\n\0");

	if(send(cliente,buffer,strlen(buffer), 0)){		//send() usada para enviar uma msg para um socket -- send(socket, *msg, tam da msg, param adicionais)
		printf("Aquardando resposta do cliente ...\n");
		while(1){
			memset(buffer,0x0,LEN);
			if((slen = recv(cliente,buffer,LEN,0)) > 0){  //recv() msms parametros da send(), mas usada para receber uma msg
				buffer[slen-1] = '\0';	//limpar o buffer, caractere nulo no final
				printf("Mensagem recebida: %s\n",buffer);
				close(cliente);
				break;
			}
		}
	}

	close(sockfd);
	printf("Servidor Encerrado!\n");

return 0;

}
