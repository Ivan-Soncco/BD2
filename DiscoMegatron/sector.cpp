#include "sector.h"
#include <cstdio>
#include <cstring>
#include <fcntl.h>   // open
#include <unistd.h>  // write, close, lseek
#include <sys/stat.h> // permisos
#include <iostream>

Sector::Sector(const char* nombre_, int tamanoSector) : tamano(tamanoSector) {
    strncpy(nombre, nombre_, sizeof(nombre));
    nombre[sizeof(nombre) - 1] = '\0';

    // Verificar si el archivo ya existe
    struct stat st;
    if (stat(nombre, &st) == 0) {
        std::cerr << "Aviso: el archivo \"" << nombre << "\" ya existe. No se sobrescribirá.\n";
        return;
    }
    
    // Creo un archivo .txt 
    int fd = open(nombre, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("open() error en sector");
        return;
    }

    // Mueve el puntero al byte (tamano - 1)
    if (lseek(fd, tamano - 1, SEEK_SET) == -1) {
        perror("lseek() error");
        close(fd);
        return;
    }

    // Escribe un byte vacío para asignar espacio
    if (write(fd, "", 1) != 1) {
        perror("write() error");
        close(fd);
        return;
    }

    // Cierra el archivo
    close(fd);

    std::cout << "    * " << nombre << std::endl;
}

const char* Sector::getNombre() const {
    return nombre;
}

int Sector::getTamano() const {
    return tamano;
}
