#ifndef SEMAFORO_H
#define SEMAFORO_H
// semaforo.h
// ----------
//
// Implementación simple de semáforos de Dijkstra.
// Utilizable con hilos creados con pthreads
// (c) 2014-2015 José Miguel Santos Espino, Universidad de Las Palmas de Gran Canaria
// 

// Tipo de datos para los semáforos
typedef void* Semaforo;

// Crea un nuevo semáforo
// El valor inicial debe ser igual o superior a cero
// (no admite valores negativos)

Semaforo NEWSEM (int valor_inicial);

// Operaciones P/V o Wait/Signal sobre un semáforo
// Tienen el mismo comportamiento que los semáforos clásicos (Dijkstra)
// El semáforo debe haber sido creado previamente con NEWSEM()

void WAIT (Semaforo sem);
void SIGNAL (Semaforo sem);

// Destruye un semáforo creado anteriormente con NEWSEM()
// Libera los recursos que el semáforo tiene reservados. El semáforo deja de
// ser utilizable si es destruido.
// No se debería invocar a DESTROYSEM() si el semáforo tiene operaciones WAIT() en curso.

void DESTROYSEM (Semaforo sem);

#endif

