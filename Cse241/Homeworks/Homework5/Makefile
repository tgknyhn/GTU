compile: testRun.o GlobalFunctions.o HexArray1D.o HexVector.o AbstractHex.o 
	g++ -std=c++11 testRun.o GlobalFunctions.o HexArray1D.o HexVector.o AbstractHex.o -o RunThis && ./RunThis

testRun.o: testRun.cpp
	g++ -std=c++11 -c testRun.cpp

GlobalFunctions.o: GlobalFunctions.cpp GlobalFunctions.h
	g++ -std=c++11 -c GlobalFunctions.cpp

HexArray1D.o: HexArray1D.cpp HexArray1D.h
	g++ -std=c++11 -c HexArray1D.cpp

HexVector.o: HexVector.cpp HexVector.h
	g++ -std=c++11 -c HexVector.cpp

AbstractHex.o: AbstractHex.cpp AbstractHex.h
	g++ -std=c++11 -c AbstractHex.cpp

clean:
	rm *.o