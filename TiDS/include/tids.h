#ifndef TIDS_H
#define TIDS_H

#include <iostream>
#include <string>
#include <vector>


using namespace std;


/*=====================================================================================*/

class TiGroup;
class TiConj;




class TiAttr {
	public:
	string name;
	string value;

	TiAttr (string name);
	void write(int nvl=0);
};


class TiObj {
public:
	string classe;
	string name;
private:
	vector<TiAttr*> attrs;	
	TiGroup* objs;

public:
	TiObj();
	TiObj(string classe, string name);
	TiObj* add (string classe, string name);

	void getConj(TiConj *out, string classe);
	TiObj* getObj(string name);
	TiObj* getObj(string classe, string name);

	void setAttr(string name, string value);
	string getAttr (string name, string nil="");
	int getAttrInt (string name, int nil=0);
	

	void write(int nvl=0);
	
	TiObj* operator[] (string name);

	private:
	TiAttr* getAttrPtr(string name);	
};

/*-------------------------------------------------------------------------------------*/


/*=====================================================================================*/

class TiConj{
public:
	string classe;
private:
	vector<TiObj*> objs;

public:
	TiConj();
	TiConj(string classe);

	int size();

	void add (TiObj* obj);
	void add (TiConj* src);
	TiObj* add (string name);

	TiObj* operator[](int i);
	TiObj* operator[](string name);
	void filter(string filtro);

	void write(int nvl=0);
};

/*-------------------------------------------------------------------------------------*/



/*=====================================================================================*/

class TiGroup {
private:
	vector<TiConj*> conjs;

public:
	TiObj* add (string classe, string name);
	void getConj(TiConj *out, string classe);
	TiObj* getObj(string classe, string name);
	TiObj* getObj(string name);
	void write(int nvl=0);

protected:
	TiConj* getConjPtr(string classe);
};

/*-------------------------------------------------------------------------------------*/


/*=====================================================================================*/

class TiDS {
	TiObj* cur;
	vector<TiObj*> path;
	TiObj objs;

public:
	TiDS ();

	void load(string file);
	int enter (TiObj* obj);
	void leave ();
	void setInRoot();

	void create(string classe, string name);
	TiObj* add(string classe, string name);

	void getConj(TiConj *out, string classe);

	void setAttr(string name, string value);
	string getAttr(string name, string nil="");
	int getAttrInt(string name, int nil=0);

	vector<TiObj*>* getPath();
	void write();

	TiObj* operator[] (string name);	

	/*---------------------------------------*/
	private:
	void readFile(string* out, string file);
	void parse(string text);

};


/*-------------------------------------------------------------------------------------*/


#endif
