#include "disco.h"
#include <iostream>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

Disco::Disco(const char* nombre) {
    // Copiar nombre a atributo
    strncpy(nombreDisco, nombre, sizeof(nombreDisco));
    nombreDisco[sizeof(nombreDisco) - 1] = '\0';


    // Pedir parámetros
    int nPlatos, nPistas, nSectores, tamSector;
    if(discoExiste(nombreDisco)){
        leerInfoDeDiscoYActualizar(nombreDisco, nPlatos, nPistas, nSectores, tamSector);
    } else {
        mkdir(nombreDisco, 0755);
        std::cout << "Ingrese número de platos: ";
        std::cin >> nPlatos;
        std::cout << "Ingrese número de pistas por lado: ";
        std::cin >> nPistas;
        std::cout << "Ingrese número de sectores por pista: ";
        std::cin >> nSectores;
        std::cout << "Ingrese tamaño del sector (bytes): ";
        std::cin >> tamSector;
    }
    nPlatosXDisco = nPlatos;

    // Inicializar el array de platos

    // Obtener el directorio de trabajo actual
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return;
    }

    // Cambiar el directorio de trabajo al directorio del disco
    if (chdir(nombreDisco) != 0) {
        perror("chdir() error");
        return;
    }

    for (int i = 0; i < nPlatosXDisco; ++i) {
        // Construir el nombre para el plato: Plato_i
        char nombrePlato[30];
        snprintf(nombrePlato, sizeof(nombrePlato), "Plato_%d", i);

        // Crear un objeto Plato
        Plato plato(nombrePlato, nPistas, nSectores, tamSector);

        // Añadir el objeto plato al vector de platos
        platos.push_back(plato);
    }

    // Volver al directorio de trabajo original
    if (chdir(cwd) != 0) {
        perror("chdir() error");
        return;
    }

    guardarInformacionDisco(nombreDisco, nPlatos, nPistas, nSectores, tamSector);
}

void Disco::guardarInformacionDisco(const char* nombreDisco, int nPlatos, int nPistas, int nSectores, int tamSector) {
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return;
    }

    char rutaSector[512];
    snprintf(rutaSector, sizeof(rutaSector), "%s/%s/Plato_0/Pista_0A/Sector_0.txt", cwd, nombreDisco);

    int fd = open(rutaSector, O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error al abrir archivo sector");
        return;
    }

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%d#%d#%d#%d\n", nPlatos, nPistas, nSectores, tamSector);

    ssize_t bytesEscritos = write(fd, buffer, strlen(buffer));
    if (bytesEscritos == -1) {
        perror("Error al escribir en archivo sector");
    }

    close(fd);
}

bool Disco::discoExiste(const char* nombreDisco) {
    // This function does not change the current working directory.
    DIR* dir = opendir(nombreDisco);
    if (dir) {
        closedir(dir);
        return true;
    } else {
        return false;
    }
}

void Disco::leerInfoDeDiscoYActualizar(const char* nombreDisco, int& nPlatos, int& nPistas, int& nSectores, int& tamSector) {
    char cwd[256] = {0};
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return;
    }

    char rutaSector[512] = {0};
    snprintf(rutaSector, sizeof(rutaSector), "%s/%s/Plato_0/Pista_0A/Sector_0.txt", cwd, nombreDisco);

    int fd = open(rutaSector, O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir archivo sector");
        return;
    }

    char buffer[256] = {0};
    ssize_t bytesLeidos = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesLeidos == -1) {
        perror("Error al leer archivo sector");
        close(fd);
        return;
    }
    buffer[bytesLeidos] = '\0';

    close(fd);

    sscanf(buffer, "%d#%d#%d#%d", &nPlatos, &nPistas, &nSectores, &tamSector);
}

void Disco::mostrarInformacion() {
    std::cout << "Nombre del disco: " << nombreDisco << std::endl;

    char cwd[256] = {0};
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return;
    }

    char rutaSector[512]= {0};
    snprintf(rutaSector, sizeof(rutaSector), "%s/%s/Plato_0/Pista_0A/Sector_0.txt", cwd, nombreDisco);

    int fd = open(rutaSector, O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir archivo sector");
        return;
    }

    char buffer[256]= {0};
    ssize_t bytesLeidos = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesLeidos == -1) {
        perror("Error al leer archivo sector");
        close(fd);
        return;
    }
    buffer[bytesLeidos] = '\0';

    close(fd);

    int nPlatos, nPistas, nSectores, tamSector;
    sscanf(buffer, "%d#%d#%d#%d", &nPlatos, &nPistas, &nSectores, &tamSector);

    std::cout << "Número de platos: " << nPlatos << std::endl;
    std::cout << "Número de pistas: " << nPistas << std::endl;
    std::cout << "Número de sectores: " << nSectores << std::endl;
    std::cout << "Tamaño del sector: " << tamSector << std::endl;
    mostrarArbol();
}

void Disco::mostrarArbol() {
}
