/*=====================================================================================*/

#include "tisys.h"
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include "tlex.h"

/*-------------------------------------------------------------------------------------*/


/*=====================================================================================*/

int isPreposition(char* word){
	static string prepositions[] = {"all", "from", "to"};
	int i;
	for (i=0; i<3; i++){
		if ( prepositions[i] == word )
			return 1;
	}
	return 0;
}

char* Arg_get (int argc, char** argv, char* name, char* defaul){
	int i;

	if ( strcmp(name, "AKK") != 0 ){
		for ( i=1; i<argc; i++){
			if ( strcmp(name, argv[i]) == 0 ){		
				if ( i+1 >= argc )
					return "";
				if ( argv[i+1][0] == '-' )
					return "";
				return argv[i+1];
			}
		}
	} else {
		for ( i=1; i<argc; i++){
			if ( !isPreposition(argv[i]) ){
				return argv[i];
			} else {
				i++;
			}
		}
	}
	return defaul;
}

int Arg_isExist (int argc, char** argv, char* name){
	int i;
	for ( i=1; i<argc; i++){
		if ( strcmp(name, argv[i]) == 0 ){
			return 1;
		}
	}
	return 0;
}


/*-------------------------------------------------------------------------------------*/


/*=====================================================================================*/

void TiSys::getFileType(string* out_type, string name){
	unsigned found = name.find_last_of(".");
	string ext = name.substr(found+1);
	if ( ext == "php" ){
		*out_type = "File:Code:PHP";
	} else if ( ext == "c" ){
		*out_type = "File:Code:C";
	} else if ( ext == "cpp" ){
		*out_type = "File:Code:CPP";
	} else if ( ext == "h" ){
		*out_type = "File:Code:CHeader";
	}  else if ( ext == "txt" ){
		*out_type = "File:Text:TXT";
	} else if ( ext == "doc" ){
		*out_type = "File:Document:DOC";
	} else if ( ext == "odt" ){
		*out_type = "File:Document:ODT";
	} else if ( ext == "pdf" ){
		*out_type = "File:Document:PDF";
	} else if ( ext == "xml" ){
		*out_type = "File:Data:XML";
	} else if ( ext == "html" ){
		*out_type = "File:Data:HTML";
	} else if ( ext == "jpg" ){
		*out_type = "File:Image:JPG";
	} else if ( ext == "jpeg" ){
		*out_type = "File:Image:JPG";
	} else if ( ext == "png" ){
		*out_type = "File:Image:PNG";
	} else if ( ext == "gif" ){
		*out_type = "File:Image:GIF";
	} else if ( ext == "mp3" ){
		*out_type = "File:Music:MP3";
	} else if ( ext == "ogg" ){
		*out_type = "File:Music:OGG";
	} else if ( ext == "flv" ){
		*out_type = "File:Video:FLV";
	} else if ( ext == "mp4" ){
		*out_type = "File:Video:MP4";
	} else if ( ext == "zip" ){
		*out_type = "File:Packet:ZIP";
	} else if ( ext == "gz" ){
		*out_type = "File:Packet:GZ";
	} else if ( ext == "rar" ){
		*out_type = "File:Packet:RAR";
	} else {
		*out_type = "File";
	}
}

void TiSys::getFolderType(string* out_type, string tpath){
	string path = tpath + "/.tiobj.xml";
	*out_type = "Folder";
	FILE* fp = fopen (path.c_str(), "r");
	if ( !fp ){
		return;
	}
	fclose(fp);

	xmlDocPtr doc = xmlParseFile( path.c_str() );
	xmlXPathObjectPtr result = getnodeset (doc, (xmlChar*)"/tiobj/class" );
	xmlNodeSetPtr nodeset = result->nodesetval;
	xmlChar *text = xmlNodeListGetString(doc, nodeset->nodeTab[0]->xmlChildrenNode, 1);
	*out_type += ":";
	*out_type += (char*) text;
	xmlFree(text);
	xmlXPathFreeObject (result);
	xmlFreeDoc(doc);
	xmlCleanupParser();
}

int TiSys::isa(string typebase, string type){
	Tlex lex;
	lex.addDiv(':');
	lex.load(typebase);
	while ( !lex.isEOF() ){
		Token token;
		lex.next(&token);
		if ( token.value == type )
			return 1;
	}
	return 0;
}

xmlXPathObjectPtr TiSys::getnodeset (xmlDocPtr doc, xmlChar *xpath){	
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;
	context = xmlXPathNewContext(doc);
	if (context == NULL) {
		return NULL;
	}
	result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	if (result == NULL) {
		return NULL;
	}
	if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
		xmlXPathFreeObject(result);
		return NULL;
	}
	return result;
}

/*-------------------------------------------------------------------------------------*/


/*=====================================================================================*/

TiItem::TiItem (string name, string type){
	this->name = name;
	this->type = type;
}

int TiItem::isa(string type){
	Tlex lex;
	lex.addDiv(':');
	lex.load(this->type);
	while ( !lex.isEOF() ){
		Token token;
		lex.next(&token);
		if ( token.value == type ){
			return 1;
		}
	}
	return 0;
}

/*-------------------------------------------------------------------------------------*/


/*=====================================================================================*/

TiFolder::TiFolder (string path):TiItem(path,""){
	if ( path != "")
		this->path = path;
	else
		this->path = ".";
	TiSys::getFolderType(&this->type, this->path);
}

void TiFolder::setPath(string path){
	if ( path != "")
		this->path = path;
	else
		this->path = ".";
	TiSys::getFolderType(&this->type, this->path);
}

void TiFolder::load(){
	DIR *dp;
	struct dirent *ep;     
	dp = opendir ( path.c_str() );

	if (dp != NULL){
		while ( ep=readdir(dp) ){
			string type;
			TiItem* novo;
			if ( ep->d_type == ISFILE ){					
				TiSys::getFileType(&type, ep->d_name);
				novo = new TiItem(ep->d_name, type);
				this->subitens.push_back(novo);
			} else if ( ep->d_type == ISFOLDER ){
				TiSys::getFolderType(&type, ep->d_name);
				novo = new TiItem(ep->d_name, type);
				this->subitens.push_back(novo);
			}
		}
		closedir (dp);
	}
}

void TiFolder::load(string filter){
	if ( filter == "" ){
		this->load();
		return;
	}

	DIR *dp;
	struct dirent *ep;     
	dp = opendir ( path.c_str() );

	if (dp != NULL){
		while ( ep=readdir(dp) ){
			string type;
			TiItem* novo;
			if ( ep->d_type == ISFILE ){					
				TiSys::getFileType(&type, ep->d_name);
				if ( TiSys::isa(type, filter) ){
					novo = new TiItem(ep->d_name, type);
					this->subitens.push_back(novo);
				}
			} else if ( ep->d_type == ISFOLDER ){
				TiSys::getFolderType(&type, ep->d_name);
				if ( TiSys::isa(type, filter) ){
					novo = new TiItem(ep->d_name, type);
					this->subitens.push_back(novo);
				}
			}
		}
		closedir (dp);
	}
}

void TiFolder::sort(){
	
}

void TiFolder::write(){
	int i;
	for (i=0; i<this->subitens.size(); i++){
		if ( this->subitens[i]->name[0] != '.' ){
			cout << this->subitens[i]->type << " --- " << this->subitens[i]->name << endl;
		}
	}
}

char* TiFolder::getRealPath(){
	char actualpath [PATH_MAX+1];
	return realpath(path.c_str(), actualpath);
}

/*-------------------------------------------------------------------------------------*/


