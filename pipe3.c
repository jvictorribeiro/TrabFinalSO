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
	sem_t *sincro;
  int i,x;
  pid_t pid;
  int fd[2];

 if(pipe(fd) < 0){    //cria o pipe
   perror("pipe");    //funcao pipe retorna -1 em caso de erro
   return -1;
 }
 produtor = sem_open ("pSem", O_CREAT | O_EXCL, 0644, 1);
 organizador=sem_open("oSem",O_CREAT | O_EXCL, 0322, 1);//e zro pq o produtor vai esperar o consumidor
 sincro= sem_open ("sSem", O_CREAT | O_EXCL, 0144, 0);

 for(i = 0; i < 20; ++i){
   pid= fork();
   if(pid == 0){ //se processo filho: sai do ciclo!
     break;
   }
 }

 if(pid > 0){  //processo filho
	 /* Esperando todos os filhos saire, afinal, o ultimo processo a sair sera um filho */
				while(pid=waitpid(-1, NULL, 0))	/* Espera pelo processo término do processo filho até que */
				{
					if(errno == ECHILD)			/* Nenhum processo filho exista mais, entao errno e atualizado com ECHILD */
						break;
				}
   printf("Todos os filhos sairam\n");
	 sem_unlink ("oSem");//fechar o semaforo
   sem_close(organizador);
   sem_unlink ("pSem");
   sem_close(produtor);
	 sem_unlink("sSem");
	 sem_close(sincro);
	 exit(0);

 }else{    //processo pai
	 if(i<10){

     for(x=0;x<10;x++){
		 	close(fd[1]);    //fecha a escrita do pipe, ja que sera usada a leitura fd[0]
			sem_wait(organizador);
			sem_post(sincro);
     	int msg_recebida;


     read(fd[0],&msg_recebida,sizeof(msg_recebida));   //leitura do que foi escrito no pipe

     printf("String lida pelo filho %d no pipe: %d\n",x,msg_recebida);
    sem_post(organizador);
   }
   close(fd[0]);//fecha a leitura do pipe, ja que sera usada a escrita fd[1]
 }else{
	 close(fd[0]);      //fecha a leitura do pipe, ja que sera usada a escrita fd[1]
	 sem_wait (produtor);
   int msg[1] = {10};
   printf("String enviada pelo processo %d no pipe: %i\n",i,msg[0]);
	 sem_wait(sincro);
   write(fd[1],msg,sizeof(msg));  //escreve a mensagem no pipe, funcao write recebe o file descriptors, um ponteiro e o num de bytes +1 por causa do \0
   sem_post(produtor);
   //printf("acabo produtor\n");           /* V operation */
   close(fd[1]);//fecha a escrita do pipe, ja que sera usada a leitura fd[0]
   exit(0);

 }


 }
 exit(0);
return 0;

}
