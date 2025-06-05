#ifndef DISCO_H
#define DISCO_H

#include <vector>
#include "plato.h"
#include "fileManager.h"
#include <dirent.h>
#include <unistd.h>

class Disco {
private:
    char nombreDisco[50];
    int nPlatosXDisco;
    int nPistasXPlato;
    int nSectoresXPista;
    int tSector;
    int tBloque;
    std::vector<Plato> platos;

    char** rutasSectoresCilindrico;   
    int totalRutasSectores;           
    int posicionActualSector;

public:
    Disco(const char* nombre);
    void guardarInformacionDisco(const char* nombreDisco, int nPlatos, int nPistas, int nSectores, int tamSector);
    bool discoExiste(const char* nombreDisco);
    void leerInfoDeDiscoYActualizar(const char* nombreDisco, int& nPlatos, int& nPistas, int& nSectores, int& tamSector);
    void mostrarInformacion();
    void mostrarArbol();
    FileManager* fileManager;

    void addFile(char* nombreFile);
    void anadirADisco(char* buff, ssize_t bleidos);
    void configurarRutaSector(char* rutaSector);
    void generarRutasSectoresCilindrico();
    int obtenerSiguienteSectorLibre();
    void anadirArchivo(const char* nombreArchivo);

    int platoActual = 0;
    int pistaActual = 0;
    char ladoActual = 'A'; // 'A' o 'B'
    int sectorActual = 0;

};

#endif
