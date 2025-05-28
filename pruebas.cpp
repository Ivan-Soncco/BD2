#include <iostream>
#include <string.h>
#include <fstream>
#include <fcntl.h>    
#include <unistd.h>
#define MAX_PATH_LENGTH 1024
#define SECTOR_SIZE 512

using namespace std;

const char* inferir_tipo(const char* campo) {
    if (campo[0] == '\0') return "VACIO";

    char* endptr;
    long val_entero = strtol(campo, &endptr, 10);
    if (*endptr == '\0') return "int";

    double val_float = strtod(campo, &endptr);
    if (*endptr == '\0') return "float";

    if (strlen(campo) == 1) return "char";

    return "str";
}

void procesar_linea_y_mostrar_tipos(const char* linea, ssize_t len, char* lTipos) {
    char campo[SECTOR_SIZE] = {0};  
    int campo_idx = 0;
    int salida_idx = 0;

    for (int i = 0; i < len && linea[i] != '\r' && linea[i] != '\n'; i++) {
        if (linea[i] == '\t') {
            campo[campo_idx] = '\0';
            const char* tipo = inferir_tipo(campo);
            
            for (int k = 0; tipo[k] != '\0'; k++) {
                lTipos[salida_idx++] = tipo[k];
            }

            // Añadir tabulador
            lTipos[salida_idx++] = '\t';

            campo_idx = 0;
            campo[0] = '\0';
        } else {
            if (campo_idx < SECTOR_SIZE - 1)
                campo[campo_idx++] = linea[i];
        }
    }

    // Procesar último campo
    campo[campo_idx] = '\0';
    const char* tipo = inferir_tipo(campo);
    for (int k = 0; tipo[k] != '\0'; k++) {
        lTipos[salida_idx++] = tipo[k];
    }
    lTipos[salida_idx] = '\0';
}

int main(){
  char buff[SECTOR_SIZE] = {0};
  char tipos[SECTOR_SIZE] = {0};
  char atributos[SECTOR_SIZE] = {0};
  char lTipos[SECTOR_SIZE] = {0};
  char lEsquema[SECTOR_SIZE] = {0};
  int i,atributosIdx,lEsquemaIdx;
  int fd = open("titanic.tsv",O_RDONLY);
  ssize_t bLeidos= read(fd,buff,SECTOR_SIZE);
  for(i = 0, atributosIdx=0;buff[i] != '\r';i++){
    atributos[i]=buff[i];
  }
  i++;
  i++;
  printf("%c\n",buff[i]);
  //ir hasta \r
  procesar_linea_y_mostrar_tipos(&buff[i], bLeidos - i, lTipos);  
  lEsquemaIdx=0;
  int m=0;
  int n=0;
  while(atributos[m]!='\0' || lTipos[n]!='\0'){
    for(;atributos[m]!='\t' && atributos[m]!='\0';m++){
      lEsquema[lEsquemaIdx++]= atributos[m];
    }
    if(atributos[m]!= '\0')
      m++;
    lEsquema[lEsquemaIdx++]='#';
    for(;lTipos[n]!='\t' && lTipos[n]!='\0';n++){
      lEsquema[lEsquemaIdx++]= lTipos[n];
    }
    if(lTipos[n]=='\0'){
      lEsquema[lEsquemaIdx++]='\r';
      lEsquema[lEsquemaIdx++]='\n';
    }else{
      lEsquema[lEsquemaIdx++]='#';
      n++;
    }
  }
  return 0;
}
