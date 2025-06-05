# Compilar main.cpp
main.o: main.cpp disco.h logicaFunciones.h
	g++ -g -Wall -std=c++11 -c main.cpp

# Compilar disco.cpp
disco.o: disco.cpp disco.h
	g++ -g -Wall -std=c++11 -c disco.cpp

# Compilar plato.cpp
plato.o: plato.cpp plato.h
	g++ -g -Wall -std=c++11 -c plato.cpp

# Compilar platoManager.cpp
platoManager.o: platoManager.cpp platoManager.h plato.h
	g++ -g -Wall -std=c++11 -c platoManager.cpp

# Compilar pista.cpp
pista.o: pista.cpp pista.h
	g++ -g -Wall -std=c++11 -c pista.cpp

# Compilar pistaManager.cpp
pistaManager.o: pistaManager.cpp pistaManager.h pista.h
	g++ -g -Wall -std=c++11 -c pistaManager.cpp

# Compilar sector.cpp
sector.o: sector.cpp sector.h
	g++ -g -Wall -std=c++11 -c sector.cpp

# Compilar sectorManager.cpp
sectorManager.o: sectorManager.cpp sectorManager.h
	g++ -g -Wall -std=c++11 -c sectorManager.cpp

# Compilar logicaFunciones.cpp
logicaFunciones.o: logicaFunciones.cpp logicaFunciones.h
	g++ -g -Wall -std=c++11 -c logicaFunciones.cpp

# Enlazar todo en el ejecutable
programa: main.o disco.o plato.o platoManager.o pista.o pistaManager.o sector.o sectorManager.o logicaFunciones.o
	g++ -g -Wall -std=c++11 -o programa main.o disco.o plato.o platoManager.o pista.o pistaManager.o sector.o sectorManager.o logicaFunciones.o

# Limpiar archivos objeto y el ejecutable
.PHONY: clean programa
clean:
	rm -f *.o programa
