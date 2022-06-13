888851.tar.gz: player.cpp
	mkdir 888851
	cp player.cpp 888851
	tar -zcvf 888851.tar.gz 888851
	rm -r 888851

build/play: build/play.o build/player.o
	g++ build/play.o build/player.o -o build/play

build/verify: build/verify.o build/player.o
	g++ build/verify.o build/player.o -o build/verify

build/test: test.cpp player.cpp
	g++ -g test.cpp -o build/test

build/play.o: play.cpp
	g++ -g -c play.cpp -o build/play.o

build/verify.o: verify.cpp
	g++ -g -c verify.cpp -o build/verify.cpp

build/player.o: player.cpp
	g++ -g -c player.cpp -o build/player.o

clean:
	rm build/*
	rm 888851.tar.gz