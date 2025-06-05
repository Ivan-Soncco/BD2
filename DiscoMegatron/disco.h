#ifndef DISCO_H
#define DISCO_H

#include <vector>
#include "plato.h"
#include "platoManager.h"
#include "pistaManager.h"
#include "sectorManager.h"
#include <dirent.h>

class Disco {
private:
    char nombreDisco[50];
    int nPlatosXDisco;
    std::vector<Plato> platos;

public:
    Disco(const char* nombre);
    void guardarInformacionDisco(const char* nombreDisco, int nPlatos, int nPistas, int nSectores, int tamSector);
    bool discoExiste(const char* nombreDisco);
    void leerInfoDeDiscoYActualizar(const char* nombreDisco, int& nPlatos, int& nPistas, int& nSectores, int& tamSector);
    void mostrarInformacion();
    void mostrarArbol();
};

#endif
