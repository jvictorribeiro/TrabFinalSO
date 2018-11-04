//IPC - mem partilhada 1 com 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHTAM 200   //tamanho da mem compartilhada

int main(){
  int shmid;
  key_t key;
  char *shm;
  char *s;

  key = 1234;

  shmid = shmget(key, SHTAM, IPC_CREAT | 0666);  //funcao shmget() para criar segmento de mem partilhada, ela retorna o id do segmento
                                                  //--shmget(key, tam em bytes do segmento, permicoes do segmento)
  if(shmid < 0){
    perror("shmget");
    exit(1);
  }else
		printf("Segmento criado com sucesso!\n");

  shm = shmat(shmid, NULL, 0);      //shmat() liga o processo ao segmento -- shmat(id do segmento, endereco do segmento, 0)

  if(shm == (char *) -1){  //shmat() retorna –1 em caso de insucesso
    perror("shmat");
    exit(1);
  }

  memcpy(shm, "Hello!", 6);   //escreve hello na mem partilhada

  s = shm;
  s += 6;

  *s = 0;  // ponteiro para a string

  while(*shm != '*') //enquanto a string n aparece '*', o processo dorme
    sleep(1);

  return 0;
}
