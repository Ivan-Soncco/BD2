#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include "file.h"

class FileManager {
private:
    char nombre[50];  // nombre fijo "fileManager"
    int inicioFileManager;
    int tamFileManager;
    char rutaFileManager[100];
public:
    FileManager();
    const char* getNombre() const;
    const char* getRuta() const;
    void setRuta(char* ruta);
    int getInicio() const;
    int getTam() const;
    void setInicio(int inicio);
    void setTam(int tam);

    void añadirFileAFileManager(File* file_);
    void mostrarArchivosDesdeBinario(File* file);

};

#endif