// test_mistring.c
// ===============
// Batería de pruebas de la biblioteca "mistring"
//
// Podrás empezar a usarla cuando hayas hecho un fichero "mistring.h"
// al menos con los cuerpos vacíos de las funciones mi_strlen, mi_strcat 
// y mi_strequals

#ifndef TEST
#include "mistring.h"
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Este bloque es para probar que "test_mistring.c" está correctamente
// escrito, sin necesidad de que aún esté implementado "mi_string.c"
// En ese caso, compilar con la opción -DTEST
#ifdef TEST
#include <string.h>
#define mi_strlen strlen
#define mi_strcat strcat
#define mi_strequals(a,b) (strcmp(a,b)==0)
#endif

#define DebeSerCierto(x)	assert(x)
#define DebeSerFalso(x)		assert(!(x))

void INICIO_TEST (const char* titulo_test)
{
  printf("- batería de pruebas para %s: ", titulo_test); 
 	// fflush fuerza que se imprima el mensaje anterior
	// sin necesidad de utilizar un salto de línea
	fflush(stdout);
}

void FIN_TEST (const char* titulo_test)
{
  printf ("hecho\n");
}

void test_mi_strlen()
{
  char cadena_1000 [1000];
  int i;
  #define LONGITUD_TEST 500
  for (i=0; i<LONGITUD_TEST; i++) 
  {
    cadena_1000[i] = 32+(rand()%96);
  }
  cadena_1000[LONGITUD_TEST] = '\0'; 
  
  INICIO_TEST("mi_strlen");
	DebeSerCierto(mi_strlen("hola mundo") == 10);
	DebeSerCierto(mi_strlen("") == 0);
	DebeSerCierto(mi_strlen(cadena_1000) == LONGITUD_TEST);
	FIN_TEST("mi_strlen");
}

void test_mi_strcat()
{
	char cadena1[15] = { 'h','o','l','a', '\0' };
	char* resultado = mi_strcat(cadena1," mundo");
	INICIO_TEST("mi_strcat");
	DebeSerCierto(cadena1 == resultado);
	DebeSerCierto(mi_strlen(cadena1)==10);
	DebeSerCierto(mi_strequals(cadena1,"hola mundo"));
	DebeSerFalso(mi_strequals(cadena1,"hola"));
	
	//Concatenar palabra grande en palabra pequeña
	char cadena2[50] = "hola";
	char* resultado2 = mi_strcat(cadena2," palabra_muy_muy_muy_muy_grande");
	DebeSerCierto(cadena2 == resultado2);
	DebeSerCierto(mi_strlen(cadena2)==35);
	DebeSerCierto(mi_strequals(cadena2,"hola palabra_muy_muy_muy_muy_grande"));
	DebeSerFalso(mi_strequals(cadena2,"hola"));
	
	//Concatenar palabra pequeña en palabra grande
	char cadena3[50] = "palabra_muy_muy_muy_muy_grande";
	char* resultado3 = mi_strcat(cadena3," hola");
	DebeSerCierto(cadena3 == resultado3);
	DebeSerCierto(mi_strlen(cadena3)==35);
	DebeSerCierto(mi_strequals(cadena3,"palabra_muy_muy_muy_muy_grande hola"));
	DebeSerFalso(mi_strequals(cadena3,"palabra_muy_muy_muy_muy_grande"));
	
	//Concatenar varias palabras seguidas
	char cadena4[30] = "prueba";
	int i;
	for (i = 1; i <= 5 ; i++)
	{
		DebeSerCierto(mi_strlen(cadena4)==(6*i));
		char* resultado4 = mi_strcat(cadena4,"prueba");
		DebeSerCierto(cadena4 == resultado4);
		DebeSerCierto(mi_strlen(cadena4)==(6*(i+1)));
	}
	
	FIN_TEST("mi_strcat");
}

void test_mi_strcpy()
{
	char cadena1[15] = { 'h','o','l','a', '\0' };
	char* resultado = mi_strcpy(cadena1,"mundo");
	INICIO_TEST("mi_strcpy");
	DebeSerCierto(cadena1 == resultado);
	DebeSerCierto(mi_strlen(cadena1)==5);
	DebeSerCierto(mi_strequals(cadena1,"mundo"));
	DebeSerFalso(mi_strequals(cadena1,"hola"));
	
	//Copiar una palabra grande en una pequeña
	char cadena2[50] = "palabra_pequeña";
	char* resultado2 = mi_strcpy(cadena2,"palabra_muy_muy_muy_muy_grande");
	DebeSerCierto(cadena2 == resultado2);
	DebeSerCierto(mi_strlen(cadena2)==30);
	DebeSerCierto(mi_strequals(cadena2,"palabra_muy_muy_muy_muy_grande"));
	DebeSerFalso(mi_strequals(cadena2,"palabra_pequeña"));
	
	//Copiar una palabra pequeña en una grande
	char cadena3[50] = "palabra_muy_muy_muy_muy_grande";
	char* resultado3 = mi_strcpy(cadena3,"palabra_pequeña");
	DebeSerCierto(cadena3 == resultado3);
	//La ñ son dos caracteres, pero el tamaño real es 15, no 16.
	DebeSerCierto(mi_strlen(cadena3)==16);
	DebeSerCierto(mi_strequals(cadena3,"palabra_pequeña"));
	DebeSerFalso(mi_strequals(cadena3,"palabra_muy_muy_muy_muy_grande"));
	
	//Copiar palabra varias veces
	char cadena4[10] = "prueba";
	int i;
	for (i = 1; i <= 5 ; i++)
	{
		char* resultado4 = mi_strcpy(cadena4,"magdalena");
		DebeSerCierto(cadena4 == resultado4);
		DebeSerCierto(mi_strlen(cadena4)==9);
		DebeSerCierto(mi_strequals(cadena4,"magdalena"));
	}
	
	FIN_TEST("mi_strcpy");
	

}

void test_mi_strdup()
{
	char cadena1[5] = { 'h','o','l','a', '\0' };
	char* resultado = mi_strdup(cadena1);
	INICIO_TEST("mi_strdup");
	DebeSerCierto(cadena1 != resultado);
	DebeSerCierto(mi_strlen(cadena1)==4);
	DebeSerCierto(mi_strlen(resultado)==4);
	DebeSerCierto(mi_strequals(cadena1,"hola"));
	DebeSerCierto(mi_strequals(resultado,"hola"));
	
	//Llamadas consecutivas
	char cadena2[16] = "prueba_duplicate";
	int i;
	for (i = 0; i < 10; i++)
	{
		char* resultado2 = mi_strdup(cadena2);
		DebeSerCierto(cadena2 != resultado);
		DebeSerCierto(mi_strlen(cadena2)==16);
		DebeSerCierto(mi_strlen(resultado2)==16);
		DebeSerCierto(mi_strequals(cadena2,"prueba_duplicate"));
		DebeSerCierto(mi_strequals(resultado2,"prueba_duplicate"));
		//Evita fugas de memoria
		free(resultado2);
	}
	
	FIN_TEST("mi_strdup");
}

void test_mi_strequals(){
	
	char cadena[5]="hola";
	
	INICIO_TEST("mi_strequals");
	
	//Palabras iguales
	DebeSerCierto(mi_strequals(cadena, "hola"));
	
	//La primera letra es diferente
	DebeSerFalso(mi_strequals(cadena, "oola"));
	
	//La última letra es diferente
	DebeSerFalso(mi_strequals(cadena, "holl"));	
	
	//La palabra diferente es mayor
	DebeSerFalso(mi_strequals(cadena, "holita"));
	
	//La palabra diferente es menor
	DebeSerFalso(mi_strequals(cadena, "hol"));

	//Todas las letras son distintas, mismo tamaño
	DebeSerFalso(mi_strequals(cadena, "caso"));
	
	//Todas las letras son distintas, diferente tamaño
	DebeSerFalso(mi_strequals(cadena, "casito"));
	
	FIN_TEST("mi_strequals");
}


void ejecuta_tests ()
{
	test_mi_strlen();
	test_mi_strequals();
	test_mi_strcat();
	test_mi_strcpy();
	test_mi_strdup();
}

main()
{
	puts("Iniciando tests...");
	
	ejecuta_tests();
	
	puts("Batería de test completa.");
}
