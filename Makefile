build/play: build/play.o build/player.o
	g++ build/play.o build/player.o -o build/play

build/verify: build/verify.o build/player.o
	g++ build/verify.o build/player.o -o build/verify

build/test: build/test.o build/player.o
	g++ build/test.o build/player.o -o build/test

build/play.o: play.cpp
	g++ -g -c play.cpp -o build/play.o

build/verify.o: verify.cpp
	g++ -g -c verify.cpp -o build/verify.cpp

build/player.o: player.cpp
	g++ -g -c player.cpp -o build/player.o

build/test.o: test.cpp
	g++ -g -c test.cpp -o build/test.o

clean:
	rm build/*