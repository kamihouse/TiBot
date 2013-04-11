#include "tids.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tlex.h"


void indent(int size){
	for (int i=0; i<size; i++){
		cout << '\t';
	} 
}



/*=====================================================================================*/

TiAttr::TiAttr (string name){
	this->name = name;
}

void TiAttr::write(int nvl){
	indent(nvl);
	cout << name << " = " << value << ";";
}


TiObj::TiObj(){
	objs = NULL;
}


TiObj::TiObj(string classe, string name){
	this->classe = classe;
	this->name = name;
	objs = NULL;
}


void TiObj::setAttr(string name, string value){
	TiAttr* attr = getAttrPtr(name);
	if ( !attr ){
		attr = new TiAttr(name);
		attrs.push_back(attr);
	} 
	attr->value = value;
}


string TiObj::getAttr (string name, string nil){
	TiAttr* attr = getAttrPtr(name);
	if ( !attr ){
		return nil;
	}
	return attr->value;
}


int TiObj::getAttrInt (string name, int nil){
	TiAttr* attr = getAttrPtr(name);
	if ( !attr ){
		return nil;
	}
	return atoi(attr->value.c_str());
}


TiAttr* TiObj::getAttrPtr(string name){
	for (int i=0; i<attrs.size(); i++){
		if (attrs[i]->name == name)
			return attrs[i];
	}
	return NULL;
}	

TiObj* TiObj::add (string classe, string name){
	if ( !objs ){
		objs = new TiGroup();
	}
	return objs->add(classe, name);
}

void TiObj::getConj(TiConj* out, string classe){
	if ( objs )
		objs->getConj(out, classe);
}

TiObj* TiObj::getObj(string name){
	if ( !objs )
		return NULL;
	return objs->getObj(name);
}

TiObj* TiObj::getObj(string classe, string name){
	if ( !objs )
		return NULL;
	return objs->getObj(classe, name);
}

void TiObj::write(int nvl){
	indent(nvl);
	cout << classe << " " << name;
	if ( attrs.size() > 0 || objs ){
		cout << " {\n";
		for (int i=0; i<attrs.size(); i++){
			attrs[i]->write(nvl+1);
			cout << endl;
		}
		if (objs)
			objs->write(nvl+1);
		indent(nvl);
		cout << "}\n";
	} else {
		cout << ";\n";
	}
}

TiObj* TiObj::operator[] (string name){
	if ( !objs )
		return NULL;
	Token t;
	Tlex lex;
	lex.addSymbol('@');
	lex.addDiv(':');
	lex.load(name);
	lex.next(&t);
	if ( t.value == "@" ){
		Token classe, name;
		lex.next(&classe);
		lex.next(&name);
		return getObj(classe.value, name.value);
	} else {
		return objs->getObj(name);
	}
}

/*-------------------------------------------------------------------------------------*/


/*=====================================================================================*/

TiConj::TiConj(){
}

TiConj::TiConj(string classe){
	this->classe = classe;
}

int TiConj::size(){
	return objs.size();
}

void TiConj::add(TiObj* obj){
	objs.push_back(obj);
}

void TiConj::add(TiConj* src){
	for (int i=0; i<src->size(); i++){
		this->add( (*src)[i]);
	}
}

TiObj* TiConj::add(string name){
	TiObj* obj = new TiObj(this->classe, name);
	objs.push_back( obj );
	return obj;
}

void TiConj::filter(string filtro){
	Token attr, op, value;
	Tlex lex;
	lex.addDiv(' ');
	lex.addSymbol('=');
	lex.load(filtro);
	lex.next(&attr);
	lex.next(&op);
	lex.next(&value);
	if ( op.value == "=" ){
		int i;
		for (i=0; i<this->size(); ){
			if (this->objs[i]->getAttr(attr.value) != value.value ){
				this->objs[i] = this->objs.back();
				this->objs.pop_back();				
			} else {
				i++;
			}
		}
	}
}
 
TiObj* TiConj::operator[](int i){
	return objs[i];
}

TiObj* TiConj::operator[](string name){
	for (int i=0; i<objs.size(); i++){
		if ( name == objs[i]->name )
			return objs[i];
	}
}

void TiConj::write(int nvl){
	for (int i=0; i<objs.size(); i++){
		objs[i]->write(nvl);
	}
}	

/*-------------------------------------------------------------------------------------*/



/*=====================================================================================*/

TiObj* TiGroup::add (string classe, string name){
	TiConj* conj = getConjPtr(classe);
	if (!conj){
		conj = new TiConj(classe);
		conjs.push_back(conj);
	}
	return conj->add(name);
}

void TiGroup::getConj(TiConj* out, string classe){
	out->classe = classe;
	for (int i=0; i<conjs.size(); i++){
		if (classe == conjs[i]->classe ){
			out->add(conjs[i]);
		}
	}
}

TiObj* TiGroup::getObj(string classe, string name){
	TiConj* conj = getConjPtr(classe);
	if (!conj){
		return NULL;
	}
	return (*conj)[name];
}

TiObj* TiGroup::getObj(string name){
	TiObj* res = NULL;
	for (int i=0; i<conjs.size(); i++){		
		if ( res = getObj(conjs[i]->classe, name) )
			return res;
	}
	return NULL;
}

void TiGroup::write(int nvl){
	for (int i=0; i<conjs.size(); i++){
		conjs[i]->write(nvl);
	}
}

TiConj* TiGroup::getConjPtr(string classe){
	for (int i=0; i<conjs.size(); i++){
		if (classe == conjs[i]->classe ){
			return conjs[i];
		}
	}
	return NULL;
}

/*-------------------------------------------------------------------------------------*/


/*=====================================================================================*/


TiDS::TiDS (){
	cur = &objs;
}	

void TiDS::load(string file){
	string text;
	readFile(&text, file);
	parse(text);
	if (path.size() > 0){
		cout << "ERROR: parse file\n";
		exit(1);
	}
	objs.name = file;
	objs.classe = "File";
}

int TiDS::enter (TiObj* obj){
	if ( obj ){
		path.push_back(cur);
		cur = obj;
		return true;
	} else
		return false;
}

void TiDS::leave (){
	if (path.size() > 0){
		cur = path.back();
		path.pop_back();
	} else
		cur = &objs;			
}

void TiDS::setInRoot(){
	for (int i=0; i<path.size(); i++){
		path.pop_back();
	}
	cur = &objs;
}

void TiDS::create(string classe, string name){
	TiObj* obj = add(classe, name);
	enter(obj);
}

TiObj* TiDS::add(string classe, string name){
	return cur->add(classe, name);
}

void TiDS::getConj(TiConj* out, string classe){
	cur->getConj(out, classe);
}

void TiDS::setAttr(string name, string value){
	cur->setAttr(name, value);
}

string TiDS::getAttr(string name, string nil){
	return cur->getAttr(name, nil);
}

int TiDS::getAttrInt(string name, int nil){
	return cur->getAttrInt(name, nil);
}

vector<TiObj*>* TiDS::getPath(){
	return &this->path;
}

void TiDS::write(){
	cur->write(0);
}

TiObj* TiDS::operator[] (string name){
	return (*cur)[name];
}

/*---------------------------------------*/
void TiDS::readFile(string* out, string file){
	static char buffer[64*1024];
	FILE* fp = fopen(file.c_str(), "r");
	if (!fp)
		return;
	int size = fread(buffer, 1, 16*1024, fp);
	buffer[size] = '\0';
	*out = buffer;
	fclose(fp);
}

void TiDS::parse(string text){
	Tlex lex;
	lex.load(text);
	lex.addDiv(' ');
	lex.addDiv('\n');
	lex.addDiv('\t');
	lex.addSymbol('=');
	lex.addSymbol(';');
	lex.addBlock('{','}');
	
	vector<Token*> stack;
	while ( !lex.isEOF() ){
		Token *a1 = lex.next();
				
		if ( a1->value==";" || a1->isBlock() ){
			if ( stack.size() == 1 ){
				Token* classe = stack.back();
				stack.pop_back();
				if ( a1->isBlock() ){
					this->create(classe->value, "");
					parse(a1->value);
					this->leave();
				} else {
					this->add(classe->value, "");
				}
			} else if ( stack.size() == 2 ){
				Token* name = stack.back();
				stack.pop_back();
				Token* classe = stack.back();
				stack.pop_back();
				this->create(classe->value, name->value);
				if ( a1->isBlock() )
					parse(a1->value);
				this->leave();
			} else if ( stack.size() == 3 && stack[1]->value == "=" ){
				Token* value = stack.back();
				stack.pop_back();
				stack.pop_back();
				Token* attr = stack.back();
				stack.pop_back();
				this->setAttr(attr->value, value->value);
			}
		} else
			stack.push_back(a1);
	}
}


/*-------------------------------------------------------------------------------------*/

