play: play.o hexgame.o
	g++ play.o hexgame.o -o play && ./play

play.o: play.cpp
	g++ -c play.cpp

hexgame.o: hexgame.cpp hexgame.h
	g++ -c hexgame.cpp

clean:
	rm *.o
