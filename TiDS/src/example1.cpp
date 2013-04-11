#include <iostream>
#include "tids.h"

using namespace std;


int main (int argc, char** argv){
	TiDS ti;
	ti.load(argv[1]);
	ti.write();

	/**TiConj nouns;
	ti.getConj(&nouns, "Noun");
	nouns.filter("genero = masculino");
	nouns.write();*/

	return 0;
}


