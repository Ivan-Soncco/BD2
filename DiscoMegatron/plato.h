#ifndef PLATO_H
#define PLATO_H

#include <vector>
#include "pista.h"

class Plato {
private:
    char nombre[30];
    int nPistasXPlato;
    std::vector<Pista> ladoA;
    std::vector<Pista> ladoB;

public:
    Plato(const char* nombre_, int nPistas,int nSectores,int tamSector);
    void agregarPista(const Pista& pista, bool ladoB_);
    const std::vector<Pista>& getLadoA() const;
    const std::vector<Pista>& getLadoB() const;
    const char* getNombre() const;
};

#endif
