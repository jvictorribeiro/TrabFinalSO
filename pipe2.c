//IPC - pipe 10 com 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    int fd1[2],fd2[2],fd3[2],fd4[2],fd5[2],fd6[2],fd7[2],fd8[2],fd9[2],fd10[2], turn=0;
    /*cria-se 10 pipes e a variavel turn que vai definir o que cada um vai fazer (ler, escrever, aguardar...) */
    pid_t pid;   /* Armazena o pid, para o tratamento de pai e filho */

    /* Cria o pipe 1 */
    if(pipe(fd1)<0) {
        perror("pipe") ;
        return -1 ;
    }
    /* Cria o pipe 2 */
    if(pipe(fd2)<0) {
        perror("pipe") ;
        return -1 ;
    }
    /* Cria o pipe 3 */
    if(pipe(fd3)<0) {
        perror("pipe") ;
        return -1 ;
    }
    /* Cria o pipe 4 */
    if(pipe(fd4)<0) {
        perror("pipe") ;
        return -1 ;
    }
    /* Cria o pipe 5 */
    if(pipe(fd5)<0) {
        perror("pipe") ;
        return -1 ;
    }
    /* Cria o pipe 6 */
    if(pipe(fd6)<0) {
        perror("pipe") ;
        return -1 ;
    }
    /* Cria o pipe 7 */
    if(pipe(fd7)<0) {
        perror("pipe") ;
        return -1 ;
    }
    /* Cria o pipe 8 */
    if(pipe(fd8)<0) {
        perror("pipe") ;
        return -1 ;
    }
    /* Cria o pipe 9 */
    if(pipe(fd9)<0) {
        perror("pipe") ;
        return -1 ;
    }
    /* Cria o pipe 10 */
    if(pipe(fd10)<0) {
        perror("pipe") ;
        return -1 ;
    }

    
