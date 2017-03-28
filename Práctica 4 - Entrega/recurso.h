#ifndef __RECURSO_H
#define __RECURSO_H

// Simulacro de una base de datos
// en la que podemos leer o escribir una ristra de texto

// Inicialización de la BD
void recurso_inicializa();

// Operación de lectura
// Devuelve un char* que es una copia de la ristra que 
// hay en la BD. 
char* recurso_leer();

// Operación de escritura
void recurso_escribir (const char* texto);

#endif
