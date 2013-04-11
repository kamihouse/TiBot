#include "tios.cpp"
#include "tlex.h"



int main (int argc, char** argv){
	if ( argc <= 1 ){
		printf("use: %s class\n", argv[0]);
		return 1;
	}

	string classe = Arg_get(argc, argv, "AKK", "");	

	unsigned nivel = 0;
	TiFolder folder;

	Tlex lex;	
	lex.addDiv('/');	
	lex.load( folder.getRealPath() );
	while ( !lex.isEOF() ){
		Token t;
		lex.next(&t);
		nivel++;
	}

	unsigned i;
	string path = "";
	for (i=nivel; i>0; i--){
		folder.setPath(path);
		if ( folder.isa(classe) ){
			break;
		}
		path = "../" + path; 
	}
	if ( i != 0)
		cout << folder.getRealPath() << endl;

	return 0;
}
