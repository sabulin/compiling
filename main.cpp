#include <iostream>
#include "LexicalAnalysis.h"
#include "GrammaticalAnalysis.h"


using namespace std;

int main()
{
    cout << "-------------------- �����Ǵʷ����� ----------------------------" << endl;
//    char test[] = "double x = 5; int b = x + 5; int a = 0; if(a < 3){ int z = a + 1;} else int yy=100;#";
 char test[]="int sort(int a){double b = 1;}#";
 //  char test[]="int a=1;if(a>1){a=2;} else {a=3;}#";

    scan(test);
    cout << "-------------------- ÿ���ʵ��ֱ���չʾ ----------------------------" << endl;
    printToken();
    printSymbolTableL();
    cout << "-------------------- �������﷨���� ----------------------------" << endl;


//    setSymbolTable(getLSymbolTable(), getLSymbolTableSize());

    grammaticalAnalysis(getToken(), getTop());



    return 0;
}



