/*
definiciones.h 
*/


//Tamaño de la poblacion, radio de contagio, periodo de incubacion, periodo de recuperacion
//Probabilidad de contagio, probabilidad de cambio de velocidad y direccion

#include <stdio.h>
#include <time.h>
#include "funciones.h"
#include "interacciones.h"
#include "probabilidad.h"
#include "definiciones.h"

struct poblaciones
{
	int tam, radio_cont, periodo_incu, periodo_rec, tam_escenario, tiempo_simulacion;
	float prob_cont, prob_camb_vel;
};


//Edad, tiempo en el estado, probabilidad de morir, vector de posicion, vector de velocidad, vector de estado.
struct persona_virus
{
	int edad, t_estado, estado;
	float prob_morir;
	int pos[2], vel[2];
};


//extern int tam_escenario;

//extern int tiempo_simulacion;
