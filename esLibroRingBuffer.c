#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 10
typedef struct {
    int buffer[BUFFER_SIZE];
    int N;
} RingBuffer;

RingBuffer bufferRing;

int count = 0;
int generIndex = 0;
int outputindex = 0;
pthread_mutex_t mutex;
pthread_cond_t not_empty;
pthread_cond_t not_full;

void* generatoreN(void* arg) {
    for (int i = 0; i < bufferRing.N; i++) {
        int num = rand() % 100;        
        pthread_mutex_lock(&mutex);            
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&not_full, &mutex);
        }
        bufferRing.buffer[generIndex] = num;
        generIndex = (generIndex + 1) % BUFFER_SIZE;
        count++;       
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void* output(void* arg) {
    for (int i = 0; i < bufferRing.N; i++) {
        pthread_mutex_lock(&mutex);        
        while (count == 0) {
            pthread_cond_wait(&not_empty, &mutex);
        }
        int num = bufferRing.buffer[outputindex];
        outputindex = (outputindex + 1) % BUFFER_SIZE;
        count--;        
        printf("%d\n", num);        
        pthread_cond_signal(&not_full);        
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
     if (argc != 3) {
        printf("Errore nella compilazione del programma\n");
        return -1;
    }
    bufferRing.N = atoi(argv[2]);
    if (bufferRing.N <= 0) {
        printf("Il numero di elementi deve essere positivo\n");
        return -1;
    }
    pthread_t generatoreN_thread, output_thread;

    pthread_create(&generatoreN_thread, NULL, generatoreN, NULL);
    pthread_create(&output_thread, NULL, output, NULL);

    pthread_join(generatoreN_thread, NULL);
    pthread_join(output_thread, NULL);
    return 0;
}