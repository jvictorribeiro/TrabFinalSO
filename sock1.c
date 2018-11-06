//IPC socket 10 com 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   //fork...

//headers para socket
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORTA 5000

int main(){
	pid_t pid;

	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}

	if(pid > 0){
    int sockfd, new_socket, valorLido;
    struct sockaddr_in endereco;
    int opt = 1;    //valor de opt
    int addrlen = sizeof(endereco);    //tam do endereço
    char buffer[1024] = {0};
    char *msg = "Mensagem enviada pelo servidor";

   if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0){   //criando o descriptor do socket
       perror("socket falhou");
       exit(1);
   }

   if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){    //opcional, mas ajuda a manipular as opcoes para o socket referido
       perror("setsockopt");
       exit(EXIT_FAILURE);
   }

   endereco.sin_family = AF_INET;
   endereco.sin_addr.s_addr = INADDR_ANY;
   endereco.sin_port = htons(PORTA);

   if(bind(sockfd, (struct sockaddr *)&endereco, sizeof(endereco)) < 0){  //liga o socket a porta 5000
       perror("bind falhou");
       exit(1);
   }

	 for(int i = 0; i < 10; i++){		//for para os 10 clientes, não precisa de semaforo, pois para no listen
   	if(listen(sockfd, 1) < 0){   //aguarda conexoes do cliente
       	perror("listen");
       	exit(1);
   	}

   	if((new_socket = accept(sockfd, (struct sockaddr *)&endereco,(socklen_t*)&addrlen)) < 0){
       	perror("accept");
       	exit(1);
   	}

   	printf("Servidor: ");
   	valorLido = read(new_socket , buffer, 1024);   //le a msg
   	printf("%s %d\n",buffer, i);
   	send(new_socket , msg , strlen(msg) , 0 );   //envia a msg
	}

 }else if(pid == 0){
		pid_t childpid;
		for(int i = 0; i < 10; i++){		// cria os dez filhos, clientes
			if(( childpid = fork()) < 0){
				perror("fork");
				exit(1);
			}
			if(childpid == 0){
   			struct sockaddr_in endereco;
   			int clientSock = 0, valorLido;
   			struct sockaddr_in enderecoServer;
   			char *msg = "Mensagem enviada pelo cliente";
   			char buffer[1024] = {0};

   			if((clientSock = socket(AF_INET, SOCK_STREAM, 0)) < 0){    //ciente cria o socket igual ao servidor
       		printf("\n erro na criacao do socket \n");
       		return -1;
   			}

   			memset(&enderecoServer, '0', sizeof(enderecoServer));

   			enderecoServer.sin_family = AF_INET;
   			enderecoServer.sin_port = htons(PORTA);

   			if(inet_pton(AF_INET, "127.0.0.1", &enderecoServer.sin_addr) <= 0){    //inet pton converte enderecos para forma binaria
       		printf("\nEndereco invalido \n");
       		return -1;
   			}

   			if(connect(clientSock, (struct sockaddr *)&enderecoServer, sizeof(enderecoServer)) < 0){ //conecta o socket referenciado, ao endereco especificado
       		printf("\nConexao falhou \n");
       		return -1;
   			}

   			send(clientSock , msg , strlen(msg) , 0);    //msg do enviada pelo cliente
   			printf("Cliente %d: ", i);
   			valorLido = read( clientSock , buffer, 1024);    //msg lida por ele
   			printf("%s\n",buffer);
	 			exit(0);
		}
 	}
}

return 0;

}
