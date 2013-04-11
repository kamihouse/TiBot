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
	int i;
		
	if (argc <= 2) {
		printf("Usage: %s docname xpath\n", argv[0]);
		return(0);
	}
	xmlChar *xpath = (xmlChar*) argv[2];
	char* docname = argv[1];
	

	xmlDocPtr doc = getdoc(docname);
	xmlXPathObjectPtr result = getnodeset (doc, xpath);
	if (result){
		xmlNodeSetPtr nodeset = result->nodesetval;
		for (i=0; i < nodeset->nodeNr; i++) {
			xmlChar *text = xmlNodeListGetString(doc, nodeset->nodeTab[i]->xmlChildrenNode, 1);
			printf("%s\n", text);
			xmlFree(text);
		}
		xmlXPathFreeObject (result);
	}


	xmlFreeDoc(doc);
	xmlCleanupParser();
	return (1);
}
