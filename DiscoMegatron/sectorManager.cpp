#include "sectorManager.h"
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>

SectorManager::SectorManager(int nSectores_, int tamSector_)
    : nSectores(nSectores_), tamSector(tamSector_) {}

void SectorManager::crearSectores(Pista& pista, const char* rutaPista) {
    for (int i = 0; i < nSectores; ++i) {
        char nombreSector[50];
        snprintf(nombreSector, sizeof(nombreSector), "Sector_%d.txt", i);

        char rutaSector[300];
        snprintf(rutaSector, sizeof(rutaSector), "%s/%s", rutaPista, nombreSector);

        // Abrir archivo para escritura, crear si no existe, permisos rw-r--r--
        int fd = open(rutaSector, O_WRONLY | O_CREAT, 0644);
        if (fd == -1) {
            perror("Error al crear archivo sector");
            continue;
        }

        // Crear buffer lleno de espacios
        char* buffer = new char[tamSector];
        memset(buffer, ' ', tamSector);

        // Escribir el buffer completo
        ssize_t totalEscrito = 0;
        while (totalEscrito < tamSector) {
            ssize_t bytesEscritos = write(fd, buffer + totalEscrito, tamSector - totalEscrito);
            if (bytesEscritos == -1) {
                perror("Error al escribir en archivo sector");
                break;
            }
            totalEscrito += bytesEscritos;
        }

        delete[] buffer;
        close(fd);

        Sector sector(nombreSector, tamSector);
        sectores.push_back(sector);
    }
}

const std::vector<Sector>& SectorManager::getSectores() const {
    return sectores;
}
