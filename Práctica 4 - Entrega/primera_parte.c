// Prueba concurrente de acceso a la BD simulada
// Se lanzan unos hilos lectores y unos hilos escritores
// Al no haber sincronización entre los hilos,
// el resultado es que la BD se corrompe

#include "semaforo.h"
#include "recurso.h"

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

//int readCount;
//Semaforo mutex,databaseAccess;
Semaforo mutex;
int escribiendo;
int nlec,nesc;

// Función que ejecutan los hilos lectores
// El primer parámetro no se usa, sólo se pone porque lo
// obliga la biblioteca pthreads
void* lector (void* arg)
{
  unsigned id_hilo = (unsigned)pthread_self();
  // Lee diez veces de la BD
  for (int i=1;i<=10;i++)
  {
  	
  	WAIT(mutex);
  		while(escribiendo || nesc>0){
  			SIGNAL(mutex);
  			WAIT(mutex);
  		}
  		nlec++;
  	SIGNAL(mutex);

      char* str = recurso_leer();


  	WAIT(mutex);
  		nlec--;
  	SIGNAL(mutex);

      // imprime la cadena leída y el ID del hilo 
      printf("<<< Leí    (%u) <<< %s\n", id_hilo, str);
      free(str);
  }
  return NULL;
}

// Función que ejecutan los hilos escritores
// El primer parámetro es la ristra de texto que se escribirá
// en la BD. Es un void* porque así lo obliga pthreads.
void* escritor (void* ptexto)
{ 
  char* texto = (char*)ptexto;
  // Escribimos la cadena cuatro veces
  // Antes de cada iteración hacemos una pausa aleatoria
  for (int i=1;i<=4;i++)
  { 
    sleep( rand()%5 );

  	WAIT(mutex);
  		nesc++;
  		while(escribiendo || nlec>0){
  			SIGNAL(mutex);
  			WAIT(mutex);
  		}
  		nesc--;
  		escribiendo = 1;
  	SIGNAL(mutex);

    recurso_escribir(texto);

  	WAIT(mutex);
  		escribiendo = 0;
  	SIGNAL(mutex);

    printf(">>> escribí >>> %s\n",texto);
  }
  return NULL;
}

// Rutina que lanza los hilos y los pone a acceder a la BD
void test_recurso_hilos()
{
  // Cinco lectores, cada uno lee 10 veces
  const int NLECTORES = 5;
  pthread_t lectores[NLECTORES];

  // Tres escritores, cada uno escribe 4 veces
  pthread_t escritor1,escritor2,escritor3;
  
  // Textos que escriben los escritores
  const char* texto1 = "-----------------------------------------------";
  const char* texto2 = "En un lugar de la Mancha, de cuyo nombre no quiero acordarme";
  const char* texto3 = "Como una loncha de queso en un sandwich preso";

  // Lanzamiento de los hilos lectores
  
  // Lanzamiento de los hilos escritores
  pthread_create(&escritor1,NULL,escritor,(void*)texto1);
  pthread_create(&escritor2,NULL,escritor,(void*)texto2);
  pthread_create(&escritor3,NULL,escritor,(void*)texto3);
  
  for (int lec=0;lec<NLECTORES;lec++)
    pthread_create(&lectores[lec],NULL,lector,NULL);

  // Espera por la finalización de todos los hilos
  void* dummy;
  for (int lec=0;lec<NLECTORES;lec++)
  {
    pthread_join(lectores[lec],&dummy);
  }
  pthread_join(escritor1,&dummy);
  pthread_join(escritor2,&dummy);
  pthread_join(escritor3,&dummy);
}

int main()
{
	//readCount = 0;
	//databaseAccess = NEWSEM(1);
	mutex = NEWSEM(1);
	escribiendo = 0;
	printf("»»» Comienza la prueba de hilos...\n");
	recurso_inicializa();
	test_recurso_hilos();
	printf("»»» Fin de la prueba de hilos\n");
	exit(0);
}