/*
Probabilidad.c

En este archivo van las funciones auxiliares que tienen que ver con la probabilidad.

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include "definiciones.h"
#include "funciones.h"
#include "interacciones.h"
#include "probabilidad.h"


void calc_edad(int numero_personas, struct persona_virus *personas, int alfa, int beta, int seed){

	int i, edad;
    float mu;
    gsl_rng *r;

	
    mu = 100;  

    /* 
     * Inicializar la librería, el generador de números pseudoaleatorios y la semilla.
     */
    gsl_rng_env_setup();
    r = gsl_rng_alloc (gsl_rng_default);
    gsl_rng_set(r, seed);

    for(i = 0;i < numero_personas;i++){
        personas[i].edad = roundf(mu * gsl_ran_beta(r, alfa, beta));
        
    }

}

//calcular una posicion aleatoria (inicio)
int calc_pos(int tam_escenario){									//beti zenbaki bera ez emateko semilla bezela j pasa
	int i = rand()%tam_escenario;
	return i;
}

//calcular la velocidad maximo
int calc_vel(){
	int i = rand()%5;
	int j;
	switch(i){
		case 0:
			j=-2;
			break;
		case 1:
			j=-1;
			break;
		case 2:
			j=0;
			break;
		case 3:
			j=1;
			break;
		case 4:
			j=2;
			break;
	}
	return j;
}

//calcular probabilidad de morirse
float calc_morir(int edad){
	float i;
    if(edad>=80){
        i = 0.148;
    }else if(edad>=70 && edad<80){
        i = 0.08;
    }else if(edad>=60 && edad<70){
        i = 0.036;
    }else if(edad>=50 && edad<60){
        i = 0.013;
    }else if(edad>=40 && edad<50){
        i = 0.004;
    }else if(edad>=10 && edad<40){
        i = 0.002;
    }else{
        i = 0;
    }
	return i;
			
}

//Aplicar la probabilidad de morir
//Return 0  HIL egin da
//Return 1  EZ da HIL
int p_morir(float prob_morir){
	//0 edo 1 zenbaki bat hemango du.
	int j = rand()%101;
	if(j < (prob_morir*100.0)) return 0;   // HIL EGIN DA
	else return 1;							// EZ DA HIL
}


//Aplicar la probabilidad de contagiar
//Return 0  NO se contagia
//Return 1  SI se contagia
int p_contagiar(int prob_contagiar){
	//0 edo 1 zenbaki bat hemango du.
	int j = rand()%101;
	if(j < (prob_contagiar*100)) return 0;   // No se contagia
	else return 1;							// Si se contagia
}

