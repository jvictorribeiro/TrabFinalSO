//IPC - pipe 1 para 1

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>    //pid_t...
#include <unistd.h>       //fork...

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

 if(pid > 0){       //processo pai
   close(fd[0]);      //fecha a leitura do pipe, ja que sera usada a escrita fd[1]
   int msg[1] = {10};
   printf("Valor enviado pelo pai no pipe: %i\n",msg[0]);

   write(fd[1],msg,sizeof(msg));  //escreve a mensagem no pipe, funcao write recebe o file descriptors, um ponteiro e o num de bytes +1 por causa do \0
   close(fd[1]);    //fecha escrita
   exit(0);
 }else{    //processo filho
   int msg_recebida;
   close(fd[1]);    //fecha a escrita do pipe, ja que sera usada a leitura fd[0]

   read(fd[0],&msg_recebida,sizeof(msg_recebida));   //leitura do que foi escrito no pipe
   close(fd[0]);
   printf("Valor lido pelo filho no pipe: %d\n",msg_recebida);
   exit(0);
 }

return 0;

}
