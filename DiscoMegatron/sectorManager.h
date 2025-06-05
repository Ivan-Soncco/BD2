#ifndef SECTORMANAGER_H
#define SECTORMANAGER_H

#include "pista.h"
#include <vector>
#include "sector.h"

class SectorManager {
private:
    int nSectores;
    int tamSector;
    std::vector<Sector> sectores;

public:
    SectorManager(int nSectores_, int tamSector_);
    void crearSectores(Pista& pista, const char* rutaPista);
    const std::vector<Sector>& getSectores() const;
    int getCantidadSectores() const { return nSectores; }
};

#endif
