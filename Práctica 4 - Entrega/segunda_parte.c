//Implementation of shared bath
//Without Starvation

#include "semaforo.h"
#include "recurso.h"

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

Semaforo lock,mutex,bath;
int women,men,bathLimit;

/*	
 *	female thread
 */
void* female(void* arg){


	WAIT(lock);		//lock -> if while condition is true, lock wont allow more men/women to enter
	WAIT(mutex);
		printf("Llega una mujer a la sala de espera.(Dentro del baño)-> Hombres: %d Mujeres: %d\n",men,women);
		while((men != 0) || (women == bathLimit)){	//represents entering bathroom
			SIGNAL(mutex);	
			WAIT(mutex);
		}

	SIGNAL(mutex);
	SIGNAL(lock);	//unlock

	WAIT(mutex);
		women++;
	SIGNAL(mutex);

	sleep(1);		

	WAIT(mutex);

		WAIT(bath);
			printf("Una mujer hace sus cosas en el baño... Duracion aproximada: 20 minutos.(Dentro del baño)-> Hombres: %d Mujeres: %d\n",men,women);
		SIGNAL(bath);

	SIGNAL(mutex);

	
	WAIT(mutex);
		women--;
	SIGNAL(mutex);
}

/*	
 *	male thread
 */
void* male(void* arg){

	WAIT(lock);		//lock -> if while condition is true, lock wont allow more men/women to enter
	WAIT(mutex);
		printf("Llega un hombre a la sala de espera.(Dentro del baño)-> Hombres: %d Mujeres: %d\n",men,women);
		while((women != 0) || (men == bathLimit)){	//represents entering bathroom
			SIGNAL(mutex);
			WAIT(mutex);
		}

	SIGNAL(mutex);
	SIGNAL(lock);	//unlock


	WAIT(mutex);
		men++;
	SIGNAL(mutex);
		
	sleep(1);

	WAIT(mutex);

		WAIT(bath);
			printf("Un hombre hace sus cosas en el baño... Duracion aproximada: 3 minutos.(Dentro del baño)-> Hombres: %d Mujeres: %d\n",men,women);
		SIGNAL(bath);

	SIGNAL(mutex);

	WAIT(mutex);
		men--;
	SIGNAL(mutex);
}

/*
 *	Launchs male and female threads
 */
void launchThreads(){
	mutex = NEWSEM(1);
	lock = NEWSEM(1);
	bathLimit = 3;
	bath = NEWSEM(bathLimit);
	women = 0;
	men = 0;

  	const int people = 50;
  	pthread_t peopleThreads[people];

  	int i;

  	for (i = 0; i < people; i++)
  	{
  		if ( rand()%2 )
  		{
  			pthread_create(&peopleThreads[i],NULL,male,NULL);
  		}else{
  			pthread_create(&peopleThreads[i],NULL,female,NULL);
  		}
  	}
  
    void* dummy;

  	for (i=0;i<people;i++)
  		pthread_join(peopleThreads[i],&dummy);

}

int main(){
	printf("Comienzan los juegos del retrete -->\n");
	launchThreads();
	printf("Terminan los juegos del retrete.\n");
}
