#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutexV, mutexM, mutexMR;

void* threadMasinaVerde(void* arg)
{
    sem_wait(&mutexV);

    printf("Masina verde a intrat in intersectie.\n");
    sem_post(&mutexV);
    sleep(0.5);
    printf("Masina verde a iesit din intersectie.\n");
    sem_destroy(&mutexV);
}

void* threadMotocicleta(void* arg)
{
    sem_wait(&mutexM);
    printf("Motocicleta a intrat in intersectie.\n");
    sleep(1);

    int semaforVerde; sem_getvalue(&mutexV, &semaforVerde);

    if(!semaforVerde){
        sem_post(&mutexM);
        printf("Motocicleta a iesit din intersectie.\n");
    } else {
        printf("Motocicleta a cedat trecerea masinii verzi.\n");
        sleep(1);
        sem_post(&mutexM);
        printf("Motocicleta a iesit din intersectie.\n");
    }

    sem_post(&mutexM);
}

void* threadMasinaRosie(void* arg)
{

    sem_wait(&mutexMR);
    printf("Masina rosie a intrat in intersectie.\n");
    sleep(1);

    int semaforMotocicleta; sem_getvalue(&mutexM, &semaforMotocicleta);

    if(!semaforMotocicleta) {
        sem_post(&mutexMR);
        printf("Masina rosie a iesit din intersectie.\n");
    } else {
        printf("Masina rosie a cedat trecerea motocicletei.\n");
        sleep(1);
        sem_post(&mutexMR);
        printf("Masina rosie a iesit din intersectie.\n");
    }

    sem_post(&mutexMR);
}

int main()
{
    sem_init(&mutexV, 0, 1);
    sem_init(&mutexM, 0, 1);
    sem_init(&mutexMR, 0, 1);
    pthread_t t1,t2, t3;
    pthread_create(&t2,NULL,threadMotocicleta,NULL);
    pthread_create(&t3,NULL,threadMasinaRosie,NULL);
    pthread_create(&t1,NULL,threadMasinaVerde,NULL);
    sleep(1);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    sem_destroy(&mutexM);
    sem_destroy(&mutexMR);
    return 0;
}
