#include <iostream>
#include "LexicalAnalysis.h"
#include "GrammaticalAnalysis.h"


using namespace std;

int main()
{
    cout << "-------------------- Lexical Analysis ----------------------------" << endl;
    char test[] = "double x = 5; int b = x + 5; int a = 0; if(a < 3){ int z = a + 1;}#";
    //char test[] = "boolean m = 1 ; int x = 5; int c = x + m; int a = 0; while(a < 3){ int z = a + 1; }#";

    //char test[] = "boolean x=1;\n int b = 7 + 5;\n int c = b+1;\n int a=0;\n while(a<3){{ \n int z = a + 1; \n}#";
    //char test[] = "// 这是单行注释\n /* 多行注释\n 多行注释 */ \n boolean x = 1.1.; int b = 7 + 5; int c = b+1; int a=0; while(a<3){ int z = a + 1; }#";
    scan(test);
    printToken();
    printSymbolTableL();
    cout << "-------------------- Grammatical Analysis ----------------------------" << endl;

    cout << "-------------------- Semantic Analysis ----------------------------" << endl;

//    setSymbolTable(getLSymbolTable(), getLSymbolTableSize());

    grammaticalAnalysis(getToken(), getTop());

//    printIntermediateCodeLine();
//
//    printSymbolTable();

    return 0;
}


//boolean m = 1;
//int x = 5;
//int c = x + m;
//int a = 0;
//while(a < 3){
//    int z = a + 1;
//}

//double x = 5;
//int b = x + 5;
//int a = 0;
//while(a < 3){
//    int z = a + 1;
//}
