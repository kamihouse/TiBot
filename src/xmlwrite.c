#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

xmlDocPtr getdoc (char *docname) {
	xmlDocPtr doc;
	doc = xmlParseFile(docname);
	
	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return NULL;
	}

	return doc;
}

xmlXPathObjectPtr getnodeset (xmlDocPtr doc, xmlChar *xpath){
	
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;

	context = xmlXPathNewContext(doc);
	if (context == NULL) {
		fprintf(stderr, "Error in xmlXPathNewContext\n");
		return NULL;
	}
	result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	if (result == NULL) {
		fprintf(stderr, "Error in xmlXPathEvalExpression\n");
		return NULL;
	}
	if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
		xmlXPathFreeObject(result);
                //printf("No result\n");
		return NULL;
	}
	return result;
}


int main(int argc, char **argv) {
		
	if (argc <= 3) {
		printf("Usage: %s docname xpath newvalue\n", argv[0]);
		return(0);
	}
	char* docname = argv[1];
	xmlChar* xpath = (xmlChar*) argv[2];	
	xmlChar* newvalue = (xmlChar*) argv[3];
	

	xmlDocPtr doc = getdoc(docname);
	xmlXPathObjectPtr result = getnodeset (doc, xpath);
	if (result){
		int i = 0;
		xmlNodeSetPtr nodeset = result->nodesetval;
		//for (i=0; i < nodeset->nodeNr; i++) {
			xmlNode* cur = nodeset->nodeTab[i]->xmlChildrenNode;
			xmlNodeSetContent(cur, newvalue);
		//}
		xmlXPathFreeObject (result);
	}

	xmlSaveFileEnc(docname, doc, "UTF-8");
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return (1);
}




/*#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>

int main(int argc, char **argv){
	char* filename;
	if (argc < 2) {
		fprintf(stderr, "Usage: %s filename.xml\n", argv[0]);
		return 1;
	}
	filename = argv[1];

	xmlDoc*  document = xmlReadFile(filename, NULL, 0);
	xmlNode* root = xmlDocGetRootElement(document);	
	fprintf(stdout, "Root is <%s> (%i)\n", root->name, root->type);

	xmlNode* node;
	xmlNode* first_child = root->children;
	for (node=first_child; node; node=node->next) {
		//if ( node->type == 
		fprintf(stdout, "\t Child is <%s> (%i)\n", node->name, node->type);
	}


	xmlSaveFileEnc("teste02.xml", document, "UTF-8");
	return 0;
}*/
