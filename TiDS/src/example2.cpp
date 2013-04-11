#include <iostream>
#include "tids.h"

using namespace std;


int main (){
	TiDS ti;
	ti.load("deutsch.tds");
	

	ti.enter(ti["tieraaa"]);
	ti.write();

	return 0;
}


