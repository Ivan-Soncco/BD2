#include "plato.h"
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>

Plato::Plato(const char* nombre_, int nPistas ,int nSectores,int tamSector): nPistasXPlato(nPistas) {
    strncpy(nombre, nombre_, sizeof(nombre));
    nombre[sizeof(nombre) - 1] = '\0';

    // Se crea la carpeta con el nombre del Plato
    mkdir(nombre, 0755);

    // Ingresar a la carpeta del plato
    if (chdir(nombre) != 0) {
        perror("chdir() error");
        return;
    }

    for(int i = 0; i<nPistasXPlato; i++ ){
        // Se genera el nombre para la pista que se creara ya sea del lado A o del B
        char nombrePistaA[30];
        char nombrePistaB[30];
        snprintf(nombrePistaA, sizeof(nombrePistaA), "Pista_%dA", i);
        snprintf(nombrePistaB, sizeof(nombrePistaB), "Pista_%dB", i);

        // Se crearan pistas para el ladoA y ladoB del plato Pista(nombrePista,nSectores, tamSector);
        Pista pistaA(nombrePistaA, nSectores, tamSector);
        Pista pistaB(nombrePistaB, nSectores, tamSector);

        // Añadir el objeto pista al vector de pistas segun sea del ladoA o del B
        ladoA.push_back(pistaA);
        ladoB.push_back(pistaB);
    }

    // Regresar a la carpeta anterior del plato
    if (chdir("..") != 0) {
        perror("chdir() error");
        return;
    }
}

const char* Plato::getNombre() const {
    return nombre;
}
