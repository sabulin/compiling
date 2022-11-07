//
// Created by lenovo on 2022/11/7.
//

#ifndef MYLAB2_DATASTRUCTURE_H
#define MYLAB2_DATASTRUCTURE_H

#include <string>

using namespace std;

typedef struct WORD{
    int code;
    string key;
    string addr = "#";
    string position;
    int lineNum;
    string extra;
    bool variable;
} WORD;

enum symbol{
    ADD = 21, SUBTRACT, MULTIPLY, DIVIDE, EQ_TWO, EQ, L_BRACE, R_BRACE, BIGGER_EQ, BIGGER, SMALLER_EQ, SMALLER, L_PARENTHESIS, R_PARENTHESIS, SEMICOLON, START_END
};

typedef struct SymbolLine{
    string addr;
    string value;
    string type;
    string vType;
} SymbolLine;

typedef struct AnalysisCell{
    string op;
    int direct;
} AnalysisCell;

typedef struct Collection{
    int* vts = (int*)malloc(100 * sizeof(int));
    int top = 0;
} Collection;

typedef struct Item{
    int pId;
    int idx;  // 在分析栈中最后一个元素的下标
} Item;

typedef struct Production{
    //int id;
    string leftPart;
    string rightPart[10];
    int rightPartLength;
} Production;

typedef struct State{
    int id;
    Item *items = (Item*)malloc(100 * sizeof(Item));
    int top = 0;
} State;


#endif //MYLAB2_DATASTRUCTURE_H
