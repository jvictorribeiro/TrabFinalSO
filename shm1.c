// Ipc - mem partilhada 1 com 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>	/*key_t, pit_t,...*/
#include <sys/ipc.h>
#include <sys/shm.h>	/*shmat()...*/
#include <unistd.h>

int main(){
	pid_t pid;
	int *valor;

	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}

  if(pid > 0){ //pai escreve
    key_t key = ftok("shmfile",65); //ftok para gerar uma 'key' unica

    int shmid = shmget(key,1024,0666|IPC_CREAT);    //funcao shmget() para criar segmento de mem partilhada, ela retorna o id do segmento
                                                    //--shmget(key, tam em bytes do segmento, permicoes do segmento)
		valor = shmat(shmid,(void*)0,0);      //shmat() liga o processo ao segmento -- shmat(id do segmento, endereco do segmento, 0)

		*valor = 10;
    printf("Valor escrito na memoria pelo pai: %d\n",valor[0]);

    shmdt(valor);  //desligar do segmento
  }
	else if(pid == 0){	//filho le
    key_t key = ftok("shmfile",65); //ftok para gerar uma 'key' unica

    int shmid = shmget(key,1024,0666|IPC_CREAT);  /*funcao shmget() para criar segmento de mem partilhada, ela retorna o id do segmento
                                                    --shmget(key, tam em bytes do segmento, permicoes do segmento)*/

    int *valor = shmat(shmid,(void*)0,0);  //shmat() liga o processo ao segmento -- shmat(id do segmento, endereco do segmento, 0)

    printf("Valor lido da memoria pelo filho: %d\n",valor[0]);

    shmdt(valor);     //desligar do segmento

    shmctl(shmid,IPC_RMID,NULL);    //destruir o segmento criado
  }

return 0;

}
