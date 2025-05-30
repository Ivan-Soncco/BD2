#ifndef FILE_H
#define FILE_H

class File{
    private:
        //El file que contenga la lista de archivos.
        char nombre[50];
        int bloque;
        int inicioFile;
        int tamFile;
        //int tamRegistro;
    public:
    File(const char* nombre_,int bloque_,int inicioFile_,int tamFile_);
    ~File(){}
    //registro;
    void crearArchivo();
    void setNombre(const char* nom);
    void setBloque(int b);
    void setInicioFile(int inicio);
    void setTamFile(int tam);

    const char* getNombre() const;
    int getBloque() const;
    int getInicioFile() const;
    int getTamFile() const;
};


#endif