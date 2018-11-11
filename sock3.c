//IPC socket 10 com 10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   //fork...
#include <sys/wait.h>
#include <errno.h>
//headers para socket
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//header para socklen_
#include <semaphore.h>
#include <fcntl.h>

#define PORTA 5000


int main(){
	pid_t pid;
	int *valor, i, j;
	sem_t *produtor;
  sem_t *organizador;
	sem_t *sincro;
	sem_t *sincro2;

	produtor = sem_open ("pSem", O_CREAT | O_EXCL, 0644, 1);
	organizador = sem_open("oSem",O_CREAT | O_EXCL, 0322, 0);
	//sincro = sem_open ("pSem", O_CREAT | O_EXCL, 0161, 1);
	//sincro2 = sem_open ("tSem", O_CREAT | O_EXCL, 1288, 0);


	for(i=0; i<20; i++){		/* Criando 20 processos filhos para se comunicarem entre si */
		if((pid=fork())<0){
			perror("fork\n");
			exit(1);
		}

		if(pid==0)
			break;
	}
	if(pid>0){
		while(pid=waitpid(-1, NULL, 0)){
			if(errno == ECHILD)
			break;
		}
		//fechar o semaforo
		sem_unlink ("oSem");
		sem_close(organizador);
		sem_unlink ("pSem");
		sem_close(produtor);
		//sem_unlink("sSem");
		//sem_close(sincro);
		//sem_unlink("tSem");
		//sem_close(sincro2);
	  exit(0);

	}else if(i<10){
		int sockfd, new_socket, valorLido;
    struct sockaddr_in endereco;
    int opt = 1;    //valor de opt
    int addrlen = sizeof(endereco);    //tam do endereço
    int buffer[1]={10};

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

		for(j = 0; j < 10; ++j){
   		if(listen(sockfd, 1) < 0){   //aguarda conexoes do cliente
      perror("listen");
      exit(1);
   		}
		}

   	if((new_socket = accept(sockfd, (struct sockaddr *)&endereco,(socklen_t*)&addrlen)) < 0){
      perror("accept");
      exit(1);
   	}
 		sem_wait(produtor);
 		printf("Servidor %d enviou: ",i);
 		send(new_socket ,(char*)buffer , 1*sizeof(int) , 0 );   //envia a msg
 		printf("%d\n",buffer[0] );

 		sem_wait(organizador);
 		sem_post(produtor);
	}else{
		if(i>10){
			struct sockaddr_in endereco;
  	 	int clientSock = 0, valorLido;
   		struct sockaddr_in enderecoServer;
  		int buffer[1] = {0};

   		if((clientSock = socket(AF_INET, SOCK_STREAM, 0)) < 0){    //ciente cria o socket igual ao servidor
      	printf("\n erro na criacao do socket \n");
      	return -1;
   		}

   		memset(&enderecoServer, '0', sizeof(enderecoServer));

   		enderecoServer.sin_family = AF_INET;
   		enderecoServer.sin_port = htons(PORTA);

   		if(inet_pton(AF_INET, "127.0.0.1", &enderecoServer.sin_addr) <= 0){    //inet pton converte enderecos para forma binaria
      	printf("\nEndereco invalido\n");
      	return -1;
   		}

   		if(connect(clientSock, (struct sockaddr *)&enderecoServer, sizeof(enderecoServer)) < 0){ //conecta o socket referenciado, ao endereco especificado
      	printf("\nConexao falhou \n");
      	return -1;
   		}
   		printf("Cliente %d recebeu: ",i);
   		recv( clientSock , buffer, 1*sizeof(int),0);    //msg lida por ele

   		printf("%d\n",buffer[0]);
   		//sem_post(produtor);

			sem_post(organizador);
		}
	}
	sem_unlink ("oSem");//fechar o semaforo
	sem_close(organizador);
	sem_unlink ("pSem");
	sem_close(produtor);

	return 0;
}
