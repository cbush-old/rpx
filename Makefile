all:
	g++ -std=c++11 *.cpp -o rpmd;

install:
	mv rpmd /usr/bin/rpmd

