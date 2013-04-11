all:
	cd src; make;
	cd scripts; make;

install:
	cp bin/* ~/bin/
