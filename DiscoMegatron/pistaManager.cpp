#include "pistaManager.h"
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>

PistaManager::PistaManager(int nPistas_, SectorManager& sm)
    : nPistas(nPistas_), sectorManager(sm) {}

void PistaManager::crearPistas(Plato& plato, const char* rutaPlato) {
    for (int i = 0; i < nPistas; ++i) {
        for (int lado = 0; lado < 2; ++lado) {
            char nombrePista[30];
            snprintf(nombrePista, sizeof(nombrePista), "Pista_%d%c", i, lado == 0 ? 'A' : 'B');

            Pista pista(nombrePista);
            pistas.push_back(pista);
            plato.agregarPista(pista, lado == 1); // lado == 1 → B

            // Crear ruta de carpeta: rutaPlato/Pista_0A
            char rutaPista[256];
            snprintf(rutaPista, sizeof(rutaPista), "%s/%s", rutaPlato, nombrePista);
            mkdir(rutaPista, 0755);

            // Crear los sectores en esa ruta
            sectorManager.crearSectores(pista, rutaPista);
        }
    }
}

const std::vector<Pista>& PistaManager::getPistas() const {
    return pistas;
}

Pista* PistaManager::getPista(Plato* plato, int index) {
    if (index >= 0 && index < pistas.size()) {
        return &pistas[index];
    }
    return nullptr;
}
