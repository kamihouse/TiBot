#include "tlex.h"
#include <stdio.h>
#include <stdlib.h>

int Token::isString(){
	return type == TYPE_STRING;
}

int Token::isSymbol(){
	return type == TYPE_SYMBOL;
}
	
int Token::isInt(){
	return type == TYPE_INT;
}

int Token::isFloat(){
	return type == TYPE_FLOAT;
}

int Token::isSegment(){
	return type == TYPE_SEGMENT;
}

int Token::isBlock(){
	return type == TYPE_BLOCK;
}

int Token::getInt(){
	return atoi(value.c_str());
}

double Token::getDouble(){
	return atof(value.c_str());
}


void Token::write(){
	cout << value << "(" << type << ")\n"; 
}




void Tlex::load(string text){
	cur = 0;
	this->text = text;
}

int Tlex::loadFile(string filename){
	static char buffer[16*1024];
	FILE *fp = fopen(filename.c_str(), "r");
	if ( !fp )
		return 0;

	int size = fread(buffer, 1, 16*1024, fp);
	buffer[size] = '\0';
	text = buffer;
	cur = 0;
	fclose(fp);
}

void Tlex::addDiv (char c){
	divisors.push_back(c);
}

void Tlex::addSymbol (char c){
	symbols.push_back(c);
}

void Tlex::addBlock (char open, char close){
	SBlock aux;
	aux.open = open;
	aux.close = close;
	blocks.push_back(aux);
}

void Tlex::next(Token* tok){
	char c;
	tok->type = 0;
	tok->value = "";

	while ( true ){
		if ( isEOF() )
			return ; 	
		c = text[cur];
		if ( !isaDivisor(c) )
			break;
		cur++;
	}

	int level = 0;
	char first = c;
	char bopen, bclose;
	if ( first >= '0' && first <= '9' ){
		tok->value += first;
		tok->type = Token::TYPE_INT;
	} else if ( first == '\"' || first == '\''){
		tok->type = Token::TYPE_SEGMENT;
	} else if ( isaBlockOpen(first) ){
		bopen = first;
		bclose = getBlockClose (first);
		tok->type = Token::TYPE_BLOCK;
		level++;
	} else if ( isaSymbol(first) ){
		tok->value += first;
		tok->type = Token::TYPE_SYMBOL;
		cur++;
		return;
	} else {
		tok->value += first;
		tok->type = Token::TYPE_STRING;
	}
	cur++;

	while ( !isEOF() ){
		char c = text[cur];
		if ( tok->type == Token::TYPE_SEGMENT ){
			if ( c == '\"' || c == '\'' ){
				cur++; break;
			}
		} else if ( tok->type == Token::TYPE_BLOCK ){
			if ( c == bopen )
				level++;
			else if ( c == bclose ){
				level--;
				if ( level <= 0 ){
					cur++; break;
				}
			}
		} else {
			if ( isaBlockOpen(c) || isaBlockClose(c) || c == '\'' || c == '\"' )
				break;
			if ( isaDivisor(c) )
				break;
			if ( isaSymbol(c) ) // symbol
				break;
			if ( tok->type == Token::TYPE_INT && c == '.' )
				tok->type = Token::TYPE_FLOAT;
		}
		tok->value += c;
		cur++;
	}

}

Token* Tlex::next(){
	Token *aux = new Token();
	next(aux);
	return aux;
}

void Tlex::prox(Token* tok){
	int save = this->cur;
	this->next(tok);
	this->cur = save;
}

int Tlex::isEOF(){
	return cur >= this->text.length();
}


int Tlex::isaDivisor (char c){
	int i;
	for (i=0; i<divisors.size(); i++){
		if ( c == divisors[i] ){
			return true;
		}
	}
	return false;
}

int Tlex::isaSymbol (char c){
	int i;
	for (i=0; i<symbols.size(); i++){
		if ( c == symbols[i] )
			return true;
	}
	return false;
}

int Tlex::isaBlockOpen (char c){
	int i;
	for (i=0; i<blocks.size(); i++){
		if ( c == blocks[i].open )
			return true;
	}
	return false;
}

int Tlex::isaBlockClose (char c){
	int i;
	for (i=0; i<blocks.size(); i++){
		if ( c == blocks[i].close )
			return true;
	}
	return false;
}

char Tlex::getBlockClose (char open){
	int i;
	for (i=0; i<blocks.size(); i++){
		if ( open == blocks[i].open )
			return blocks[i].close;
	}
	return '\0';
}
