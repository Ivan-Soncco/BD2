#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include "file.h"

class FileManager {
private:
    char nombre[50];  // nombre fijo "fileManager"
    int inicioFileManager;
    int tamFileManager;
    char rutaFileManager[100];
    char nombreDisco[100];
public:
    FileManager(const char* nombreDisco);
    const char* getNombre() const;
    const char* getRuta() const;
    int getInicio() const;
    int getTam() const;
    void setInicio(int inicio);
    void setTam(int tam);
    void setNombreDisco(const char* nombre);

    void añadirFileAFileManager(File* file_);
    void mostrarArchivosDesdeBinario(File* file);

    bool fMExiste();
    void recuperarInfoFM();
    void guardarInfoEnSector0();

    const int tNombre=20;
    const int tBloqueFM=9;
    const int tInicioFM=9;
    const int tFM=9;

};

#endif