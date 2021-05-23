/*
Probabilidad.h

En este archivo van las funciones auxiliares que tienen que ver con la probabilidad.

*/

#ifndef PROBABILIDAD_H
#define PROBABILIDAD_H

//Calcula la edad utilizando gsl
 void calc_edad(int numero_personas, struct persona_virus *personas, int alfa, int beta, int seed);

//Devuele una posicion aleatori menor que el maximo del escenario.
 int calc_pos(int tam_escenario);

//Devuelve una nueva velocidad aleatoriamente +-2
 int calc_vel();

//Devuelve a probabilidad de morirse en funcion de la edad
 float calc_morir(int edad);

//Segun el resultado determinaremos si el paciente muere o no 
 int p_morir(float prob_morir);

 //Segun el resultado determinaremos si el paciente se contagia o no 
 int p_contagiar(int prob_contagiar);
 
#endif
