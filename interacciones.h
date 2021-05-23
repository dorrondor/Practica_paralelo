/*
interacciones.h
*/

#ifndef INTERACCIONES_H
#define INTERACCIONES_H

 void movimiento(struct persona_virus *personas, int tam_poblacion, int tam_escenario);
 int propagacion(struct persona_virus *personas, struct poblaciones *poblacion, int world_size);
 void propagacion_2(struct persona_virus *personas, struct poblaciones *poblacion, int tam);
 
#endif
