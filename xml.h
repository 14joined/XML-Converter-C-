#ifndef ___XML___
#define ___XML___

#include <stdlib.h>

typedef struct xmlElement {
    char* tag;
    char* field;
    struct xmlElement* next;

    struct xmlElement* attrRoot;
    struct xmlElement* attrNode;
    struct xmlElement* elemRoot;
    struct xmlElement* elemNode;
} xmlElement;

typedef struct xmlLinkedList {
    struct xmlLinkedList* previous;
    xmlElement* element;
} xmlLinkedList;

xmlElement* create();

// Process an individual XML Element
void __xmlElement(xmlElement* e, char* text, int l, int r);

// Free memory for a node of xmlElement kind
void freeXmlElement(xmlElement* node);

// With an string (concatenate from the xml file) extract objects, save to root element
xmlElement* extract(char* xml);

// Take a root, write to 'file' the output xml file, the whole progress is the same like
// the previous implement with C++
void convert(FILE* file, xmlElement* node, int depth);

// Extract objects then convert back to output file
void exe();

#endif
