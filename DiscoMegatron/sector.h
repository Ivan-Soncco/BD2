#ifndef SECTOR_H
#define SECTOR_H

class Sector {
private:
    char nombre[30];
    int tamano;

public:
    Sector(const char* nombre_, int tamanoSector);
    const char* getNombre() const;
    int getTamano() const;
};

#endif
