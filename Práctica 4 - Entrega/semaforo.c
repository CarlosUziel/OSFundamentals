#include "semaforo.h"

#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

typedef struct
{
	int valor;
	pthread_mutex_t cerrojo;
	pthread_cond_t cola;
} semaforo_t;

Semaforo NEWSEM (int valor_inicial)
{
   assert(valor_inicial>=0);
   semaforo_t* sem = (semaforo_t*)malloc(sizeof(semaforo_t));
   assert(sem!=NULL);
	sem->valor = valor_inicial;
	pthread_mutex_init (&sem->cerrojo,NULL);
	pthread_cond_init (&sem->cola,NULL);
	return (void*)sem;
}

void DESTROYSEM (Semaforo psem)
{
   semaforo_t* sem = (semaforo_t*)psem;
   pthread_mutex_destroy(&sem->cerrojo);
   pthread_cond_destroy(&sem->cola);
   free(psem);
}

void WAIT (Semaforo psem)
{
   semaforo_t* sem = (semaforo_t*)psem;
	pthread_mutex_lock (&sem->cerrojo);
	while ( sem->valor <= 0 )
	{
		pthread_cond_wait (&sem->cola,&sem->cerrojo);
	}
	sem->valor--;
	pthread_mutex_unlock (&sem->cerrojo);
}

void SIGNAL (Semaforo psem)
{
   semaforo_t* sem = (semaforo_t*)psem;
	pthread_mutex_lock (&sem->cerrojo);
	sem->valor++;
	pthread_cond_signal (&sem->cola);
	pthread_mutex_unlock (&sem->cerrojo);	
}
