//IPC - pipe 1 para 1

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
 pid_t pid;   //variavel para armazenar pid
 int fd[2];  //Cria um vetor para receber a entrada e saida do pipe, file descriptors

 if(pipe(fd) < 0){    //cria o pipe
   perror("pipe");    //funcao pipe retorna -1 em caso de erro
   return -1;
 }

 if((pid = fork()) < 0){  //cria o processo filho usando a funcao fork
   perror("fork");
   return -1;
 }

 if(pid > 0){  //processo pai
   close(fd[0]);      //fecha a leitura do pipe, ja que sera usada a escrita fd[1]
   char msg[200]="HELLO!!!";
   printf("Enviada pelo pai no pipe: %s\n",msg);

   write(fd[1],msg,sizeof(msg)+1);  //escreve a mensagem no pipe, funcao write recebe o file descriptors, um ponteiro e o num de bytes +1 por causa do \0

   exit(0);
   }
   else{    //processo filho
     char msg_recebida[200];
     close(fd[1]);    //fecha a escrita do pipe, ja que sera usada a leitura fd[0]

     read(fd[0],msg_recebida,sizeof(msg_recebida));   //leitura do que foi escrito no pipe

     printf("sringlida pelo filho no pipe: %s\n",msg_recebida);

     exit(0);
     }
return 0;

}
