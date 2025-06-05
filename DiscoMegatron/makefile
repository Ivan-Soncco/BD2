programa: main.o disco.o plato.o pista.o sector.o fileManager.o logicaFunciones.o file.o
	g++ -g -Wall -std=c++11 main.o disco.o plato.o pista.o sector.o fileManager.o logicaFunciones.o file.o -o programa

main.o: main.cpp disco.h plato.h pista.h sector.h fileManager.h logicaFunciones.h
	g++ -g -Wall -std=c++11 -c main.cpp

disco.o: disco.cpp disco.h
	g++ -g -Wall -std=c++11 -c disco.cpp

plato.o: plato.cpp plato.h
	g++ -g -Wall -std=c++11 -c plato.cpp

pista.o: pista.cpp pista.h
	g++ -g -Wall -std=c++11 -c pista.cpp

sector.o: sector.cpp sector.h
	g++ -g -Wall -std=c++11 -c sector.cpp

fileManager.o: fileManager.cpp fileManager.h
	g++ -g -Wall -std=c++11 -c fileManager.cpp

logicaFunciones.o: logicaFunciones.cpp logicaFunciones.h
	g++ -g -Wall -std=c++11 -c logicaFunciones.cpp

clean:
	rm -f programa *.o
