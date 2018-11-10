//IPC - pipe 10 com 1

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <sys/shm.h>        /* shmat(), IPC_RMID        */

//headers para semaforo -- rodar com -lpthread
#include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>

int main(){
  sem_t *produtor;
  sem_t *consumidor;
  int i,j;
  pid_t pid;
  int fd[2];

  if(pipe(fd) < 0){    //cria o pipe
    perror("pipe");    //funcao pipe retorna -1 em caso de erro
    return -1;
  }

  produtor = sem_open ("pSem", O_CREAT | O_EXCL, 0644, 1);    //cria os semaforos
  consumidor = sem_open("qSem", O_CREAT | O_EXCL, 0322, 0); // valor do semaforo eh zro pq o produtor vai esperar o consumidor

  for(i = 0; i < 10; ++i){  //cria dez filhos
    pid= fork();
    if(pid == 0){ //se processo filho: sai do ciclo!
      break;
    }
  }

  if(pid == 0){  //processo filho
    sem_wait(produtor);    //decrementa o valor do semaforo e trava ele
    close(fd[0]);      //fecha a leitura do pipe, ja que sera usada a escrita fd[1]
    int msg[1] = {10};
    printf("String enviada pelo pai no pipe: %i\n", msg[0]);

    write(fd[1], msg, sizeof(msg));  //escreve a mensagem no pipe, funcao write recebe o file descriptors, um ponteiro e o num de bytes +1 por causa do \0

    //printf("acabo produtor\n");
    close(fd[1]);       //fecha a escrita do pipe
    sem_post (consumidor);    //incrmenta o valor do sem e destrava ele
    exit(0);
  }else{    //processo pai
    for(j = 0;j < 10;j++){    //pai vai ler dez vezes
      int msg_recebida;
      close(fd[1]);    //fecha a escrita do pipe, ja que sera usada a leitura fd[0]
      sem_wait(consumidor);    //incrementa o semaforo e trava o consumidor, faz esperar
      read(fd[0], &msg_recebida, sizeof(msg_recebida));   //leitura do que foi escrito no pipe

      printf("String lida pelo filho %d no pipe: %d\n",j ,msg_recebida);
      sem_post(produtor);
    }
  close(fd[0]);//fecha a leitura

  sem_unlink("pSem");
  sem_close(produtor);
  sem_unlink("qSem");    //fechar o semaforo
  sem_close(consumidor);

  }

exit(0);

return 0;

}
