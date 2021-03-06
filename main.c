/*
Programa nagusia

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "definiciones.h"
#include "funciones.h"
#include "interacciones.h"
#include "probabilidad.h"

int main(int argc, char *argv[]){

    struct timeval begin, end;
	remove("Resultato.metricas");
	remove("Resultato.pos");

	
	// irakurri sarrera-datuak
    if (argc != 5) {
        printf ("\n\nERROR: ./ fichero alfa beta semilla. \n\n");
        exit (-1);
    }

	int i, alfa, beta, s, kontajiatuak, j, k, kont, sano, muerto, incubando, contagiado, recuperado;
	float r0;
	int world_rank, world_size;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	/*if (world_size < 2) {
		fprintf(stderr, "World tiene que ser >= 2%s\n", argv[0]);
    		MPI_Abort(MPI_COMM_WORLD, 1);
  	}*/

    alfa = atoi(argv[2]);
    beta = atoi(argv[3]);
    s = atoi(argv[4]);
    srand(s);
    
    struct poblaciones poblacion;
    struct persona_virus *personas, *jaso, *kontaj, *kontag_global;
    struct metricas metrica;
    metrica.anterior=1;
    int *count = malloc(world_size*sizeof(int));    
    int *despla = malloc(world_size*sizeof(int));

	leer_parametros(&poblacion, argv[1]);
    personas = malloc(poblacion.tam*sizeof(struct persona_virus));

	

    //poblacioneko parametroak eta aldagai globalak jaso fitxategi batetik.
    if(world_rank == 0){

        if (poblacion.tam%world_size!=0){
        printf("\nEl tamaño de la poblacion y la cantidad de procesadores deben ser múltiplos\n\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
        }

        if(poblacion.tam < world_size){
        	printf("\n El numero de nodos %d no puede ser mayor que la poblacion %d \n\n", world_size, poblacion.tam);
        	MPI_Abort(MPI_COMM_WORLD, 1);
        }

        //Denbora neurketak egiteko aldagaiak hasieratu
        gettimeofday(&begin, 0);

    	inicializar(poblacion.tam, personas, poblacion.tam_escenario, alfa, beta, s);
    	//Escoger el primer infectado de manera aleatoria.
    	paciente0(poblacion.tam, personas);
	
    }

    jaso = malloc(((poblacion.tam)/world_size)*sizeof(struct persona_virus));

    MPI_Datatype pers;
    crear_tipo(&personas[0].edad, &personas[0].t_estado, &personas[0].estado, &personas[0].prob_morir, &personas[0].pos, &personas[0].vel, &pers);

    //MPI_ScatterV
    MPI_Scatter(&personas[0], (poblacion.tam)/world_size, pers, jaso, (poblacion.tam)/world_size, pers, 0, MPI_COMM_WORLD );

    for(i=0; i<poblacion.tiempo_simulacion; i++){
    	//Mover cada persona y determinar la nueva velocidad (para la proxima iteracion)
    	movimiento(jaso, poblacion.tam/world_size, poblacion.tam_escenario);
    	//Calcular los estados y los contagios
    	k = 0;
    	kontajiatuak = propagacion(jaso, &poblacion, world_size);
        //printf("kontajiatuak: %d\n",kontajiatuak);
        
        if(kontajiatuak == 0){
            kontaj = malloc(sizeof(struct persona_virus));
            kontaj[0].edad = -1;
            kontajiatuak=1;
      }else{
            kontaj = malloc(kontajiatuak*sizeof(struct persona_virus));
            for(j=0; j<poblacion.tam/world_size; j++){
                if(jaso[j].estado == 2){
                    //mempcpy
                    memcpy(&kontaj[k], &jaso[j], sizeof(struct persona_virus)); 
			//printf("%d %d %d %d %d\n",j,k, poblacion.tam/world_size, jaso[j].edad, kontaj[k].edad);  
                  k++;
                }
            }
        }

	count[world_rank]=kontajiatuak;

	MPI_Allreduce(&kontajiatuak, &kont, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	kontag_global = malloc(kont*sizeof(struct persona_virus));
	MPI_Allgather(MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, &count[0], 1, MPI_INT, MPI_COMM_WORLD);
	MPI_Allgather(&kontajiatuak, 1, MPI_INT, despla, 1, MPI_INT, MPI_COMM_WORLD);
	for(j=0;j<world_size;j++){
		if(j==0){
			despla[j]=0;
		}else{
			despla[j]=despla[j-1]+count[j-1];
		}
	}
	MPI_Allgatherv(&kontaj[0], kontajiatuak, pers, kontag_global, count, despla, pers, MPI_COMM_WORLD);
	free(kontaj);
        for(j=0; j<kont; j++){
            if (kontag_global[j].edad!=-1){
                propagacion_2(kontaj, &poblacion, kont);
            }
        }
   	free(kontag_global);
	coger_metricas(&poblacion, personas, &metrica);
/*	if(world_rank==0){
		r0=(float) kont/ant;
		if(kont==0)
			ant=1;
		else
			ant=kont;
		printf("R0 %f\n", r0);
	}*/
		if(i%5==0){
			//Posizioa eta metrikak behar dira. Orduan zergaitik bidali pertsona guztiak? Ideia ondo dago baina ez da OSO POLITA.
			MPI_Gather(&jaso[0], (poblacion.tam)/world_size, pers, personas, (poblacion.tam)/world_size, pers, 0, MPI_COMM_WORLD );
            MPI_Reduce(&metrica.sano, &sano, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
            MPI_Reduce(&metrica.incubando, &incubando, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
            MPI_Reduce(&metrica.contagiado, &contagiado, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
            MPI_Reduce(&metrica.recuperado, &recuperado, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
            MPI_Reduce(&metrica.muerto, &muerto, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
            MPI_Reduce(&metrica.r0, &r0, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
			if(world_rank == 0 && poblacion.metrica == 1){
			metrica.sano=sano;
			metrica.incubando=incubando;
			metrica.contagiado=contagiado;
			metrica.recuperado=recuperado;
			metrica.muerto=muerto;
			metrica.r0=r0;
			escribir_metrica(&poblacion, personas, &metrica);
			}
			if(world_rank == 0 && poblacion.posicion == 1)
			escribir_posicion(&poblacion, personas);
		}
    }
	free(jaso);
    free(personas);
	free(despla);
	free(count);
	if(world_rank == 0){

        gettimeofday(&end, 0);

        long seconds = end.tv_sec - begin.tv_sec;
        long microseconds = end.tv_usec - begin.tv_usec;
        double elapsed = seconds + microseconds*1e-6;

        printf("Time measured: %.3f seconds.\n", elapsed);
	}

	MPI_Finalize();


	return(0);
}
