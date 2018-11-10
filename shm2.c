// Ipc - mem partilhada 10 com 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
//headers para mem partilhada
#include <sys/ipc.h>
#include <sys/shm.h>
//headers para semaforo  --> compilar com -lpthread
#include <semaphore.h>
#include <fcntl.h>

int main(){
	pid_t pid;
	int *valor, i, j;
	sem_t *produtor;
  sem_t *consumidor;

	/*cria os semaforos*/
	produtor = sem_open ("pSem", O_CREAT | O_EXCL, 0644, 1);
	consumidor = sem_open("qSem",O_CREAT | O_EXCL, 0322, 0);  	//eh zero pq o produtor vai esperar o consumidor

	for(i = 0; i < 10; ++i){		//cria 10 filhos
  	pid= fork();
  	if(pid == 0){ //se processo filho: sai do ciclo!
    	break;
    }
 	}

  if(pid == 0){		//filho
		sem_wait(produtor);		//trava semaforo
    key_t key = ftok("shmfile",65); //ftok para gerar uma 'key' unica

    int shmid = shmget(key, 1024, 0666|IPC_CREAT);    //funcao shmget() para criar segmento de mem partilhada, ela retorna o id do segmento
                                                    //--shmget(key, tam em bytes do segmento, permicoes do segmento)
    int *valor =  shmat(shmid,(void*)0,0);      //shmat() liga o processo ao segmento -- shmat(id do segmento, endereco do segmento, 0)

		*valor = 10;
		printf("dado escrito na memoria pelo filho: %d\n" ,valor[0]);

		shmdt(valor);  //desligar do segmento
		sem_post (consumidor);		//destrava
		exit(0);
  }
	else{
		for(j = 0;j < 10;j++){	//pai vai ler 10 vezes
			sem_wait(consumidor);		//trava consumidor
    	key_t key = ftok("shmfile",65); //ftok para gerar uma 'key' unica

    	int shmid = shmget(key,1024,0666|IPC_CREAT);  //funcao shmget() para criar segmento de mem partilhada, ela retorna o id do segmento
                                                    //--shmget(key, tam em bytes do segmento, permicoes do segmento)

			int *valor =  shmat(shmid,(void*)0,0);  //shmat() liga o processo ao segmento -- shmat(id do segmento, endereco do segmento, 0)

  		printf("dado lido pelo pai do filho %d: %d\n", j, valor[0]);
			sem_post(produtor);		//libera produtor
			if(i == 9){
    		shmdt(valor);     //desligar do segmento
				shmctl(shmid,IPC_RMID,NULL);    //destruir o segmento criado/
			}
			wait(NULL);
		}
		//fechar os semaforos
		sem_unlink("pSem");
		sem_close(produtor);
		sem_unlink("qSem");
		sem_close(consumidor);
	}

exit(0);

return 0;

}
