#include "disco.h"
//#include "file.h"
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
    nPistasXPlato = nPistas;
    nSectoresXPista = nSectores;
    tSector = tamSector;
    tBloque = tSector * nPlatosXDisco * 2;

    // Inicializar variables para navegación cilíndrica
    platoActual = 0;
    pistaActual = 0;
    ladoActual = 'A';
    sectorActual = nSectores - 1; // Empezar desde el sector máximo (saltando el sector 0)

    std::cout << "* " << nombre << std::endl;
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
    fileManager = new FileManager(nombreDisco);
}


void Disco::guardarInformacionDisco(const char* nombreDisco, int nPlatos, int nPistas, int nSectores, int tamSector) {
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return;
    }

    char rutaSector[512];
    snprintf(rutaSector, sizeof(rutaSector), "%s/%s/Plato_0/Pista_0A/Sector_0.txt", cwd, nombreDisco);

    int fd = open(rutaSector, O_WRONLY , 0644);
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
}

void Disco::addFile(char* nombreFile){
    //abrir con el nombre del archivo.
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    std::cout << "Directorio actual: " << cwd << std::endl;
    int fd = open(nombreFile, O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir el archivo para añadirlo al disco");
        return;
    }
    //añadir al disco con logica de bloques
    char buffer[tBloque];
    ssize_t bytesLeidos;

    while ((bytesLeidos = read(fd, buffer, sizeof(buffer))) > 0) {
        // Aquí deberías escribir `bytesLeidos` bytes del `buffer` en los sectores del disco virtual
        // Por ejemplo: this->escribirEnSector(buffer, bytesLeidos);
        anadirADisco(buffer, bytesLeidos);
    }

    if (bytesLeidos == -1) {
        perror("Error al leer archivo");
    }

    close(fd);
}

void Disco::anadirADisco(char* buff, ssize_t bleidos) {
    char rutaSector[512];
    ssize_t bEscritosTotales = 0;

    while (bEscritosTotales < bleidos) {
        configurarRutaSector(rutaSector);

        int fd = open(rutaSector, O_WRONLY | O_TRUNC); // Limpiamos el sector antes de escribir
        if (fd == -1) {
            perror("Error al abrir sector");
            return;
        }

        ssize_t restantes = bleidos - bEscritosTotales;
        ssize_t porEscribir = std::min((ssize_t)tSector, restantes);

        std::cout << "Escribiendo en: " << rutaSector << std::endl;
        ssize_t escritos = write(fd, buff + bEscritosTotales, porEscribir);
        if (escritos == -1) {
            perror("Error al escribir en sector");
            close(fd);
            return;
        }

        bEscritosTotales += escritos;
        close(fd);
    }
}

void Disco::configurarRutaSector(char* rutaSector) {
    // Construir el nombre de la pista
    char nombrePista[30];
    snprintf(nombrePista, sizeof(nombrePista), "Pista_%d%c", pistaActual, ladoActual);

    // Construir la ruta del sector actual
    snprintf(rutaSector, 512, "%s/Plato_%d/%s/Sector_%d.txt", nombreDisco, platoActual, nombrePista, sectorActual);

    // Avanzar al siguiente plato en el mismo sector/pista/lado
    ++platoActual;

    // Si hemos recorrido todos los platos en este sector/pista/lado
    if (platoActual >= nPlatosXDisco) {
        platoActual = 0;
        
        if (ladoActual == 'A') {
            // Cambiar al lado B de la misma pista y mismo sector
            ladoActual = 'B';
        } else {
            // Ya terminamos lado A y B, ahora decrementar sector
            ladoActual = 'A';
            --sectorActual;
            
            if (sectorActual <= 0) {
                // Ya terminamos todos los sectores (5,4,3,2,1), pasar a siguiente pista
                sectorActual = nSectoresXPista - 1; // Reiniciar al sector máximo
                ++pistaActual;
                
                if (pistaActual >= nPistasXPlato) {
                    // Hemos completado todas las pistas, reiniciar
                    pistaActual = 0;
                    
                    // Verificar si hemos usado todos los sectores (opcional: detección de disco lleno)
                    static int ciclosCompletos = 0;
                    ciclosCompletos++;
                    if (ciclosCompletos > 2) { // Permitir algunos ciclos antes de declarar disco lleno
                        std::cerr << "Advertencia: Disco casi lleno, reescribiendo sectores." << std::endl;
                    }
                }
            }
        }
    }
}