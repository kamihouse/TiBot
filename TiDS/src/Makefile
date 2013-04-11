all: tlex.o tids.o example1 example2
	cp *.h ../include/

example1: tlex.o tids.o
	g++ example1.cpp ../lib/tlex.o ../lib/tids.o -o ../bin/example1

example2: tlex.o tids.o
	g++ example2.cpp ../lib/tlex.o ../lib/tids.o -o ../bin/example2

tlex.o: tlex.h tlex.cpp
	g++ -c tlex.cpp -o ../lib/tlex.o

tids.o: tids.h tids.cpp
	g++ -c tids.cpp -o ../lib/tids.o
