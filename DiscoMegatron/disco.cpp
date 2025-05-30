  #include <iostream>
  #include "disco.h"
  #include "logicaFunciones.h"
  #include "file.h"
  #include <sys/stat.h>
  #include <cstdio>
  #include <unistd.h>
  #include <fcntl.h>
  #include <string.h>

  using namespace std;

  Disco::Disco(const char* nombre_, int tSector_, int nSectoresXP_, int nPistas_, int nPlatos_)
      : tSector(tSector_),
        nSectoresXPista(nSectoresXP_),
        nPistas(nPistas_),
        nPlatos(nPlatos_),
        fileManager()
  {
      strncpy(nombreDisco, nombre_, sizeof(nombreDisco));
      nombreDisco[sizeof(nombreDisco) - 1] = '\0';
      nSectoresXBloque = nPlatos * 2;
      tTotal = tSector_ * nSectoresXP_ * nPistas_ * nPlatos_ * 2;
      crearEstructuraDisco();
  }

  Disco Disco::crearDinamicamente() {
      char nombre[50];
      int tSector, nSectoresXP, nPistas, nPlatos;

      std::cout << "Ingrese nombre del disco: ";
      std::cin >> nombre;

      std::cout << "Ingrese tamaño del sector: ";
      std::cin >> tSector;

      std::cout << "Ingrese número de sectores por pista: ";
      std::cin >> nSectoresXP;

      std::cout << "Ingrese número de pistas: ";
      std::cin >> nPistas;

      std::cout << "Ingrese número de platos: ";
      std::cin >> nPlatos;

      return Disco(nombre, tSector, nSectoresXP, nPistas, nPlatos);
  }

  void Disco::crearEstructuraDisco() {
      if (access(nombreDisco, F_OK) == 0) {
          std::cout << "El disco '" << nombreDisco << "' ya existe. No se crea nuevamente.\n";
          return;
      }
      mkdir(nombreDisco, 0777);
      irACarpeta(nombreDisco);

      for (int i = 0; i < nPlatos; i++) {
          crearPlato(i);
      }
      volverACarpeta("..");
      guardarMetadata();
      configurarFileManager();
  }

  void Disco::crearPlato(int indexPlato) {
      char nombreCarpeta[50];
      sprintf(nombreCarpeta, "plato%d", indexPlato + 1);

      if (mkdir(nombreCarpeta, 0777) == 0) {
          std::cout << "Carpeta creada: " << nombreCarpeta << std::endl;
      } else {
          perror("Error al crear carpeta");
      }

      char carpetaAnterior[100];
      getcwd(carpetaAnterior, sizeof(carpetaAnterior));
      irACarpeta(nombreCarpeta);

      for (int j = 0; j < nPistas; ++j) {
          crearPista("A", j);
          crearPista("B", j);
      }

      volverACarpeta(carpetaAnterior);
  }

  void Disco::crearPista(const char* tipoPista, int indexPista) {
      char nombrePista[50];
      sprintf(nombrePista, "pista%d%s", indexPista + 1, tipoPista);

      if (mkdir(nombrePista, 0777) == 0) {
          std::cout << "Carpeta creada: " << nombrePista << std::endl;
      } else {
          perror("Error al crear Pista");
      }

      char carpetaAnterior[100];
      getcwd(carpetaAnterior, sizeof(carpetaAnterior));
      irACarpeta(nombrePista);

      for (int k = 0; k < nSectoresXPista; ++k) {
          char nombreSector[20];
          sprintf(nombreSector, "sector%d.csv", k);
          crearSector(nombreSector);
      }

      volverACarpeta(carpetaAnterior);
  }

  void Disco::crearSector(const char* nombreSector) {
      int fd = open(nombreSector, O_WRONLY | O_CREAT, 0666);
      if (fd == -1) {
          perror("Error al crear archivo");
          return;
      }

      char relleno[tSector];
      memset(relleno, ' ', sizeof(relleno));
      write(fd, relleno, sizeof(relleno));
      close(fd);
  }

  void Disco::irACarpeta(const char* nombre) {
      if (chdir(nombre) != 0) {
          perror("Error al cambiar de directorio");
      }
  }

  void Disco::volverACarpeta(const char* pathAnterior) {
      if (chdir(pathAnterior) != 0) {
          perror("Error al volver al directorio anterior");
      }
  }

  const char* Disco::getNombreDisco() const {
    return nombreDisco;
    }

  void Disco::guardarMetadata() {
      char path[200];
      sprintf(path, "%s/plato1/pista1A/sector0.csv", nombreDisco);

      int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
      if (fd == -1) {
          perror("Error al abrir archivo de metadata para escribir");
          return;
      }

      char buffer[300];
      int offset = 0;

      //agrego metadatos de DIsco
      int written1 = snprintf(buffer + offset, sizeof(buffer) - offset,
                              "%s#%d#%d#%d#%d#%d\n",
                              nombreDisco, tSector, nSectoresXPista,
                              nSectoresXBloque, nPistas, nPlatos);
      offset += written1;

      //agrego matadatos de fileManager
      int written2 = snprintf(buffer + offset, sizeof(buffer) - offset,
                              "%s\n", fileManager.getNombre());
      offset += written2;

      // Establecer inicio justo después del segundo '\n'
      fileManager.setInicio(offset);
      cout<<"Inicio de fileM: "<<fileManager.getInicio();
      // Escribir todo al archivo
      write(fd, buffer, offset);
      close(fd);
  }

  Disco Disco::leerMetadata(const char* nombreDiscoInput) {
      char path[200];
      sprintf(path, "%s/plato1/pista1A/sector0.csv", nombreDiscoInput);

      int fd = open(path, O_RDONLY);
      if (fd == -1) {
          perror("Error al abrir archivo de metadata para lectura");
          return Disco("error", 0, 0, 0, 0);
      }

      char buffer[200];
      ssize_t bytesLeidos = read(fd, buffer, sizeof(buffer) - 1);
      close(fd);

      if (bytesLeidos <= 0) {
          perror("Error al leer metadata");
          return Disco("error", 0, 0, 0, 0);
      }

      buffer[bytesLeidos] = '\0';

      char nombre[50];
      int tSector, nSectoresXP, nSectoresXB, nPistas, nPlatos;

      int ok = sscanf(buffer, "%49[^#]#%d#%d#%d#%d#%d",
                      nombre, &tSector, &nSectoresXP, &nSectoresXB, &nPistas, &nPlatos);

      if (ok != 5) {
          std::cerr << "Error al parsear metadata\n";
          return Disco("error", 0, 0, 0, 0);
      }

      return Disco(nombre, tSector, nSectoresXP, nPistas, nPlatos);
  }

  FileManager& Disco::getFileManager() {
    return fileManager;
}

void Disco::configurarFileManager(){
    //agregar al final del archivo 
    char rutaActual[100]={0};
    getcwd(rutaActual, sizeof(rutaActual));
    char ruta[100] = {0};
    snprintf(ruta, 100, "%s/%s/",rutaActual, getNombreDisco());
    crearRutaPlatoPistaSector("1","1A","0",ruta);
    fileManager.setRuta(ruta);
    File* ffileManager= new File(fileManager.getNombre(),0,fileManager.getInicio(),fileManager.getTam());
    fileManager.añadirFileAFileManager(ffileManager);
}