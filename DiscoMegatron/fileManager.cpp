#include "fileManager.h"
#include "logicaFunciones.h"
#include "file.h"
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>



FileManager::FileManager(const char* nombreDisco) : inicioFileManager(15), tamFileManager(0) {
    strncpy(nombre, "fileManager", sizeof(nombre));
    nombre[sizeof(nombre) - 1] = '\0';

    setNombreDisco(nombreDisco);
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return;
    }
    snprintf(rutaFileManager, sizeof(rutaFileManager), "%s/%s/Plato_0/Pista_0A/Sector_0.txt", cwd, nombreDisco);
    
    if(fMExiste()){
        recuperarInfoFM();
    }else{
        guardarInfoEnSector0();
    }
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
    

    
    actualizarTamaño+=4;
    file_->setTamFile(actualizarTamaño);
    int tam = file_->getTamFile();
    escribirCampoBinario(fd, &tam, "int", 5);

    close(fd);
}

const char* FileManager::getRuta() const{
    return rutaFileManager;
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

void FileManager::setNombreDisco(const char* nombre) {
    strncpy(nombreDisco, nombre, sizeof(nombreDisco));
    nombreDisco[sizeof(nombreDisco) - 1] = '\0';
}


void FileManager::guardarInfoEnSector0(){
    //voy a guardar nombre, bInicio=0, inicioFileManager=15, tamFileManager.
    //Ir al sector0.txt y mover el puntero del archivo al byte=15.
    //En ese byte guardar lo siguiente nombre#bInicio#inicioFileManager#tamFileManager;
    //cada uno de estos campos tiene un espacio definido especifico: 
    //nombre=20bytes, bInicio=9bytes, inicioFileManager=9bytes, tamFileManager=9bytes.
    //Asi aunque le nombre sea fileManager el siguiente campo iniciara luego de estos 20 bytes separados para nombre y luego su separador #.
    //Y entonces el tamaño total de por decir este registro seria de tTotalRegistro=20+1+9+1+9+1+9=50bytes.
    char cwd[256] = {0};
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return;
    }

    char rutaSector[512] = {0};
    snprintf(rutaSector, sizeof(rutaSector), "%s/%s/Plato_0/Pista_0A/Sector_0.txt", cwd, nombreDisco);
    
    int fd = open(rutaSector, O_WRONLY);
    if (fd == -1) {
        perror("Error al abrir sector0.txt");
        return;
    }

    // Mover puntero al byte 15 dentro del sector0
    if (lseek(fd, 15, SEEK_SET) == -1) {
        perror("Error en lseek");
        close(fd);
        return;
    }

    // Preparar campos
    char nombreFormateado[tNombre+1]; // 20 + 1 por '\0' (aunque '\0' no se escribirá)
    snprintf(nombreFormateado, sizeof(nombreFormateado), "%-20s", nombre); // Alineado izquierda, padding espacios

    char bInicio[10];
    snprintf(bInicio, sizeof(bInicio), "%09d", 0);  // valor fijo 0

    char inicioFM[10];
    snprintf(inicioFM, sizeof(inicioFM), "%09d", 15);  // como especificaste
    tamFileManager=50;
    char tamFM[10];
    snprintf(tamFM, sizeof(tamFM), "%09d", tamFileManager);  // actual

    // Concatenar todo: nombre#bInicio#inicioFileManager#tamFileManager;
    char registro[51];  // 50 bytes + '\0' para seguridad
    snprintf(registro, sizeof(registro), "%s#%s#%s#%s", nombreFormateado, bInicio, inicioFM, tamFM);

    // Escribir en el archivo
    if (write(fd, registro, 50) != 50) {
        perror("Error al escribir en sector0.txt");
    }

    close(fd);  
}

bool FileManager::fMExiste(){
    //Ingresa al archivo sector
    //Verifica el byte 15, si existe fileManager ahi deberia estar la letra f
    //Si esta devuelve true, sino false
    int fd = open(getRuta(), O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir sector0.txt");
        return false;
    }

    // Mover al byte 15
    if (lseek(fd, 15, SEEK_SET) == -1) {
        perror("Error al hacer lseek");
        close(fd);
        return false;
    }
    // Leer el primer carácter del nombre
    char primerChar;
    ssize_t leido = read(fd, &primerChar, 1);
    if (leido != 1) {
        close(fd);
        return false;
    }

    close(fd);

    // Verificamos si es 'f'
    return primerChar == 'f';
}

void FileManager::recuperarInfoFM(){
    //para recuperar la info accedemos al sector con getRuta() 
    //vamos al byte15
    //Actualizaremos nada mas la variable tamFileManager
    //recorremos hasta tNombre+1+tBloqueFM+1+tInicioFM+1+1;
    //Se supone que estamos en el apartado de los 9 ultimos bytes donde se encuentra el tamFileManager anteriormente guardado.
    //Ahora tenemos que recuperarlo de esos bytes y guardarlo en la variable int tamFileManager que es de nuestra clase.  
    int fd = open(getRuta(), O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir sector0.txt");
        return;
    }

    // Mover al byte 15 donde empieza el registro
    if (lseek(fd, 15, SEEK_SET) == -1) {
        perror("Error en lseek");
        close(fd);
        return;
    }

    // Leer todo el bloque de 50 bytes
    char buffer[51] = {0};  // 50 bytes + '\0' por seguridad
    if (read(fd, buffer, 50) != 50) {
        perror("Error al leer registro del fileManager");
        close(fd);
        return;
    }

    close(fd);

    // Extraer tamFileManager desde los últimos 9 bytes del buffer
    // Formato: nombre[20]#bInicio[9]#inicioFM[9]#tamFileManager[9]
    // Posiciones: 0-19, 20, 21-29, 30, 31-39, 40, 41-49

    char tamStr[10] = {0};  // 9 dígitos + '\0'
    strncpy(tamStr, buffer + 41, 9);

    tamFileManager = atoi(tamStr);  // Actualizamos el atributo

}