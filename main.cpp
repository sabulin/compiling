#include <iostream>
#include "LexicalAnalysis.h"
#include "GrammaticalAnalysis.h"


using namespace std;

int main()
{
    cout << "-------------------- �����Ǵʷ����� ----------------------------" << endl;
  //  char test[] = " if(a < 3){ int z = a + 1;} else {int yy=100;}#";//��ֵ+��֧
//char test[]="int sort(int  a){double b = 1;}#"; //��������
 //  char test[]="int a=1;while(a>1){a=2;}#";//ѭ�����
  char test[]="int ;;;;;a=1;#"; //������

    scan(test);
    cout << "-------------------- ÿ���ʵ��ֱ���չʾ ----------------------------" << endl;
    printToken();
    printSymbolTableL();
    cout << "-------------------- �������﷨���� ----------------------------" << endl;


//    setSymbolTable(getLSymbolTable(), getLSymbolTableSize());

    grammaticalAnalysis(getToken(), getTop());



    return 0;
}



