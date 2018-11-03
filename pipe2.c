#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
  int i, n =10;
  pid_t pid,pid2,pid3,pid4,pid5,pid6,pid7,pid8,pid9;
  int fd[2], fd1[2], fd2[2], fd3[2], fd4[2], fd5[2], fd6[2], fd7[2], fd8[2], fd9[2];

  if(pipe(fd)<0){ //cria o pipe1
    perror("pipe");
    return -1;
  }

  if(pipe(fd1)<0){  //cria o pipe2
    perror("pipe");
    return -1;
  }

  if(pipe(fd2)<0){  //cria o pipe3
    perror("pipe");
    return -1;
  }

  if(pipe(fd3)<0){  //cria o pipe4
    perror("pipe");
    return -1;
  }

  if(pipe(fd4)<0){  //cria o pipe5
    perror("pipe");
    return -1;
  }

  if(pipe(fd5)<0){  //cria o pipe6
    perror("pipe");
    return -1;
  }

  if(pipe(fd6)<0){  //cria o pipe7
    perror("pipe");
    return -1;
  }

  if(pipe(fd7)<0){  //cria o pipe8
    perror("pipe");
    return -1;
  }

  if(pipe(fd8)<0){  //cria o pipe9
    perror("pipe");
    return -1;
  }

  if(pipe(fd9)<0){  //cria o pipe10
    perror("pipe");
    return -1;
  }

 for(i = 0; i < n; ++i){
   pid= fork();
   if(pid != 0){ //se processo filho: sai do ciclo!
     break;
   }
 }

 if(pid > 0 && i==0){
   close(fd[0]);//fecha a leitura do pipe, ja que sera usada a escrita fd[1]
   char msg[200]="Hello1!!!";
   printf("Enviada pelo processo 1 no pipe:'%s'\n",msg);

   write(fd[1],msg,sizeof(msg)+1);//escreve a mensagem no fd[1]

 }else if(pid ==0){
   char msg_recebida[200];
   close(fd[1]);//fecha a escrita do pipe, ja que sera usada a leitura fd[0]

   read(fd[0],msg_recebida,sizeof(msg_recebida));//leitura do que foi escrito

   printf("string recebida pelo processo 0 no pipe:'%s'\n",msg_recebida);
  }

  if(pid > 0 && i==1){
    close(fd1[0]);//fecha a leitura do pipe, ja que sera usada a escrita fd[1]
    char msg[200]="Hello2!!!";
    printf("Enviada pelo processo 2 no pipe:'%s'\n",msg);

    write(fd1[1],msg,sizeof(msg)+1);//escreve a mensagem no fd[1]
    wait(NULL);

  }else if(pid ==0){
    char msg_recebida[200];
    close(fd1[1]);//fecha a escrita do pipe, ja que sera usada a leitura fd[0]

    read(fd1[0],msg_recebida,sizeof(msg_recebida));//leitura do que foi escrito

    printf("string recebida pelo processo 0 no pipe:'%s'\n",msg_recebida);
  }

  if(pid > 0 && i==2){
    close(fd2[0]);//fecha a leitura do pipe, ja que sera usada a escrita fd[1]
    char msg[200]="Hello3!!!";
    printf("Enviada pelo processo 3 no pipe:'%s'\n",msg);

    write(fd2[1],msg,sizeof(msg)+1);//escreve a mensagem no fd[1]
    wait(NULL);

  }else if(pid ==0){
    char msg_recebida[200];
    close(fd2[1]);//fecha a escrita do pipe, ja que sera usada a leitura fd[0]

    read(fd2[0],msg_recebida,sizeof(msg_recebida));//leitura do que foi escrito

    printf("string recebida pelo processo 0 no pipe:'%s'\n",msg_recebida);
  }

  if(pid > 0 && i==3){
    close(fd3[0]);//fecha a leitura do pipe, ja que sera usada a escrita fd[1]
    char msg[200]="Hello4!!!";
    printf("Enviada pelo processo 4 no pipe:'%s'\n",msg);

    write(fd3[1],msg,sizeof(msg)+1);//escreve a mensagem no fd[1]
    wait(NULL);

  }else if(pid ==0){
    char msg_recebida[200];
    close(fd3[1]);//fecha a escrita do pipe, ja que sera usada a leitura fd[0]

    read(fd3[0],msg_recebida,sizeof(msg_recebida));//leitura do que foi escrito

    printf("string recebida pelo processo 0 no pipe:'%s'\n",msg_recebida);
  }

  if(pid > 0 && i==4){
    close(fd4[0]);//fecha a leitura do pipe, ja que sera usada a escrita fd[1]
    char msg[200]="Hello5!!!";
    printf("Enviada pelo processo 5 no pipe:'%s'\n",msg);

    write(fd4[1],msg,sizeof(msg)+1);//escreve a mensagem no fd[1]
    wait(NULL);

  }else if(pid ==0){
    char msg_recebida[200];
    close(fd4[1]);//fecha a escrita do pipe, ja que sera usada a leitura fd[0]

    read(fd4[0],msg_recebida,sizeof(msg_recebida));//leitura do que foi escrito

    printf("string recebida pelo processo 0 no pipe:'%s'\n",msg_recebida);
  }

  if(pid > 0 && i==5){
    close(fd5[0]);//fecha a leitura do pipe, ja que sera usada a escrita fd[1]
    char msg[200]="Hello6!!!";
    printf("Enviada pelo processo 6 no pipe:'%s'\n",msg);

    write(fd5[1],msg,sizeof(msg)+1);//escreve a mensagem no fd[1]
    wait(NULL);

  }else if(pid ==0){
    char msg_recebida[200];
    close(fd5[1]);//fecha a escrita do pipe, ja que sera usada a leitura fd[0]

    read(fd5[0],msg_recebida,sizeof(msg_recebida));//leitura do que foi escrito

    printf("string recebida pelo processo 0 no pipe:'%s'\n",msg_recebida);
  }

  if(pid > 0 && i==6){
    close(fd6[0]);//fecha a leitura do pipe, ja que sera usada a escrita fd[1]
    char msg[200]="Hello7!!!";
    printf("Enviada pelo processo 7 no pipe:'%s'\n",msg);

    write(fd6[1],msg,sizeof(msg)+1);//escreve a mensagem no fd[1]
    wait(NULL);

  }else if(pid ==0){
    char msg_recebida[200];
    close(fd6[1]);//fecha a escrita do pipe, ja que sera usada a leitura fd[0]

    read(fd6[0],msg_recebida,sizeof(msg_recebida));//leitura do que foi escrito

    printf("string recebida pelo processo 0 no pipe:'%s'\n",msg_recebida);
  }

  if(pid > 0 && i==7){
    close(fd7[0]);//fecha a leitura do pipe, ja que sera usada a escrita fd[1]
    char msg[200]="Hello8!!!";
    printf("Enviada pelo processo 8 no pipe:'%s'\n",msg);

    write(fd7[1],msg,sizeof(msg)+1);//escreve a mensagem no fd[1]
    wait(NULL);

  }else if(pid ==0){
    char msg_recebida[200];
    close(fd7[1]);//fecha a escrita do pipe, ja que sera usada a leitura fd[0]

    read(fd7[0],msg_recebida,sizeof(msg_recebida));//leitura do que foi escrito

    printf("string recebida pelo processo 0 no pipe:'%s'\n",msg_recebida);
  }

  if(pid > 0 && i==8){
    close(fd8[0]);//fecha a leitura do pipe, ja que sera usada a escrita fd[1]
    char msg[200]="Hello9!!!!";
    printf("Enviada pelo processo 9 no pipe:'%s'\n",msg);

    write(fd8[1],msg,sizeof(msg)+1);//escreve a mensagem no fd[1]
    wait(NULL);

  }else if(pid ==0){
    char msg_recebida[200];
    close(fd8[1]);//fecha a escrita do pipe, ja que sera usada a leitura fd[0]

    read(fd8[0],msg_recebida,sizeof(msg_recebida));//leitura do que foi escrito

    printf("string recebida pelo processo 0 no pipe:'%s'\n",msg_recebida);
  }

  if(pid > 0 && i==9){
    close(fd9[0]);//fecha a leitura do pipe, ja que sera usada a escrita fd[1]
    char msg[200]="Hello10!!!";
    printf("Enviada pelo processo 10 no pipe:'%s'\n",msg);

    write(fd9[1],msg,sizeof(msg)+1);//escreve a mensagem no fd[1]
    wait(NULL);

  }else if(pid ==0){
    char msg_recebida[200];
    close(fd9[1]);//fecha a escrita do pipe, ja que sera usada a leitura fd[0]

    read(fd9[0],msg_recebida,sizeof(msg_recebida));//leitura do que foi escrito

    printf("string recebida pelo processo 0 no pipe:'%s'\n",msg_recebida);
  }

return 0;
}
