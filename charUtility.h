#ifndef ___CHAR_UTILITY___
#define ___CHAR_UTILITY___
#include <stdlib.h>
#include <stdio.h>

// This file contain function for handling char*
// similar to of string

char* substring(char* str, int l, int r) {
    char* substr = (char*) malloc((r - l + 1) * sizeof(char));
    for (int i = l; i < r; ++i) {
        substr[i - l] = str[i];
    }
    substr[r - l] = '\0';
    return substr;
}

int stringLen(char* c) {
    if (c == NULL) return 0;
    int len = 0;
    while (c[len] != '\0') {
        ++len;
    }
    return len;
}

char* concat(char* x, char* y) {
    int xLen = stringLen(x);
    int len = xLen + stringLen(y) + 1;
    x = (char*) realloc(x, len * sizeof(char));
    for (int i = xLen; i < len - 1; ++i) {
        x[i] = y[i - xLen];
    }
    x[len - 1] = '\0';
    return x;
}

char isSpace(char c) {
    return c == ' ' || c == '\n' || c == '\t' ? 1 : 0;
}

#endif
