/*
funciones.h
*/

#ifndef FUNCIONES_H
#define FUNCIONES_H

//leer parametros de la poblacion + tamaño de escenario + tiempo de simulacion (numero de ciclos) de un FICHERO
 void leer_parametros(struct poblaciones *poblacion, char *f);

//inicializar el array de persona_virus personas
 void inicializar(int tam_poblacion, struct persona_virus *personas, int tam_escenario, int alfa, int beta, int s);

//Elegir aleatoriamente el primer paciente infectado
 void paciente0(int tam_poblacion, struct persona_virus *personas);


 int porciento(int tam, int c);

//EScribir los resultados en un fichero
 void escribir(struct poblaciones *poblacion, struct persona_virus *personas);

 void erakutsi(struct persona_virus *personas, struct poblaciones *poblacion);
 
#endif
