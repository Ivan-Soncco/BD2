#ifndef PLATOMANAGER_H
#define PLATOMANAGER_H

#include <vector>
#include "plato.h"
#include "pistaManager.h"

class PlatoManager {
private:
    int nPlatos;
    std::vector<Plato> platos;

public:
    PlatoManager(int nPlatos_, const char* rutaDisco);
    const std::vector<Plato>& getPlatos() const;
    int getCantidadPlatos() const { return platos.size(); }
    Plato* getPlato(int index) { return &platos[index]; }
};

#endif
