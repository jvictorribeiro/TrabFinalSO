// Ipc - mem partilhada 1 com 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main(){
	pid_t pid;

	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}
  if(pid > 0){
    key_t key = ftok("shmfile",65); //ftok para gerar uma 'key' unica

    int shmid = shmget(key,1024,0666|IPC_CREAT);    //funcao shmget() para criar segmento de mem partilhada, ela retorna o id do segmento
                                                    //--shmget(key, tam em bytes do segmento, permicoes do segmento)

    char *str = (char*) shmat(shmid,(void*)0,0);      //shmat() liga o processo ao segmento -- shmat(id do segmento, endereco do segmento, 0)

    memcpy(str, "Hello!", 6);   //man page - memcpy() function copies n bytes from memory area src to memory area dest.
    printf("dado escrito na memoria pelo pai: %s\n",str);

    shmdt(str);  //desligar do segmento
  }
	else if(pid == 0){
    key_t key = ftok("shmfile",65); //ftok para gerar uma 'key' unica

    int shmid = shmget(key,1024,0666|IPC_CREAT);  //funcao shmget() para criar segmento de mem partilhada, ela retorna o id do segmento
                                                    //--shmget(key, tam em bytes do segmento, permicoes do segmento)

    char *str = (char*) shmat(shmid,(void*)0,0);  //shmat() liga o processo ao segmento -- shmat(id do segmento, endereco do segmento, 0)

    printf("dado lido da memoria pelo filho: %s\n",str);

    shmdt(str);     //desligar do segmento

    shmctl(shmid,IPC_RMID,NULL);    //destruir o segmento criado
  }

return 0;

}
