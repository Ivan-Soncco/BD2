#include "file.h"
#include <cstring>

//Constructor cree el File ADminArchivos con lista enlazada
File::File(const char* nombre_,int bloque_,int inicioFile_,int tamFile_) {
    strncpy(nombre, nombre_, sizeof(nombre));
    nombre[sizeof(nombre) - 1] = '\0';

    this->bloque = bloque_;
    this->inicioFile = inicioFile_;
    this->tamFile = tamFile_;
}

void File::setNombre(const char* nombreNuevo) {
    strncpy(nombre, nombreNuevo, sizeof(nombre));
    nombre[sizeof(nombre) - 1] = '\0'; // Asegura terminación
}

void File::setBloque(int b) {
    bloque = b;
}

void File::setInicioFile(int inicio) {
    inicioFile = inicio;
}

void File::setTamFile(int tam) {
    tamFile = tam;
}

const char* File::getNombre() const {
    return nombre;
}

int File::getBloque() const {
    return bloque;
}

int File::getInicioFile() const {
    return inicioFile;
}

int File::getTamFile() const {
    return tamFile;
}