#ifndef TLEX_H
#define TLEX_H

#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

class Token {
	public:
	string value;
	int type;

	const static int TYPE_STRING = 1;
	const static int TYPE_SYMBOL = 2;
	const static int TYPE_INT = 3;
	const static int TYPE_FLOAT = 4;
	const static int TYPE_SEGMENT = 5;
	const static int TYPE_BLOCK = 6;

	int isString();
	int isSymbol();
	int isInt();
	int isFloat();
	int isSegment();
	int isBlock();

	int getInt();
	double getDouble();

	void write();
};


class Tlex {
	typedef struct {
		char open;
		char close;
	} SBlock;

	string text;
	int cur;
	vector<char> divisors;
	vector<char> symbols;
	vector<SBlock> blocks;

	public:
	void load(string text);
	int loadFile(string filename);

	void addDiv (char c);
	void addSymbol (char c);
	void addBlock (char open, char close);

	void next(Token* tok);
	Token* next();
	void prox(Token* tok);

	int isEOF();


	private:
	int isaDivisor (char c);
	int isaSymbol (char c);
	int isaBlockOpen (char c);
	int isaBlockClose (char c);
	char getBlockClose (char open);
};



#endif
