#include <stdio.h>
#include <stdlib.h>
#include "xml.h"
#include "charUtility.h"

xmlElement* create() {
    xmlElement* e = (xmlElement*) malloc(sizeof(xmlElement));
    e->tag = e->field = NULL;
    e->next = e->attrRoot = e->attrNode = e->elemRoot = e->elemNode = NULL;
    return e;
}

void __xmlElement(xmlElement* e, char* text, int l, int r) {
    if (text != NULL) {
        int i = l, j;
        while (text[i] != '>' && text[i] != '/' && text[i] != ' ') {
            ++i;
        }
        e->tag = substring(text, l + 1, i);
        while (i < r && isSpace(text[i])) {
            while (i < r && isSpace(text[i])) ++i;
            if (i >= r || text[i] == '/' || text[i] == '>') break;
            j = i + 1;
            while (text[j] != '=') {
                ++j;
            }
            xmlElement* element = create();
            element->tag = substring(text, i, j);
            while (text[j] != '\"' && text[j] != '\'') ++j;
            i = j + 1;
            while (text[i] != '\"' && text[i] != '\'') ++i;
            element->field = substring(text, j, i + 1);
            if (e->attrRoot == NULL) {
                e->attrRoot = e->attrNode = element;
            } else {
                e->attrNode->next = element;
                e->attrNode = element;
            }
            ++i;
        }
        if (i >= r) return;
        if (text[i] != '/') {
            ++i;
            while (i < r && isSpace(text[i])) ++i;
            j = i;
            while (j < r && text[j] != '<') ++j;
            if (i < j) {
                e->field = substring(text, i, j);
            }
        }
    }
}

// With an string (concatenate from the xml file) extract objects, save to root element
xmlElement* extract(char* xml) {
    int i = 0, j;

    xmlElement* root = create();
    xmlLinkedList* node = (xmlLinkedList*) malloc(sizeof(xmlLinkedList));
    node->previous = NULL;
    node->element = root;

    int len = stringLen(xml);
    while (i < len) {
        while (i < len && isSpace(xml[i])) ++i;
        j = i + 1;
        if (j >= len) break;
        if (xml[j] == '/') {
            xmlLinkedList* foo = node;
            node = node->previous;
            free(foo);
            while (xml[j] != '>') ++j;
            i = j + 1;
            continue;
        }

        while (xml[j] != '/' && xml[j] != '>') ++j;
        xmlElement* pElement = node->element;
        xmlElement* element = create();

        if (xml[j] == '/') {
            __xmlElement(element, xml, i, j + 2);
            i = j + 2;
        } else {
            ++j;
            int k = j;
            while (isSpace(xml[j])) ++j;
            if (xml[j] != '<' || (xml[j] == '<' && xml[j + 1] == '/')) {
                while (xml[j] != '>') ++j;
                __xmlElement(element, xml, i, j + 1);
                i = j + 1;
            } else {
                __xmlElement(element, xml, i, k);
                xmlLinkedList* foo = node;
                node = (xmlLinkedList*) malloc(sizeof(xmlLinkedList));
                node->previous = foo;
                node->element = element;
                i = j;
            }
        }
        if (pElement->elemRoot == NULL) {
            pElement->elemRoot = pElement->elemNode = element;
        } else {
            pElement->elemNode->next = element;
            pElement->elemNode = element;
        }
    }
    free(node);
    return root;
}

// Take a root, write to 'file' the output xml file, the whole progress is the same like
// the previous with C++
void convert(FILE* file, xmlElement* node, int depth) {
    if (depth > 0) {
        for (int tab = 1; tab < depth; ++tab) {
            // printf("\t");
            fprintf(file, "\t");
        }
        // printf("<%s", node->tag);
        fprintf(file, "<%s", node->tag);
        for (xmlElement* e = node->attrRoot; e != NULL; e = e->next) {
            // printf(" %s=%s", e->tag, e->field);
            fprintf(file, " %s=%s", e->tag, e->field);
        }
        // printf(">");
        fprintf(file, ">");
        // if (node->field != NULL && stringLen(node->field) > 1) printf("%s", node->field);
        if (node->field != NULL)
            fprintf(file, "%s", node->field);
    }
    if (depth > 0 && node->elemRoot != NULL) fprintf(file, "\n");
    for (xmlElement* e = node->elemRoot; e != NULL; e = e->next) {
        convert(file, e, depth + 1);
    }
    if (node->elemRoot != NULL) {
        for (int tab = 1; tab < depth; ++tab) {
            // printf("\t");
            fprintf(file, "\t");
        }
    }
    if (depth > 0) {
        // printf("</%s>\n", node->tag);
        fprintf(file, "</%s>\n", node->tag);
    }
}

// Free memory whenever we do not need them anymore
void freeXmlElement(xmlElement* node) {
    if (node == NULL) return;
    if (node->next != NULL) freeXmlElement(node->next);
    if (node->attrRoot != NULL) freeXmlElement(node->attrRoot);
    if (node->elemRoot != NULL) freeXmlElement(node->elemRoot);
    if (node->tag != NULL) free(node->tag);
    if (node->field != NULL) free(node->field);
    free(node);
}

void exe() {
    // See 'data' directory
    FILE *file = fopen("data/file.xml", "r");
    FILE *fout = fopen("data/output.xml", "w");

    if (file && fout) {
        char* text = NULL;
        char* line = (char*) malloc(128 * sizeof(char));
        fseek(file, 0, SEEK_SET);
        while (fgets(line, 128, file)) {
            text = concat(text, line);
        }

        // Extract objects to root
        xmlElement* root = extract(text);

        // Convert back to xml
        convert(fout, root, 0);

        // Free node memory
        freeXmlElement(root);

        if (text != NULL) free(text);

        fclose(file);
        fclose(fout);
    }
}
