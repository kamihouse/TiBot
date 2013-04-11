

DEST="../bin/"


all: tils ticlass xml install


ticlass: ticlass.cpp tisys.o
	g++ ticlass.cpp tisys.o tlex.o -o ticlass -lxml2 -I /usr/include/libxml2

tils: tils.cpp tisys.o
	g++ tils.cpp tisys.o tlex.o -o tils -lxml2 -I /usr/include/libxml2


xml:
	gcc -Wall xmlread.c -o xmlread -lxml2 -I /usr/include/libxml2
	gcc -Wall xmlwrite.c -o xmlwrite -lxml2 -I /usr/include/libxml2


tisys.o: tisys.cpp tisys.h
	g++ -c tisys.cpp -o tisys.o -lxml2 -I /usr/include/libxml2


install:
	cp xmlread $(DEST);
	cp xmlwrite $(DEST);
	cp tils $(DEST);
	cp ticlass $(DEST);
