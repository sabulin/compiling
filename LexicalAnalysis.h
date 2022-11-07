#ifndef __LEXICALANALYSIS_H__
#define __LEXICALANALYSIS_H__

#include "datastructure.h"

WORD* getToken();

int getTop();

bool digit(char target);

bool letter(char target);

int keyword(char target[]);

bool point(char target);

void deleteBlank(char *target);

void addToken(int code, char* word);

void printToken();

void scan(char *words);

void printSymbolTableL();

SymbolLine* getLSymbolTable();

int getLSymbolTableSize();

#endif
