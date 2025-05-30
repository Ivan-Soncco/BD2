#ifndef DISCO_H
#define DISCO_H
#include "fileManager.h"

class Disco{
  private:
    char nombreDisco[50];
    int nSectoresXPista, nSectoresXBloque, nPistas, nPlatos;
    int tSector, tTotal;

    FileManager fileManager;

    void crearEstructuraDisco();
    void crearPlato(int index);
    void crearPista(const char* tipoPista, int indexPista);
    void crearSector(const char* nombreSector);

    void irACarpeta(const char* nombre);
    void volverACarpeta(const char* pathAnterior);
    const char* getNombreDisco() const;

    void guardarMetadata();
    Disco leerMetadata(const char* nombreDiscoInput);
  public:
    Disco(const char* nombre, int tSector_, int nSectoresXP_, int nPistas_, int nPlatos_);
    ~Disco(){}
    static Disco crearDinamicamente();
    FileManager& getFileManager();

    void configurarFileManager();
    void guardarFile();
    void insertar();
    void borrar();
};

#endif
