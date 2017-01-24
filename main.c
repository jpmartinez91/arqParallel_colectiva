/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: pato
 *
 * Created on 16 de enero de 2017, 9:46
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#define MAX_LONG 10000

typedef int Matriz[MAX_LONG][MAX_LONG];
typedef int b[];

/*
 * 
 * 
 */

int esPrimo(int valor);
int factorial(int valor);
void llenar_matriz(Matriz a);
int contar_primos_principal(Matriz a);
int contar_primos_secundaria(Matriz a);
void buscar_primos_principal(Matriz a, b p);
void buscar_primos_secundaria(Matriz a, b p);

int main(int argc, char** argv) {
    Matriz* a;
    a = (Matriz*) malloc(sizeof (Matriz));
    int numeroProcesadores, idProceso;
    int sumaPrincipal;
    int sumaSecundaria;
    double time_start;
    double time_end;
    double total_time;
    int fac;
    int facS;
    int primosPrincipal;
    int primosSecuencial;
    b* principal;
    b* secundaria;
    b* fprincipal;
    b* fsecundaria;
    

    char nombre_proceso[MPI_MAX_PROCESSOR_NAME];
    int longuitud_nombre_proceso;

    MPI_Init(&argc, &argv);
    time_start = MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &idProceso);
    MPI_Comm_size(MPI_COMM_WORLD, &numeroProcesadores);

    MPI_Get_processor_name(nombre_proceso, &longuitud_nombre_proceso);


    if (idProceso == 0) {
        llenar_matriz(*a);
        primosPrincipal = contar_primos_principal(*a);
        principal = (b*) malloc(sizeof (int) *primosPrincipal);
        fprincipal = (b*) malloc(sizeof (int) *primosPrincipal);
        primosSecuencial = contar_primos_secundaria(*a);
        secundaria = (b*) malloc(sizeof (int) *primosSecuencial);
        fsecundaria = (b*) malloc(sizeof (int) *primosSecuencial);
        buscar_primos_principal(*a, *principal);
        buscar_primos_secundaria(*a, *secundaria);
    }
    
    MPI_Scatter(principal, primosPrincipal, MPI_INT, fprincipal, primosPrincipal, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < primosPrincipal; i++) {
        printf("%i \n", principal[i]);
    }
    
    //MPI_Scatter(dPrincipal, primosPrincipal, MPI_INT, facPrincipal, primosPrincipal, MPI_INT, 0, MPI_COMM_WORLD);
    //MPI_Scatter(dPrincipal, primosPrincipal, MPI_INT, facPrincipal, primosPrincipal, MPI_INT, 0, MPI_COMM_WORLD);

    /*for (int i = 0; i < primosPrincipal; i++) {
        fac = factorial(facPrincipal[i]);
    }

    MPI_Reduce(&fac, &sumaPrincipal, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


    int dSecundaria [primosSecuencial];
    int facSecundaria [primosSecuencial];
    //int dSecundaria [MAX_LONG];
    int contSecundaria = 0;
    for (int i = 0; i < MAX_LONG; i++) {
        if (esPrimo(a[i][i]) == 1) {
            dSecundaria[contPrincipal] = a[i][i];
            contSecundaria++;
        }
    }



    MPI_Scatter(dSecundaria, primosSecuencial, MPI_INT, facSecundaria, primosSecuencial, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < primosSecuencial; i++) {
        facS = factorial(facSecundaria[i]);
    }

    MPI_Reduce(&facS, &sumaSecundaria, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


    time_end = MPI_Wtime();

    total_time = time_end - time_start;
    if (idProceso == 0) {
        printf("El valor de la D. Principal es: %d\n", sumaPrincipal);
        printf("El valor de la D. Principal es: %d\n", sumaSecundaria);
        printf("Tiempo de procesamiento: %f \n", total_time);
    }*/
    MPI_Finalize();
    return (EXIT_SUCCESS);
}

int esPrimo(int valor) {
    int contador = 0;
    if (valor > 1) {
        for (int i = 1; i <= valor; i++) {
            if (valor % i == 0) {
                contador++;
            }
        }
        if (contador == 2) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

int factorial(int valor) {
    int fact = 1;
    for (int i = 1; i <= valor; i++) {
        fact = fact*i;
    }
    return fact;
}

void llenar_matriz(Matriz a) {
    for (int i = 0; i < MAX_LONG; i++) {
        for (int j = 0; j < MAX_LONG; j++) {
            a[i][j] = (rand() % 11);
        }
    }
}

int contar_primos_principal(Matriz a) {
    int suma = 0;
    for (int i = 0; i < MAX_LONG; i++) {
        if (esPrimo(a[i][i]) == 1) {
            suma++;
        }
    }
    return suma;
}

int contar_primos_secundaria(Matriz a) {
    int suma = 0;
    for (int i = 0; i < MAX_LONG; i++) {
        if (esPrimo(a[i][MAX_LONG - i - 1]) == 1) {
            suma++;
        }
    }
    return suma;
}

void buscar_primos_principal(Matriz a, b p) {
    int suma = 0;
    for (int i = 0; i < MAX_LONG; i++) {
        if (esPrimo(a[i][i]) == 1) {
            p[suma] = a[i][i];
            printf("%d \t", p[suma]);
            suma++;
        }
    }
}

void buscar_primos_secundaria(Matriz a, b p) {
    int suma = 0;
    for (int i = 0; i < MAX_LONG; i++) {
        if (esPrimo(a[i][MAX_LONG - i - 1]) == 1) {
            p[suma] = a[i][MAX_LONG - i - 1];
            printf("%d \t", p[suma]);
            suma++;
        }
    }
}