/*
interacciones.c

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "definiciones.h"
#include "funciones.h"
#include "probabilidad.h"
#include "interacciones.h"



//Mover personas del vector XXXXX compuesto por struct-s persona_virus
//Despues de moverlos determinar la nueva velocidad y direccion de movimiento.
void movimiento(struct persona_virus *personas, int tam_poblacion, int tam_escenario  ){
	
	int i;

	for(i=0; i<tam_poblacion; i++){		
		
		//Posizio berria esleitu
		// Escenarioen limiteak pasatzen ez dituela ziurtatuz. (aldagai global bat da)
		if((personas[i].pos[0]+personas[i].vel[0]) < tam_escenario && (personas[i].pos[0]+personas[i].vel[0]) > 0)
			personas[i].pos[0] += personas[i].vel[0];

		if((personas[i].pos[1]+personas[i].vel[1]) < tam_escenario && (personas[i].pos[1]+personas[i].vel[1]) > 0)
			personas[i].pos[1] += personas[i].vel[1];
        
		//Hurrengo iterazioarako velocidad aldatu.
		personas[i].vel[0] = calc_vel();
		personas[i].vel[1] = calc_vel();
	}
}


// Se recorrera todo el array de personas.
//Primero se analizara su estado aplicando la probabilidades de poblacion.XXXX y el persona_virus.t_estado
//       contagioso (2) -> muerto (5)
//       contagioso (2) -> curado (3)
//       incubando (1) -> contagioso (2)
//Si el estado es contagioso se analizara el radio de contagio.
//Si dentro del estado esta alguna persona de estado sano (0) se aplicara poblacion.prob_cont
int propagacion(struct persona_virus *personas, struct poblaciones *poblacion, int world_size){
	int kontajiatuak = 0;
	int i;
	int tam = (poblacion->tam)/world_size;
	//printf("%d\n",tam);

	for(i=0; i<(poblacion->tam)/world_size; i++){

        //Inkubatzen badago eta inkubatzeko periodoa bukatzean, kontagioso bihurtuko da.
		if(personas[i].estado == 1 && personas[i].t_estado >= poblacion->periodo_incu){
			personas[i].estado = 2;
			personas[i].t_estado = 0;
			//kontajiatuak++;
		}

		//Kontajioso bada baina periodo de recuperacion bukatzen badu, sendatu egingo da.
		if(personas[i].estado == 2 && personas[i].t_estado >= poblacion->periodo_rec){
			personas[i].estado = 3;
			personas[i].t_estado = 0;
//			kontajiatuak--;
		}

		//Kontajioso direnak hilko diren ala ez kalkulatu. 
		//Eta honez gain bere kutsaketa erradioa begiratu, pertsona osasuntxurenbat badago bertan kutxatzeko probabilitatea aplikatu
		if(personas[i].estado == 2){
			// EZ bada  hil
			if(p_morir(personas[i].prob_morir) == 1)  personas[i].t_estado++;
			// Hil egin bada
			else if (p_morir(personas[i].prob_morir) == 0){
				personas[i].estado=5;
				personas[i].t_estado = 0;
//				kontajiatuak--;
			}
		}

		if(personas[i].estado==2){
                        kontajiatuak++;
                }

		//Estadoan gehitu (egun bat gehitu)
		personas[i].t_estado ++;

		//Aztertu ea radio de kontajion dagoen eta hala bada probabilidad de contajio aplikatu.
		if(personas[i].estado == 2 && personas[i].t_estado!=0){ 
			int j, z, k;
			//Bere erradioa aztertu eta batenbat barrun badago
			for(j=0; j<(poblacion->tam)/world_size; j++){
				for(z=-poblacion->radio_cont; z<poblacion->radio_cont; z++){
					if(personas[i].pos[0]+z == personas[j].pos[0] && personas[j].estado == 0){
						for(k=-poblacion->radio_cont; k<poblacion->radio_cont; k++){
							if(personas[i].pos[1]+k == personas[j].pos[1] && personas[j].estado == 0){
								if(p_contagiar(poblacion->prob_cont) == 1){
									personas[j].estado = 1;
									personas[j].t_estado = 0;
								}
							}
						}
					}
				}
			} 
		}
	}
	
	return kontajiatuak;
}


 void propagacion_2(struct persona_virus *personas, struct poblaciones *poblacion, int tam){

	int i;
	for(i=0; i<tam; i++){
 	if(personas[i].estado == 2 && personas[i].t_estado!=0){ 
			int j, z, k;
			//Bere erradioa aztertu eta batenbat barrun badago
			for(j=0; j<tam; j++){
				for(z=-poblacion->radio_cont; z<poblacion->radio_cont; z++){
					if(personas[i].pos[0]+z == personas[j].pos[0] && personas[j].estado == 0){
						for(k=-poblacion->radio_cont; k<poblacion->radio_cont; k++){
							if(personas[i].pos[1]+k == personas[j].pos[1] && personas[j].estado == 0){
								if(p_contagiar(poblacion->prob_cont) == 1){
									personas[j].estado = 1;
									personas[j].t_estado = 0;
								}
							}
						}
					}
				}
			} 
		}
	} 
}




