#include <iostream>
#include "LexicalAnalysis.h"
#include "GrammaticalAnalysis.h"


using namespace std;

int main()
{
    cout << "-------------------- 以下是词法分析 ----------------------------" << endl;
  //  char test[] = " if(a < 3){ int z = a + 1;} else {int yy=100;}#";//赋值+分支
//char test[]="int sort(int  a){double b = 1;}#"; //函数定义
 //  char test[]="int a=1;while(a>1){a=2;}#";//循环语句
  char test[]="int ;;;;;a=1;#"; //错误处理

    scan(test);
    cout << "-------------------- 每个词的种别码展示 ----------------------------" << endl;
    printToken();
    printSymbolTableL();
    cout << "-------------------- 以下是语法分析 ----------------------------" << endl;


//    setSymbolTable(getLSymbolTable(), getLSymbolTableSize());

    grammaticalAnalysis(getToken(), getTop());



    return 0;
}



