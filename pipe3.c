//IPC - pipe 10 com 10

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <sys/shm.h>        /* shmat(), IPC_RMID        */
#include <errno.h>          /* errno, ECHILD            */

//headers para semaforo -- rodar com -lpthread
#include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>

int main(){
  sem_t *produtor;
  sem_t *consumidor;
	sem_t *sincro;   //semaforo usado para sincronizacao
  int i,j;
  pid_t pid;
  int fd[2];

  if(pipe(fd) < 0){    //cria o pipe
    perror("pipe");    //funcao pipe retorna -1 em caso de erro
    return -1;
  }

  produtor = sem_open ("pSem", O_CREAT | O_EXCL, 0644, 1);  //cria os semaforos, o ultimo argumento eh o valor que vai definir se espera
  consumidor = sem_open("qSem",O_CREAT | O_EXCL, 0322, 1);
  sincro= sem_open ("rSem", O_CREAT | O_EXCL, 0144, 0);

  for(i = 0; i < 20; ++i){    //cria 20 filhos
    pid= fork();
    if(pid == 0){ //se processo filho: sai do ciclo!
      break;
    }
  }

  if(pid > 0){  //processo pai
	  /* Esperando todos os filhos sairem*/
	  while(pid = waitpid(-1, NULL, 0)){	/* Espera pelo processo término do processo filho até que */
		    if(errno == ECHILD)			/* Nenhum processo filho exista mais, entao errno e atualizado com ECHILD */
				    break;
		}
    printf("Todos os filhos sairam\n");

    sem_unlink ("pSem");    //fecha os semaforos
    sem_close(produtor);
    sem_unlink ("qSem");
    sem_close(consumidor);
	  sem_unlink("rSem");
	  sem_close(sincro);
	  exit(0);
  }else{    //processo filho
    if(i < 10){
      close(fd[1]);    //fecha a escrita do pipe, ja que sera usada a leitura fd[0]
			sem_wait(consumidor);    //decrementa valor do semaforo, trava

     	int msg_recebida;

      read(fd[0], &msg_recebida, sizeof(msg_recebida));   //leitura do que foi escrito no pipe

      printf("valor lido pelo filho %d no pipe: %d\n",i ,msg_recebida);
		  sem_post(produtor);   //incrementa valor do semaforo, libera
      sem_post(sincro);     //incrementa semaforo para sincronizacao
      close(fd[0]);//fecha a leitura do pipe, ja que sera usada a escrita fd[1]
    }else{
      close(fd[0]);      //fecha a leitura do pipe, ja que sera usada a escrita fd[1]
	    sem_wait (produtor);
      int msg[1] = {10};
      printf("valor escrito pelo filho %d no pipe: %i\n",i,msg[0]);

      write(fd[1], msg, sizeof(msg));  //escreve a mensagem no pipe, funcao write recebe o file descriptors, um ponteiro e o num de bytes +1 por causa do \0
      sem_post(consumidor);
      sem_wait(sincro);

      //printf("acabo produtor\n");
      close(fd[1]);   //fecha a escrita do pipe
      exit(0);
    }
  }

exit(0);

return 0;

}
