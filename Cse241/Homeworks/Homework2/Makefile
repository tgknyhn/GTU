play: play.o hexgame.o
	g++ -std=c++11 play.o hexgame.o -o play && ./play

play.o: play.cpp
	g++ -std=c++11 -c play.cpp

hexgame.o: hexgame.cpp hexgame.h
	g++ -std=c++11 -c hexgame.cpp

clean:
	rm *.o
