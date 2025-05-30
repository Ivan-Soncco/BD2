#include "logicaFunciones.h"
#include "disco.h"
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cstdio>
#include <iostream>

// Verifica si una carpeta existe en el directorio actual
bool logicaValidarDiscoExistente(const char* nombreDisco) {
    struct stat info;
    return (stat(nombreDisco, &info) == 0 && S_ISDIR(info.st_mode));
}

// Carga los atributos desde el primer sector del archivo sector0.csv
void logicaCargarDatosDesdeMetadata(const char* nombreDisco, char* nombre, int* tSector, int* nSectoresXPista, int* nSectoresXBloque, int* nPistas, int* nPlatos) {
    char pathOriginal[200];
    getcwd(pathOriginal, sizeof(pathOriginal));

    // Ruta al primer sector del disco
    char pathPrimerSector[300];
    snprintf(pathPrimerSector, sizeof(pathPrimerSector),
             "%s/%s/plato1/pista1A/sector0.csv", pathOriginal, nombreDisco);

    int fd = open(pathPrimerSector, O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir sector0.csv para lectura de metadata");
        exit(1);
    }

    char buffer[200];
    ssize_t bytesLeidos = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);

    if (bytesLeidos <= 0) {
        std::cerr << "No se pudo leer metadata o archivo vacío\n";
        exit(1);
    }

    buffer[bytesLeidos] = '\0';  // Asegurar terminación de cadena

    int leidos = sscanf(buffer, "%49[^#]#%d#%d#%d#%d#%d",
                        nombre, tSector, nSectoresXPista, nSectoresXBloque, nPistas, nPlatos);

    if (leidos != 6) {
        std::cerr << "Error al parsear metadata del disco\n";
        exit(1);
    }
}

void crearRutaPlatoPistaSector(char* plato,char* pista,char* sector,char* ruta){
    char ruta2[100];
    snprintf(ruta2, 100, "plato%s/pista%s/sector%s.csv",
            plato, pista, sector);
    strcat(ruta,ruta2);
}

void escribirCampo(int fd, const void* campo, const char* tipo, int tamaño) {
    char buffer[128]; // suficientemente grande
    memset(buffer, ' ', tamaño); // padding con espacios

    if (strcmp(tipo, "char") == 0) {
        const char* str = static_cast<const char*>(campo);
        strncpy(buffer, str, tamaño);
    } 
    else if (strcmp(tipo, "int") == 0) {
        int valor = *static_cast<const int*>(campo);
        char numStr[32];
        snprintf(numStr, sizeof(numStr), "%*d", tamaño, valor);  // Alineación derecha
        strncpy(buffer, numStr, tamaño);
    }

    write(fd, buffer, tamaño);
}

void escribirCampoBinario(int fd, const void* campo, const char* tipo, int tamaño) {
    if (strcmp(tipo, "char") == 0) {
        // Para strings, rellenamos con ceros si es más corto
        char buffer[tamaño];
        memset(buffer, 0, tamaño);  // padding con \0 (null terminator)
        strncpy(buffer, static_cast<const char*>(campo), tamaño);
        write(fd, buffer, tamaño);
    } 
    else if (strcmp(tipo, "int") == 0) {
        int valor = *static_cast<const int*>(campo);
        write(fd, &valor, sizeof(int));  // escribe 4 bytes directamente
    }
}