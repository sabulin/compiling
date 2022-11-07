#include <iostream>
#include "LexicalAnalysis.h"
#include "GrammaticalAnalysis.h"


using namespace std;

int main()
{
    cout << "-------------------- 以下是词法分析 ----------------------------" << endl;
//    char test[] = "double x = 5; int b = x + 5; int a = 0; if(a < 3){ int z = a + 1;} else int yy=100;#";
 char test[]="int sort(int a){double b = 1;}#";
 //  char test[]="int a=1;if(a>1){a=2;} else {a=3;}#";

    scan(test);
    cout << "-------------------- 每个词的种别码展示 ----------------------------" << endl;
    printToken();
    printSymbolTableL();
    cout << "-------------------- 以下是语法分析 ----------------------------" << endl;


//    setSymbolTable(getLSymbolTable(), getLSymbolTableSize());

    grammaticalAnalysis(getToken(), getTop());



    return 0;
}



