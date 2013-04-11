#include "tisys.h"


int main (int argc, char** argv){
	string classe = Arg_get(argc, argv, "AKK", "");
	string local = Arg_get(argc, argv, "from", ".");
	TiFolder folder(local);
	folder.load(classe);
	folder.write();
}
