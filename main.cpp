#include <iostream>
#include <string.h>
#include <fstream>
#include <fcntl.h>    
#include <unistd.h>
#define MAX_PATH_LENGTH 1024
#define SECTOR_SIZE 512

using namespace std;

char* rutaActual(){
    char* currentDir= (char*)malloc(MAX_PATH_LENGTH);
    if (getcwd(currentDir, MAX_PATH_LENGTH) == NULL) {
        perror("Error al obtener el directorio actual");
        free(currentDir);
        return "";
    }
    if (strlen(currentDir) > 0 && currentDir[strlen(currentDir) - 1] != '/') {
        strcat(currentDir, "/");
    }
    return currentDir;
}

char* rutaDirecciones(){
    char* current_dir = rutaActual();
    strcat(current_dir,"direcciones.csv");
    return current_dir;
}

void agregarDirInicial(int fd, char*dirArchivo,char* rDefecto){
    char ruta2[MAX_PATH_LENGTH] = {0};
    char* dirActual= rutaActual();
    strcat(ruta2, rDefecto);
    strcat(ruta2, "#");
    strcat(ruta2, dirActual);
    strcat(ruta2, rDefecto);
    strcat(ruta2, ".csv");
    strcat(ruta2, "\r\n");
    cout<<"RagregarDirIni: "<<rDefecto<<endl;
    write(fd,ruta2,strlen(ruta2));
    free(dirActual);
}

void confDirecciones(){
    char* rDirecciones = rutaDirecciones();
    int fd=open(rDirecciones, O_WRONLY | O_TRUNC);

    //char rDefecto[] ="esquema";
    agregarDirInicial(fd,rDirecciones, "esquema");
    agregarDirInicial(fd,rDirecciones, "students");
    agregarDirInicial(fd,rDirecciones, "respuesta");
    close(fd);
    free(rDirecciones);

}
//Para ver si la clave que busco esta en el buffer y retorno el valor(ruta)
int validarPrimerCampo(char* buffer, char* relacion,ssize_t bytesLeidos){
    int i=0; 
    bool igualdad;
    do{
        igualdad=true;
        for(int k=0; buffer[i]!='#';i++,k++){
            if(buffer[i]!=relacion[k])
                igualdad=false;
        }
        i++;
        if(igualdad){//Si se encuentra el campo
            break;
        }
        for(; buffer[i]!='\n';i++){}
        i++;
    }while(i<bytesLeidos);
    if(igualdad){//devuelve la ruta(segundo campo)
        return i;
        
    }else{
        return -1;
    }
}
//Para copiar informacion desde una posicion del buffer
void copiarInfo(int* i, char* buff, char* contSalida, char separador){
    int j;
    for(j=0; buff[*i]!=separador;(*i)++,j++){
        contSalida[j]=buff[*i];
    }
    *i=j;

}


//Para acceder a traves de direciones.csv
bool accederARuta(char* relacion,char* rutaRel){
    int validacion;
    ssize_t bytesValidos;
    char buff[SECTOR_SIZE] = {0};
    char* rDirecciones = rutaDirecciones();

    int fd=open(rDirecciones,O_RDONLY); // Abriendo direcciones.csv
    
    while(bytesValidos= read(fd,buff,SECTOR_SIZE)){
        if(bytesValidos == -1) {
            perror("Error al leer archivo");
            break; // Salir del bucle en caso de error de lectura
        }
        if((validacion= validarPrimerCampo(buff, relacion, bytesValidos)) >=0){
            copiarInfo(&validacion, buff, rutaRel,'\r');
            break;
        }
    }
    free(rDirecciones);
    close(fd);
    if(validacion==-1){
        cout<<"No se encontro la relacion."<<endl;
        return false;
    }else{ //Listo para añadir Campos a respuesta
        cout<<rutaRel<<endl;
        return true;
    }
}

void leerInfo(char* buff,int inicio, ssize_t fin){
  for(; inicio<fin; inicio++){
    cout<<buff[inicio];
  }
  cout<<endl;
}

void construirCabeceraDesdeAtributos(const char* atributos, char* cabecera) {
    int citerador = 0, contMichi = 0;
    for(int i = 0; atributos[i] != '\n'; i++) {
        if(atributos[i] == '#') contMichi++;
        if(contMichi % 2 == 0) {
            cabecera[citerador++] = atributos[i];
        }
    }
    cabecera[citerador] = '\n';
}

void escribirCabeceraEnRespuesta(const char* cabecera, int longitud) {
    char rutaRespuesta[SECTOR_SIZE] = {0};
    accederARuta("respuesta", rutaRespuesta);
    int fd = open(rutaRespuesta, O_WRONLY | O_TRUNC, 0644);
    write(fd, cabecera, longitud);
    close(fd);
}

void addCamposEsquema(char* rutaEsquema,char* relacion){
    char buff[SECTOR_SIZE] = {0};
    int validacion;
    ssize_t bytesValidos;
    int fd=open(rutaEsquema,O_RDONLY);
    //leer datos para 
    while(bytesValidos= read(fd,buff,SECTOR_SIZE)){
        if(bytesValidos == -1) {
            perror("Error al leer archivo");
            break; // Salir del bucle en caso de error de lectura
        }
        if((validacion= validarPrimerCampo(buff, relacion, bytesValidos)) >=0){
            char eAtributos[SECTOR_SIZE] = {0};
            char cabecera[SECTOR_SIZE] = {0};
            copiarInfo(&validacion, buff, eAtributos, '\n');
            eAtributos[validacion]='\n';
            close(fd);
            //Configurar la liena para respuesta.csv
            construirCabeceraDesdeAtributos(eAtributos, cabecera);
            //Escribir la linea en respuesta.csv
            escribirCabeceraEnRespuesta(cabecera, strlen(cabecera));
            break;
        }
    }
    close(fd);

}

bool accessE(char* relacion){
    bool validacion;
    char rutaEsquema[SECTOR_SIZE] = {0};
    if(accederARuta("esquema", rutaEsquema)){// 
        cout<<"Salio: "<<rutaEsquema<<endl;
        addCamposEsquema(rutaEsquema, relacion); 
    }
    return true;
}

void addRelacion(char* relacion){
    //abrir la ruta
    char* buff[SECTOR_SIZE] = {0};
    int bEscribir;
    int fd = open(relacion,O_RDONLY);

    char rutaRespuesta[SECTOR_SIZE] = {0};
    accederARuta("respuesta",rutaRespuesta);
    int fd1=open(rutaRespuesta, O_WRONLY | O_APPEND);
    while(bEscribir= read(fd,buff,SECTOR_SIZE)){
      if(bEscribir == -1) {
            perror("Error al leer archivo");
            break; // Salir del bucle en caso de error de lectura
      }
      write(fd1,buff,bEscribir);
    }
    close(fd);
    close(fd1);
}

void accessR(char* nombreArchivoR){
    //obtener ruta de relacion y abrir
    char rutaR[SECTOR_SIZE] = {0};
    if(accederARuta(nombreArchivoR, rutaR)){
      cout<<"rutaREl: "<<rutaR<<endl;
      addRelacion(rutaR);
    }
    //copiar los archivos a respuesta.csv y cerrar
    
}

void SF(char* relacion){
    if(!accessE(relacion)){
        return;
    }
    //char* nombreArchivoR = relacion + ".txt";
    accessR(relacion);
}

void addCamposYTipos(int fd,char* lEsquema){
  char buff[SECTOR_SIZE]= {0};
  char atributos[SECTOR_SIZE]= {0};
  int i, m, lEsquemaIdx = 0;
  ssize_t bLeidos= read(fd,buff,SECTOR_SIZE);
  for(i = 0;buff[i] != '\r';i++){
    atributos[i]=buff[i];
  }
  for(m=0;atributos[m]!='\0';m++){
    if(atributos[m]=='\t' || atributos[m]=='\0'){
      lEsquema[lEsquemaIdx++]='#';
      printf(" - TIPO(int,float, str, char): ");
      char tipo[10];
      cin>>tipo; 
      for(int j=0;tipo[j]!='\0';j++){
        lEsquema[lEsquemaIdx++]=tipo[j];
      }      
      lEsquema[lEsquemaIdx++]='#';
    }
    else{
      lEsquema[lEsquemaIdx++]=atributos[m];
      printf("%c",atributos[m]);
    }
  }
  lEsquema[lEsquemaIdx++]='#';
  printf(" - TIPO(int,float, str, char): ");
  char tipo2[10];
  cin>>tipo2; 
  for(int j=0;tipo2[j]!='\0';j++){
    lEsquema[lEsquemaIdx++]=tipo2[j];
  }
  if(atributos[m]=='\0'){
    lEsquema[lEsquemaIdx++]='\r';
    lEsquema[lEsquemaIdx++]='\n';
  }
}

void addDataSet(char* dataSet){
    char lCamposTipos[SECTOR_SIZE]={0};
    char* rDirecciones = rutaDirecciones();
    int fd=open(rDirecciones, O_WRONLY | O_APPEND);
    if(fd==-1){
      cout<<"EL archivo no se encuentra"<<endl;
      return;
    }
    agregarDirInicial(fd,rDirecciones, dataSet);
    close(fd);
    free(rDirecciones);
    //pedir tipo de datos 
    //acceder al archivo
    char rutaDSet[SECTOR_SIZE] = {0};
    if(accederARuta(dataSet, rutaDSet)){
        cout<<"Salio: "<<rutaDSet<<endl;
        int fd1= open(rutaDSet, O_RDONLY);
        //funcion para añadir los campos y tipos
        addCamposYTipos(fd1, lCamposTipos);
        close(fd1);
    }else{
        cout<<"Ruta no existe"<<endl;
    }
    //añadir a esquema
    char lEsquemaCompleto[SECTOR_SIZE]= {0};
    char rutaEsquema[SECTOR_SIZE]= {0};
    strcat(lEsquemaCompleto, dataSet);
    strcat(lEsquemaCompleto,"#");
    strcat(lEsquemaCompleto,lCamposTipos);
    if(accederARuta("esquema", rutaEsquema)){
        cout<<"Salio: "<<rutaEsquema<<endl;
        char buff[SECTOR_SIZE]={0};
        int fd2= open(rutaEsquema, O_RDWR | O_APPEND);
        ssize_t bLeidos=read(fd2,buff,SECTOR_SIZE);
        if(validarPrimerCampo(buff,dataSet,bLeidos)){
          cout<<"Relacion ya se encuentra en el archivo"<<endl;
          return;
        }
        //funcion para añadir los campos y tipos
        ssize_t bLidos=  write(fd2,lEsquemaCompleto,strlen(lEsquemaCompleto));
        close(fd2);
    }

 
}

int main() {
    int opcion;
    confDirecciones();
    do {
        // 1. Mostrar las opciones
        cout << "\n--- MEGATRON 3000 ---" << endl;
        cout << "1. SELECT * FROM _____" << endl;
        cout << "2. SELECT * FROM students WHERE _____" << endl;
        cout << "3. SELECT * FROM students WHERE _____ | ______ #" << endl;
        cout << "4. Anadir un dataSet" << endl;
        cout << "0. Salir" << endl;
        cout << "-------------------------" << endl;
        cout << "Ingrese su opcion: ";

        cin >> opcion;

        switch (opcion) {
            case 1:{
                char* relacion;
                cin>>relacion;
                SF(relacion);
                break;
            }
                
            case 2:
                
                break;
            case 3: {
                
                break;
            }
            case 4: {
                char* dataSet;
                cin>>dataSet;
                addDataSet(dataSet);
                break;
            }
            case 0:
                cout << "Saliendo del programa. ¡Hasta luego!" << endl;
                break;
            default:
                cout << "Opcion invalida. Por favor, intente de nuevo." << endl;
                break;
        }
    } while (opcion != 0);

    return 0;
}
