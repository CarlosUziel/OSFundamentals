// Implementación de la base de datos simulada

// Las operaciones de lectura y escritura están 
// implementadas con retardos aleatorios, de tal forma 
// que si se ejecutan concurrentemente dan problemas.

#define _BSD_SOURCE

#include "recurso.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>

// La base de datos: una ristra de caracteres
#define TAM_BUFER 80
char bufer[TAM_BUFER+1];

// Inicialización de la BD
void recurso_inicializa()
{
  int i;
  for (i=0;i<=TAM_BUFER;i++) { bufer[i]='*'; }
  bufer[TAM_BUFER] = 0;
}

// Provoca una pausa aleatoria, de hasta una décima de segundo
void retardo_aleatorio()
{
  unsigned long retardo = rand()%100000;
  usleep(retardo);
}

// Operación de lectura
char* recurso_leer()
{
  int len = 1+strlen(bufer);
  char* copia = malloc(len);
  int i;
  // Copiamos carácter a carácter, con una pausa aleatoria
  // entre cada iteración
  for (i=0;i<len;i++)
  {
    retardo_aleatorio();
    copia[i] = bufer[i];
  }
  return copia;
}

// Operación de escritura
void recurso_escribir (const char* texto)
{
  int i;
  int len = strlen(texto);
  if (len>TAM_BUFER) len=TAM_BUFER;
  // Copiamos carácter a carácter, con una pausa aleatoria
  // en cada iteración
  for (i=0;i<len;i++)
  {
    retardo_aleatorio();
    bufer[i] = texto[i];
  }
  bufer[len]=0;
}

