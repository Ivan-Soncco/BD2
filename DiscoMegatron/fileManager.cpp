#include "fileManager.h"
#include "logicaFunciones.h"
#include "file.h"
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#define TAM_R_NOMBREARCHIVO 50
#define TAM_R_BLOQUE 5
#define TAM_R_BYTEINICIO 5
#define TAM_R_TAMAÑOFILE 5


FileManager::FileManager() : inicioFileManager(0), tamFileManager(0) {
    strncpy(nombre, "fileManager", sizeof(nombre));
    nombre[sizeof(nombre) - 1] = '\0';
}

const char* FileManager::getNombre() const {
    return nombre;
}

int FileManager::getInicio() const {
    return inicioFileManager;
}

int FileManager::getTam() const {
    return tamFileManager;
}

void FileManager::setInicio(int inicio) {
    inicioFileManager = inicio;
}

void FileManager::setTam(int tam) {
    tamFileManager = tam;
}
void FileManager::añadirFileAFileManager(File* file_){
    
    int fd = open(getRuta(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    
    escribirCampoBinario(fd, file_->getNombre(), "char", 50);  
    int actualizarTamaño=file_->getTamFile()+50;
    

    int bloque = file_->getBloque();
    escribirCampoBinario(fd, &bloque, "int", 4);
    actualizarTamaño+=4;
     

    int inicio = file_->getInicioFile();
    escribirCampoBinario(fd, &inicio, "int", 4);
    actualizarTamaño+=4;
    

    int tam = file_->getTamFile();
    actualizarTamaño+=4;
    file_->setTamFile(actualizarTamaño); 
    escribirCampoBinario(fd, &tam, "int", 5);

    close(fd);
}

const char* FileManager::getRuta() const{
    return rutaFileManager;
}
void FileManager::setRuta(char* ruta){
    strncpy(rutaFileManager, ruta, sizeof(rutaFileManager));
      rutaFileManager[sizeof(rutaFileManager) - 1] = '\0';
}

void FileManager::mostrarArchivosDesdeBinario(File* file) {
    int fd = open(getRuta(), O_RDONLY);
    if (fd == -1) {
        perror("No se pudo abrir el archivo binario");
        return;
    }

    while (true) {
        char nombre[50];
        int bloque, inicio, tam;

        // Intentar leer cada campo
        ssize_t leido = read(fd, nombre, 50);
        if (leido == 0) break; // fin del archivo
        if (leido < 0) { perror("Error al leer nombre"); break; }

        read(fd, &bloque, sizeof(int));
        read(fd, &inicio, sizeof(int));
        read(fd, &tam, sizeof(int));

        // Mostrar como texto
        nombre[49] = '\0'; // aseguramos terminación
        std::cout << "Archivo: " << nombre << "\n";
        std::cout << "  Bloque: " << bloque << "\n";
        std::cout << "  Inicio: " << inicio << "\n";
        std::cout << "  Tamaño: " << tam << "\n";
        std::cout << "-----------------------------\n";
    }

    close(fd);
}