#include "pista.h"
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>

#include "sector.h"

Pista::Pista(const char* nombre_, int nSectores, int tamSector): nSectoresXPista(nSectores) {
    strncpy(nombre, nombre_, sizeof(nombre));
    nombre[sizeof(nombre) - 1] = '\0';

    //Se crea la carpeta con el nombre de la pista
    if (mkdir(nombre, 0755) != 0) {
        if (errno != EEXIST) {
            perror("mkdir() error en Pista");
            return;
        } else {
            std::cerr << "Aviso: la carpeta \"" << nombre << "\" ya existe. Se reutilizará.\n";
        }
    }
    //Se accede a la carpeta con el nombre de la pista
    if (chdir(nombre) != 0) {
        perror("chdir() error");
        return;
    }
    std::cout << "  * " << nombre << std::endl;
    for(int i = 0; i<nSectoresXPista; i++ ){
        //Se genera el nombre para el sector
        char nombreSector[40];
        snprintf(nombreSector, sizeof(nombreSector), "Sector_%d.txt", i);
        //Se creara sectores Sector(nombre,tamSector)
        Sector sector(nombreSector, tamSector);
        //Añadir el objeto Sector al vector de sectores 
        sectores.push_back(sector);
    }
    //Regresa a la carpeta anterior del plato
    if (chdir("..") != 0) {
        perror("chdir() error");
        return;
    }
}

void Pista::agregarSector(const Sector& sector) {
    sectores.push_back(sector);
}

const std::vector<Sector>& Pista::getSectores() const {
    return sectores;
}

const char* Pista::getNombre() const {
    return nombre;
}
