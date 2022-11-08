//
// Created by lenovo on 2022/11/7.
//

#ifndef MYLAB2_DATASTRUCTURE_H
#define MYLAB2_DATASTRUCTURE_H

#include <string>

using namespace std;

typedef struct WORD{
    int code; //种别码
    string key; // 键
    string addr = "#"; //符号表地址码
    string position; //位置
    int lineNum; // 行号
    string extra; // 额外信息（没啥用）
    bool variable; //是否是变量
} WORD;

enum symbol{
    ADD = 21, SUBTRACT, MULTIPLY, DIVIDE, EQ_TWO, EQ, L_BRACE, R_BRACE, BIGGER_EQ, BIGGER, SMALLER_EQ, SMALLER, L_PARENTHESIS, R_PARENTHESIS, SEMICOLON, START_END
};

typedef struct SymbolLine{
    string addr;
    string value; // 名字
    string type;
    string vType;
} SymbolLine;

typedef struct AnalysisCell{
    string op; //状态
    int direct;//方向
} AnalysisCell; // 分析表单元

typedef struct Collection{
    int* vts = (int*)malloc(100 * sizeof(int));
    int top = 0;
} Collection;// FIRST FOLLOW集

typedef struct Item{
    int pId;  //产生式id
    int idx;  // 在分析栈中最后一个元素的下标
} Item;

typedef struct Production{
    //int id;
    string leftPart;//左部
    string rightPart[100];//右部
    int rightPartLength;
} Production;//产生式

typedef struct State{
    int id;
    Item *items = (Item*)malloc(100 * sizeof(Item)); //项目数组
    int top = 0;
} State; // 状态


#endif //MYLAB2_DATASTRUCTURE_H
