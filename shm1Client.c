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

  shmid = shmget(key, SHTAM, 0666);    //--shmget(key, tam em bytes do segmento, permicoes do segmento), ver shm1Server.c

  if(shmid < 0){
    perror("shmget");
    exit(1);
  }else
		printf("Segmento criado com sucesso!\n");

  shm = shmat(shmid, NULL, 0);  //shmat() liga o processo ao segmento

  if(shm == (char *) -1){
    perror("shmat");
    exit(1);
  }

  for(s = shm; *s != 0; s++)    //pega a string que está na mem compartilhada
    printf("%c", *s);

  printf("\n\n");

  *shm = '*';

  return 0;
}
