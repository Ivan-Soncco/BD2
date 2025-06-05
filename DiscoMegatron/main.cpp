#include <iostream>
#include "disco.h"
#include "logicaFunciones.h"

using namespace std;

int main(){
    Disco* discoActual = nullptr;
    bool salir = false;

    while (!salir) {
        cout << "\n--- MENÚ PRINCIPAL ---\n";
        cout << "1. Crear un nuevo disco\n";
        cout << "2. Usar un disco ya creado\n";
        cout << "3. Añadir un File\n";
        cout << "4. Insertar un registro\n";
        cout << "5. Mostrar información del disco\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opción: ";

        int opcion;
        cin >> opcion;

        switch (opcion) {
            case 1: {
                delete discoActual;  // borrar disco anterior si lo hubiera

                char nombreDisco[50];
                cout << "Ingrese el nombre del nuevo disco: ";
                cin >> nombreDisco;

                // Creo disco con nombre 
                discoActual = new Disco(nombreDisco);

                cout << "Disco creado correctamente.\n";
                break;
            }

            case 2: {
                char nombreDisco[50];
                cout << "Ingrese el nombre del disco existente: ";
                cin >> nombreDisco;
                /*
                char nombre[50];
                int tSector;
                int nSectoresP;
                int nSectoresB;
                int nPistas;
                int nPlatos;
                logicaCargarDatosDesdeMetadata(nombreDisco, nombre, &tSector, &nSectoresP, &nSectoresB, &nPistas, &nPlatos);
                */
                delete discoActual;
                discoActual = new Disco(nombreDisco);
                //cout << "Se usará el disco '" << nombreDisco << "' con parámetros por defecto.\n";
                break;
            }

            case 3: {
                char nombreFile[50];
                cout << "Ingrese el nombre del File que quiere insertar: ";
                cin >> nombreFile;
                discoActual->addFile(nombreFile);
                break;
            }

            case 4:
                break;

            case 5:
                if (discoActual != nullptr) {
                    discoActual->mostrarInformacion();
                } else {
                    std::cout << "Primero debe crear o seleccionar un disco.\n";
                }
                break;

            case 0:
                salir = true;
                break;

            default:
                cout << "Opción inválida. Intente nuevamente.\n";
                break;
        }
    }

    delete discoActual;
    return 0;
}
