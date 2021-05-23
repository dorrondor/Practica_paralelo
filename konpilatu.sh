#!/bin/bash

mpicc -o demo main.c funciones.c interacciones.c probabilidad.c -lgsl -lm
