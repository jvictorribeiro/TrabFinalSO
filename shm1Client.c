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

  shmid = shmget(key, SHTAM, 0666);  //criar a mem compartilhada

  if(shmid < 0){
    perror("shmget");
    exit(1);
  }

  shm = shmat(shmid, NULL, 0);

  if(shm == (char *) -1){
    perror("shmat");
    exit(1);
  }

  for(s = shm; *s != 0; s++)    //pega a string que está na mem compartilhada
    printf("%c", *s);

  *shm = '*';

  return 0;
}
