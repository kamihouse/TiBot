#ifndef TISYS_H
#define TISYS_H

/*=====================================================================================*/

#include <string>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <vector>

using namespace std;

/*-------------------------------------------------------------------------------------*/


/*=====================================================================================*/

int isPreposition(char* word);
char* Arg_get (int argc, char** argv, char* name, char* defaul);
int Arg_isExist (int argc, char** argv, char* name);

/*-------------------------------------------------------------------------------------*/


/*=====================================================================================*/

#define ISFILE 0x8
#define ISFOLDER 0x4

class TiSys {
	public: 
		static void getFileType(string* out_type, string name);
		static void getFolderType(string* out_type, string tpath);
		static int isa(string typebase, string type);

	private:
		static xmlXPathObjectPtr getnodeset (xmlDocPtr doc, xmlChar *xpath);
};


class TiItem {
	public: 
		string name;
		string type;

	public:
		TiItem (string name, string type);
		int isa(string type);
};


class TiFolder : public TiItem {
	private:
		string path;
		vector<TiItem*> subitens;

	public:
		TiFolder (string path="");
		void load();
		void load(string filter);

		void setPath(string path);
		char* getRealPath();
		
		void sort();
		void write();
};


/*-------------------------------------------------------------------------------------*/

#endif
