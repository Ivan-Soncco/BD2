#ifndef PISTA_H
#define PISTA_H

#include <vector>
#include "sector.h"

class Pista {
private:
    char nombre[30];
    int nSectoresXPista;

    std::vector<Sector> sectores;

public:
    Pista(const char* nombre_, int nSectores,int tamSector);
    void agregarSector(const Sector& sector);
    const std::vector<Sector>& getSectores() const;
    const char* getNombre() const;
};

#endif
