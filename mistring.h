#ifndef MISTRING_H
#define	MISTRING_H

/* Devuelve el numero de caracteres de str
 * @param str
 * return numero de caracteres.
 */
int mi_strlen (char* str);

/* Copia s2 en s1
 * @param s1 direccion destino de la copia
 * @param s2 string a copiar
 * return: s1
 */
char* mi_strcpy (char* s1, char* s2);

/* Copia s2 a partir de s1
 * @param s1 direccion de la string a partir de la cual se realiza la concatenacion
 * @param s2 string a concatenar
 * return: s1
 */
char* mi_strcat (char* s1, char* s2);


/* Crea un duplicado en memoria dinamica de str 
 * @param str string a duplicar
 */
char* mi_strdup (char* str);

/* Comprueba si s1 es igual a s2
 * @param s1 string
 * @param s2 string
 * return: 0 si son distintas, 1 si son iguales
 */
int mi_strequals (char* s1, char* s2);

#endif

