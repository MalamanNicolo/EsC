#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#define BUFFER_SIZE 1024
int main(int argc, char* argv[argc])
{
    //dichiarazioni variabili
    int p;
    int n;
    int fd[2];
    
    
    if(pipe(fd) == -1)//creazione pipe
    {
        printf("Errore di generazione della pipe");
        exit(-1);
    }
    p = fork();
    if(p < 0)
    {
        printf("Errore nella generazione del figlio");
        exit(-1);
    }
    if(p > 0)//padre
    {
        printf("PID: %d, PID del figlio: %d", getpid(), p);
        close(fd[0]);//chiude in lettura la pipe(uscita pipe)
        const char* messaggio = "Buona fortuna";
        if(write(fd[1], messaggio, strlen(messaggio)) < 0)
        {
            printf("\nErrore scrittura pipe");
            exit(-1);
        }
        close(fd[1]);
        wait();
    }else//figlio
    {
        char buffer[BUFFER_SIZE];
        printf("\nPID: %d, PID del padre: %d\n", getpid(), getppid());
        close(fd[1]);//chiude in scrittura la pipe(entrata pipe)
        n = read(fd[0], buffer, BUFFER_SIZE);
        if( n < 0)
        {
            printf("\nErrore lettura pipe");
            exit(-1);
        }
        buffer[n] = '\0';
        printf("Messaggio ricevuto: %s\n", buffer);
        close(fd[0]);
    }
    return 0;
}