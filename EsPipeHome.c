#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

#define BUFFER_SIZE 1024
int main(int argc, char *argv[argc])
{
    int p;
	int fd[2];	
	int n;
	int status;
	if(pipe(fd) == -1) 
	{
		printf("\nErrore creazione pipe");
		exit(-1);
	}
    p=fork(); //padre genera figlio
	if (p<0)
	{
		printf("\nErrore generazione figlio");
	}

	if(p>0) //padre
	{
		printf("\nSono il padre: il mio PID=%d\n", getpid());
		close(fd[1]);
        int numeroProdotto;
        int numeri[5];
        n=read(fd[1], numeri, 5);
        wait(&status);                
		if( n == -1)
		{
			printf("\nErrore di lettura nella pipe");
			close(fd[0]);
			exit(-1);
		}
        else
        {
            printf("Inserire un numero che verra moltiplicato per i cinque inseriti nel programma figlio");
            scanf("%d", &numeroProdotto);
            for(int i = 0; i < 5; i++)
            {
                numeri[i] = numeri[i] * numeroProdotto;
                printf("%d\n", numeri[i]);
            }            
            close(fd[0]);
        }
		
	}
	else //figlio
	{
		int numeri[5];

		printf("\nSono il figlio: il mio PID=%d\n", getpid());
		close(fd[0]);
        for(int i = 0; i < 5; i++)
        {
            printf("Inserire il %d numero\n", i + 1);
            scanf("%d", &numeri[i]);
        }
		n=write(fd[0], numeri,5);
		if(n==-1)
		{
			printf("\nErrore nella scrittura della pipe\n");
			close(fd[1]);
			exit(-1);
		}
		else 
		{			
			close(fd[1]);
		}
	}

    return 0;
}