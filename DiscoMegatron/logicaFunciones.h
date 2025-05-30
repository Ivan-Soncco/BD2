#ifndef LOGICAFUNCIONES_H
#define LOGICAFUNCIONES_H

#include "disco.h"

bool logicaValidarDiscoExistente(const char* nombreDisco);
void logicaCargarDatosDesdeMetadata(const char* nombreDisco, char* nombre, int* tSector, int* nSectoresP, 
                                    int* nSectoresB, int* nPistas, int* nPlatos);
void crearRutaPlatoPistaSector(char* plato,char* pista,char* sector,char* ruta);

void obtenerRutafileManager(char* rutafileManager);
void escribirCampo(int fd, const void* campo, const char* tipo, int tamaño);
void escribirCampoBinario(int fd, const void* campo, const char* tipo, int tamaño);
#endif
