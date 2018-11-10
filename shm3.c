// Ipc - mem partilhada 10 com 10

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
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
	sem_t *sincro;
	sem_t *sincro2;

	produtor = sem_open ("pSem", O_CREAT | O_EXCL, 0644, 1);
	consumidor = sem_open("qSem",O_CREAT | O_EXCL, 0322, 0);//e zro pq o produtor vai esperar o consumidor
	sincro = sem_open ("rSem", O_CREAT | O_EXCL, 0161, 1);
	sincro2 = sem_open ("sSem", O_CREAT | O_EXCL, 1288, 0);

	for(i = 0; i < 20; ++i){
		if((pid=fork())<0){
			perror("fork\n");
			exit(1);
		}
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
		shmdt(valor);     //desligar do segmento

		sem_unlink("pSem");
		sem_close(produtor);
		sem_unlink("qSem");//fechar o semaforo
		sem_close(consumidor);
		sem_unlink("rSem");
		sem_close(sincro);
		sem_unlink("sSem");
		sem_close(sincro2);
	  exit(0);

	}else if(i < 10){
		sem_wait(produtor);		//trava produtor
    key_t key = ftok("shmfile",65); //ftok para gerar uma 'key' unica

    int shmid = shmget(key, 1024, 0666|IPC_CREAT);    //funcao shmget() para criar segmento de mem partilhada, ela retorna o id do segmento
                                                    //--shmget(key, tam em bytes do segmento, permicoes do segmento)
    int *valor =  shmat(shmid,(void*)0,0);      //shmat() liga o processo ao segmento -- shmat(id do segmento, endereco do segmento, 0)

		*valor = 10;
		printf("dado escrito pelo filho %d na mem: %d\n",i ,valor[0]);

		sem_post(sincro2);		//sincronizador2
		sem_post(consumidor);	//libera consumidor
		sem_wait(sincro);			//sincronizador

		shmdt(valor);  //desligar do segmento

		exit(0);
	}else{
		sem_wait(consumidor);		//espera consumidor
		key_t key = ftok("shmfile",65); //ftok para gerar uma 'key' unica

    int shmid = shmget(key,1024,0666|IPC_CREAT);  //funcao shmget() para criar segmento de mem partilhada, ela retorna o id do segmento
                                                    //--shmget(key, tam em bytes do segmento, permicoes do segmento)
		int *valor =  shmat(shmid,(void*)0,0);  //shmat() liga o processo ao segmento -- shmat(id do segmento, endereco do segmento, 0)

    printf("dado lido pelo filho %d: %d\n", i, valor[0]);
		sem_post(produtor);		//libera produtor
		sem_wait(sincro2);
		sem_post(sincro);
		if(i == 19){
    	shmdt(valor);     //desligar do segmento
			shmctl(shmid,IPC_RMID,NULL);    //destruir o segmento criado/
		}

		wait(NULL);
	}
exit(0);

return 0;

}
