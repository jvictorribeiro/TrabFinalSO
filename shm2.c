// Ipc - mem partilhada 10 com 1

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

int main(){
	pid_t pid;
	int *valor, i, j;
	sem_t *produtor;
  sem_t *organizador;

	produtor = sem_open ("pSem", O_CREAT | O_EXCL, 0644, 1);
	organizador = sem_open("oSem",O_CREAT | O_EXCL, 0322, 0);//e zro pq o produtor vai esperar o consumidor

	for(i = 0; i < 10; ++i){
    pid= fork();
    if(pid == 0){ //se processo filho: sai do ciclo!
      break;
    }
 	}

  if(pid == 0){
		sem_wait(produtor);
    key_t key = ftok("shmfile",65); //ftok para gerar uma 'key' unica

    int shmid = shmget(key, 1024, 0666|IPC_CREAT);    //funcao shmget() para criar segmento de mem partilhada, ela retorna o id do segmento
                                                    //--shmget(key, tam em bytes do segmento, permicoes do segmento)
    int *valor =  shmat(shmid,(void*)0,0);      //shmat() liga o processo ao segmento -- shmat(id do segmento, endereco do segmento, 0)

		*valor = 10;
		printf("dado escrito na memoria pelo filho: %d\n",valor[0]);

		shmdt(valor);  //desligar do segmento
		sem_wait(organizador);
		sem_post (produtor);
		exit(0);
  }
	else{
		for(j=0;j<10;j++){
    		key_t key = ftok("shmfile",65); //ftok para gerar uma 'key' unica

    		int shmid = shmget(key,1024,0666|IPC_CREAT);  //funcao shmget() para criar segmento de mem partilhada, ela retorna o id do segmento
                                                    //--shmget(key, tam em bytes do segmento, permicoes do segmento)

				int *valor =  shmat(shmid,(void*)0,0);  //shmat() liga o processo ao segmento -- shmat(id do segmento, endereco do segmento, 0)

    		printf("dado lido pelo pai do filho %d: %d\n", j, valor[0]);
				sem_post(organizador);
				if(i == 9){
    			shmdt(valor);     //desligar do segmento
					shmctl(shmid,IPC_RMID,NULL);    //destruir o segmento criado/
				}
				wait(NULL);
		}
	sem_unlink ("oSem");//fechar o semaforo
	sem_close(organizador);
	sem_unlink ("pSem");
	sem_close(produtor);
	}

exit(0);

return 0;

}
