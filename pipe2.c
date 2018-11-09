//IPC - pipe 10 com 1]

#include <stdio.h>          /* printf()                 */
#include <stdlib.h>         /* exit(), malloc(), free() */
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <sys/shm.h>        /* shmat(), IPC_RMID        */
#include <errno.h>          /* errno, ECHILD            */
#include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
  sem_t *produtor;
  sem_t *organizador;
  int i,x;
  pid_t pid;
  int fd[2];

 if(pipe(fd) < 0){    //cria o pipe
   perror("pipe");    //funcao pipe retorna -1 em caso de erro
   return -1;
 }
 produtor = sem_open ("pSem", O_CREAT | O_EXCL, 0644, 1);
 organizador=sem_open("oSem",O_CREAT | O_EXCL, 0322, 0);//e zro pq o produtor vai esperar o consumidor

 for(i = 0; i < 10; ++i){
   pid= fork();
   if(pid == 0){ //se processo filho: sai do ciclo!
     break;
   }
 }

 if(pid == 0){  //processo filho
   sem_wait (produtor);
   close(fd[0]);      //fecha a leitura do pipe, ja que sera usada a escrita fd[1]
   int msg[1] = {10};
   printf("String enviada pelo pai no pipe: %i\n",msg[0]);

   write(fd[1],msg,sizeof(msg));  //escreve a mensagem no pipe, funcao write recebe o file descriptors, um ponteiro e o num de bytes +1 por causa do \0
   sem_wait(organizador);
   printf("acabo produtor\n");
   sem_post (produtor);           /* V operation */
   close(fd[1]);//fecha a escrita do pipe, ja que sera usada a leitura fd[0]
   exit(0);

 }else{    //processo pai
     for(x=0;x<10;x++){
     int msg_recebida;
     close(fd[1]);    //fecha a escrita do pipe, ja que sera usada a leitura fd[0]

     read(fd[0],&msg_recebida,sizeof(msg_recebida));   //leitura do que foi escrito no pipe

     printf("String lida pelo filho %d no pipe: %d\n",x,msg_recebida);
     sem_post(organizador);
   }
   close(fd[0]);//fecha a leitura do pipe, ja que sera usada a escrita fd[1]

   sem_unlink ("oSem");//fechar o semaforo
   sem_close(organizador);
   sem_unlink ("pSem");
   sem_close(produtor);

 }
 exit(0);
return 0;

}
