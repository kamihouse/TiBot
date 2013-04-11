#include <iostream>
#include "tisys.h"

using namespace std;


int main (int argc, char** argv){
	string path = "";
	if ( argc > 1){
		path = argv[1];
	} else {
		path = ".";
	}

	TiFolder folder(path);
	cout << folder.type << endl;
	return 0;
}
