#include "platoManager.h"
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
#include "pistaManager.h"

PlatoManager::PlatoManager(int nPlatos_, const char* rutaDisco)
    : nPlatos(nPlatos_) {
    for (int i = 0; i < nPlatos; ++i) {
        char nombrePlato[30];
        snprintf(nombrePlato, sizeof(nombrePlato), "Plato_%d", i);

        // Crear ruta completa para el plato: rutaDisco/Plato_i
        char rutaPlato[256];
        snprintf(rutaPlato, sizeof(rutaPlato), "%s/%s", rutaDisco, nombrePlato);

        // Crear carpeta para el plato
        mkdir(rutaPlato, 0755);

        // Crear objeto Plato
        Plato plato(nombrePlato);

        // Crear pistas dentro del plato, pasando la ruta
        //pm.crearPistas(plato, rutaPlato);

        // Agregar el plato a la lista
        platos.push_back(plato);
    }
}

const std::vector<Plato>& PlatoManager::getPlatos() const {
    return platos;
}
