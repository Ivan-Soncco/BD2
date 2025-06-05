#ifndef PISTAMANAGER_H
#define PISTAMANAGER_H

#include "plato.h"
#include "sectorManager.h"
#include <vector>
#include "pista.h"

class PistaManager {
private:
    int nPistas;
    SectorManager& sectorManager;
    std::vector<Pista> pistas;

public:
    PistaManager(int nPistas_, SectorManager& sm);
    void crearPistas(Plato& plato, const char* rutaPlato);
    const std::vector<Pista>& getPistas() const;
    int getCantidadPistas() const { return nPistas; }
    Pista* getPista(Plato* plato, int index);
};

#endif
