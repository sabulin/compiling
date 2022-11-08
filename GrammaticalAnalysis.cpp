#include <iostream>
#include <iomanip>
#include <string.h>
#include "datastructure.h"


using namespace std;

#define MAX 10000



// ����ʽ
Production productions[100];  // ����ʽ����
int productionTop = 0;  // ����ʽ��������±�

// ���ս��
string vtn[100];  // ���ս������
int vtnTop = 0;  // ���ս����������±�

// �ս��
string terminal[100];  // ���ս������
int terminalTop = 0;

// ״̬
State CC[100];  // ��Ŀ���淶��
int CCTop = 0;  // ����±�
State firstState;  // ��ʼ״̬

// ������
AnalysisCell actionTable[100][100];  // action ��
AnalysisCell gotoTable[100][100];  // goto ��

// FIRST ��
Collection FIRST[100];  // ���ս���±��Ӧ�� first ��
int firstTop = 0;  // ����β

// FOLLOW ��
Collection FOLLOW[100];  // ���ս���±��Ӧ�� follow ��
int followTop = 0;  // ����β

// ����item����
Item* copyItems(State target){
    Item *newArr = (Item*)malloc(100 * sizeof(Item));
    for(int i = 0; i < target.top; i++){
        newArr[i] = target.items[i];
    }
    return newArr;
}

// �ж�ָ���ַ����ǲ����ս��
bool isNonTerminal(string target){
    for(int i = 0; i < vtnTop; i++){
        if(target.compare(vtn[i]) == 0){
            return true;
        }
    }
    return false;
}

// �ж�һ��״̬���item�Ƿ��ظ�
bool itemRepeat(State state, int pId, int idx){
    for(int i = 0; i < state.top; i++){
        if(state.items[i].pId == pId && state.items[i].idx == idx){
            return true;
        }
    }
    return false;
}

// �Ƿ���ڲ���ʽ����ָ���ַ����Ĳ���ʽ��������id
int* existLeftPro(string target){
    int* result = (int*)malloc(sizeof(int) * productionTop);
    int idx = 0;
    // ��ʼ��-1
    for(int i = 0; i < productionTop; i++){
        result[i] = -1;
    }

    // ��ȡ��Ϊָ�����ս���Ĳ���ʽ
    for(int i = 0; i < productionTop; i++){
        if(productions[i].leftPart.compare(target) == 0){
            *(result + (idx++)) = i;
        }
    }

    return result;
}

// ��ȡ�հ�
State CLOSURE(State target){
    State newState;
    newState.items = copyItems(target);
    newState.top = target.top;
    //newState.id = target.id + 1;

    // �ж��Ƿ�ֹͣ
    Item *itemStack = copyItems(target);  // ������itemջ
    int stackTop = target.top;  // ջ��

    while(stackTop != 0){
        // ��ջ
        stackTop--;
        int idx =  itemStack[stackTop].idx;
        Production tempPro = productions[itemStack[stackTop].pId];
        // �жϸ�item�Ƿ�����չ
        if(idx < tempPro.rightPartLength){
            string tempVT = tempPro.rightPart[idx + 1];  // ���ұߵĵ�һ���ַ�
            if(isNonTerminal(tempVT)){  // ����Ƿ��ս��
                int* pId = existLeftPro(tempVT);
                for(int i = 0; i < productionTop; i++){
                    int id = *(pId + i);
                    if(id != -1 && !itemRepeat(newState, id, -1)){
                        // ����µ�item������������
                        newState.items[newState.top].pId = id;
                        newState.items[newState.top].idx = -1;
                        // ��ջ
                        itemStack[stackTop++] = newState.items[newState.top];
                        newState.top++;
                    }
                }
            }
        }
    }

    return newState;
}

// GOTO
State GOTO(State state, string x){
    State j;
    j.top = 0;
    //cout << "q" << endl;
    for(int i = 0; i < state.top; i++){
        int pId = state.items[i].pId;
        int idx = state.items[i].idx;
        if(productions[pId].rightPart[idx + 1].compare(x) == 0){  // �����һ��������Ŀ�����
            j.items[j.top].idx = idx + 1;
            j.items[j.top].pId = pId;
            j.top++;
        }
    }
    //cout << "w" << endl;

    return CLOSURE(j);
}

// �������״̬�Ƿ�һ��
bool checkTwoState(State s1, State s2){
    // item��������һ���϶���һ��
    if(s1.top != s2.top){
        return false;
    }

    // ���item�Ƿ�һ��
    for(int i = 0; i < s1.top; i++){
        if(!itemRepeat(s2, s1.items[i].pId, s1.items[i].idx)){
            return false;
        }
    }

    return true;
}

// �����Ŀ���淶���Ƿ��Ѿ���һ��״̬
bool checkItemInCC(State state){
    //cout << "s" << endl;
    for(int i = 0; i < CCTop; i++){
        if(checkTwoState(CC[i], state)){
            return true;
        }
    }
    //cout << "sssssssssssssssss" << endl;

    return false;
}

// ��ӡ״̬��Ŀ
void printState(State tempState){
    for(int j = 0; j < tempState.top; j++){
            Item tempItem = tempState.items[j];
            Production tempPro = productions[tempItem.pId];
            cout << tempPro.leftPart << "-->";
            for(int k = 0; k <= tempPro.rightPartLength + 1; k++){
                if(tempItem.idx == k - 1){
                    cout << ".";
                }
                if(k != tempPro.rightPartLength + 1){
                    cout << tempPro.rightPart[k];
                }
            }
            cout << endl;
        }
        cout << "--------------------------------" << endl;
}

// ��ȡ��Ŀ���淶��
void getCanonicalCollection(){
    // ��ȡ�ع��ķ��¼Ӳ���ʽ����Ŀ���磺S'->.S��
    firstState.items[0].pId = 0;
    firstState.items[0].idx = -1;
    firstState.top = 1;
    // ��ȡ����Ŀ�ıհ���Ϊ��ʼ״̬
    CC[CCTop++] = CLOSURE(firstState);

    State stateStack[MAX];
    stateStack[0] = CC[CCTop - 1];
    int stackTop = 1;

    while(stackTop != 0){
        // ��ջ
        stackTop--;
        State topState = stateStack[stackTop];
        // �Է��ս��
        for(int i = 0; i < vtnTop; i++){
            State temp = GOTO(topState, vtn[i]);
            if(temp.top !=0 && !checkItemInCC(temp)){  // һ��״̬����Ϊ�գ��統GOTO(i, "#")ʱ
                stateStack[stackTop++] = temp;
                CC[CCTop++] = temp;
            }
        }
        // ���ս��
        for(int i = 0; i < terminalTop; i++){
            State temp = GOTO(topState, terminal[i]);
            if(temp.top !=0 && !checkItemInCC(temp)){
                stateStack[stackTop++] = temp;
                CC[CCTop++] = temp;
            }
        }
    }
}

// ��ȡһ��״̬�����
int getStateId(State target){
    for(int i = 0; i < CCTop; i++){
        if(checkTwoState(CC[i], target)){
            return i;
        }
    }

    printf("get state id error!!!!!!!!!\n");
    return -1;
}

// ��ȡһ�����ս��id
int getVtnId(string target){
    for(int i = 0; i < vtnTop; i++){
        if(target.compare(vtn[i]) == 0){
            return i;
        }
    }

    printf("get vtn id error!!!!!!!!!\n");
    return -1;
}

// ��ȡһ���ս��id
int getTerminalId(string target){
    for(int i = 0; i < terminalTop; i++){
        if(target.compare(terminal[i]) == 0){
            return i;
        }
    }


    return -1;
}

// ��ʼ��������
void initAnalysisTable(){
    // goto ��
    for(int i = 0; i < CCTop; i++){
        for(int j = 0; j < vtnTop; j++){
            gotoTable[i][j].direct = -1;
            gotoTable[i][j].op = "*";
        }
    }
    // action ��
    for(int i = 0; i < CCTop; i++){
        for(int j = 0; j < terminalTop; j++){
            actionTable[i][j].direct = -1;
            actionTable[i][j].op = "*";
        }
    }
}

// ���follow��first�����Ƿ���һ���ַ�
bool checkCollection(Collection cl, int vId){
    for(int i = 0; i < cl.top; i++){
        if(cl.vts[i] == vId){
            return true;
        }
    }

    return false;
}

// ��һ�����ϼӵ���һ���У�����follow��ָ�Ƿ����follow����
void addCollection(int id, Collection target, bool follow){
    // �ӵ�follow��
    if(follow){
        for(int i = 0; i < target.top; i++){
            if(!checkCollection(FOLLOW[id], target.vts[i])){
                FOLLOW[id].vts[FOLLOW[id].top++] = target.vts[i];
            }
        }
    } else {
        // �ӵ�first��
        for(int i = 0; i < target.top; i++){
            if(!checkCollection(FIRST[id], target.vts[i])){
                FIRST[id].vts[FIRST[id].top++] = target.vts[i];
            }
        }
    }
}

// ��ȡ������ĳһ���ս����FIRST��
Collection getFIRST(int vId){
    // �����first���Ѿ����ˣ���ֱ�ӷ���
    if(FIRST[vId].top != 0){
        return FIRST[vId];
    }
    // ������first��
    for(int i = 0; i < productionTop; i++){
        // �������ʽ������v
        if(productions[i].leftPart.compare(vtn[vId]) == 0){
            // �������ʽ�Ҳ��ĵ�һ���������Լ���Ҫ������Ȼ���ǽ�FIRST[X]�ŵ�FIRST[X]�У�����ѭ��
            if(productions[i].rightPart[0].compare(vtn[vId]) == 0){
                continue;
            }
            // �������ʽ�Ҳ���һ���������ս�����ͰѸ��ս���ӽ�ȥ
            int id = getTerminalId(productions[i].rightPart[0]);
            if(id != -1){
                if(!checkCollection(FIRST[vId], id)){
                    FIRST[vId].vts[FIRST[vId].top++] = id;
                }
            } else {
                id = getVtnId(productions[i].rightPart[0]);
                if(id == -1){
                    cout << "cant find symbol [" << productions[i].rightPart[0] << "]" << endl;
                    return FIRST[vId];
                }
                // ����ʽ�Ҳ���һ�������Ƿ��ս����������first������
                addCollection(vId, getFIRST(id), false);
            }
        }
    }

    return FIRST[vId];
}

// ��ʼ��FIRST��
void initFIRST(){
    // Ϊÿ�����ս������first��
    for(int i = 0; i < vtnTop; i++){
        getFIRST(i);
    }
}

// ��ȡfollow��
Collection getFOLLOW(int vId){
    // �����follow���Ѿ����ˣ���ֱ�ӷ���
    if(FOLLOW[vId].top != 0){
        return FOLLOW[vId];
    }
    if(vId == 0){
        // �����ķ��¼ӵĲ���ʽ���Ҳ��Դ� #���� S' -> S �е� S
        FOLLOW[0].vts[FOLLOW[0].top++] = getTerminalId("#");
    }
    // ����follow��
    for(int i = 1; i < productionTop; i++){
        for(int j = 0; j <= productions[i].rightPartLength; j++){
            if(vtn[vId].compare(productions[i].rightPart[j]) == 0){
                int id = -1;
                // ����÷��ս���ڲ���ʽ�Ҳ����
                if(j == productions[i].rightPartLength){
                    // �Ҳ����һ�����Ų������Լ�����Ȼ����ѭ��
                    if(vtn[vId].compare(productions[i].leftPart) == 0){
                        continue;
                    }
                    id = getVtnId(productions[i].leftPart);
                    if(id == -1){
                        cout << "unknown non-terminal v: [" << productions[i].leftPart << "]" << endl;
                        return FOLLOW[vId];
                    }
                    // �Ѳ���ʽ�󲿷��ս����follow������
                    addCollection(vId, getFOLLOW(id), true);
                } else if((id = getTerminalId(productions[i].rightPart[j + 1])) != -1) {
                    // ����÷��ս������һ���������ս����ֱ�Ӽӵ�follow����
                    if(!checkCollection(FOLLOW[vId], id)){
                        FOLLOW[vId].vts[FOLLOW[vId].top++] = id;
                    }
                } else if((id = getVtnId(productions[i].rightPart[j + 1])) != -1){
                    // ����÷��ս����һ����������һ�����ս�����ͰѸ÷��ս����follow������
                    addCollection(vId, getFIRST(id), true);
                }
            }
        }
    }

    return FOLLOW[vId];
}

// ��ʼ��FOLLOW��
void initFOLLOW(){
    // Ϊÿ�����ս������follow��
    for(int i = 0; i < vtnTop; i++){
        getFOLLOW(i);
    }
}

// FOLLOW ���Ƿ����ĳ��Ԫ��
bool inFOLLOW(string v, int target){
    // ��ȡ���ս��id
    int vId = getVtnId(v);
    if(vId == -1){
        cout << "get non-terminal v error" << endl;
        return false;
    }
    // ��ȡfollow��
    Collection flc = FOLLOW[vId];
    // �ж϶�Ӧ�ս���Ƿ��ڷ��ս��v��follow����
    for(int i = 0; i < flc.top; i++){
        if(target == flc.vts[i]){
            return true;
        }
    }

    return false;
}

// ��ӡ������
void PrintAnalysisTable(){
    cout << std::setw(6) << "";
    for(int i = 0; i < terminalTop; i++){
        cout << std::setw(6) << terminal[i];
    }
    for(int i = 0; i < vtnTop; i++){
        cout << std::setw(6) << vtn[i];
    }
    cout << endl;
    for(int i = 0; i < CCTop; i++){
        cout << i << "   ";
        for(int j = 0; j < terminalTop; j++){
            if(actionTable[i][j].direct == -1){
                cout << std::left << std::setw(6) << actionTable[i][j].op;
            } else {
                string display = actionTable[i][j].op + to_string(actionTable[i][j].direct);
                cout << std::left << std::setw(6) << display;
            }
        }
        for(int j = 0; j < vtnTop; j++){
            if(gotoTable[i][j].direct == -1){
                cout << std::left << std::setw(6) << gotoTable[i][j].op;
            } else {
                string display = gotoTable[i][j].op + to_string(gotoTable[i][j].direct);
                cout << std::left << std::setw(6) << display;
            }
        }
        printf("\n");
    }
}

void printAnalysisTable(){

    cout << "-------------------- ������ACTION�� ----------------------------" << endl;
    cout << std::setw(6) << "";
    for(int i = 0; i < terminalTop; i++){
        cout << std::setw(6) << terminal[i];
    }
    for(int i = 0; i < CCTop; i++){
        cout << i << "   ";
        for(int j = 0; j < terminalTop; j++){
            if(actionTable[i][j].direct == -1){
                cout << std::left << std::setw(6) << actionTable[i][j].op;
            } else {
                string display = actionTable[i][j].op + to_string(actionTable[i][j].direct);
                cout << std::left << std::setw(6) << display;
            }
        }

        printf("\n");
    }
    cout<<endl;
    cout << "-------------------- ������GOTO�� ----------------------------" << endl;
    for(int i = 0; i < vtnTop; i++){
        cout << std::setw(6) << vtn[i];
    }
    cout << endl;
    for(int i = 0; i < CCTop; i++){
        cout << i << "   ";

        for(int j = 0; j < vtnTop; j++){
            if(gotoTable[i][j].direct == -1){
                cout << std::left << std::setw(6) << gotoTable[i][j].op;
            } else {
                string display = gotoTable[i][j].op + to_string(gotoTable[i][j].direct);
                cout << std::left << std::setw(6) << display;
            }
        }
        printf("\n");
    }
}

// ���������
void developAnalysisTable(){
    // ����״̬
    for(int i = 0; i < CCTop; i++){
        // ��ǰ״̬
        State tempState = CC[i];
        // ����״̬�е���Ŀ
        for(int j = 0; j < tempState.top; j++){
            // ��ǰ��Ŀ
            Item tempItem = tempState.items[j];
            // action �� goto
            if(tempItem.idx < productions[tempItem.pId].rightPartLength){
                string tempEle = productions[tempItem.pId].rightPart[tempItem.idx + 1];
                // �ս��
                int eleId = getTerminalId(tempEle);
                // ��������ս������goto��
                if(eleId == -1){
                    eleId = getVtnId(tempEle);
                    if(eleId == -1){
                        cout << "cant recognize the symbol [" << tempEle << "]" << endl;
                        return;
                    }
                    // ��ȡ��һ��״̬
                    State nextState = GOTO(tempState, tempEle);
                    // ��ȡ״̬id
                    int nextStateId = getStateId(nextState);
                    if(nextStateId == -1){
                        printf("cant get state id!!!!!!!!!!!\n");
                        return;
                    }
                    // ���
                    gotoTable[i][eleId].direct = nextStateId;
                    gotoTable[i][eleId].op = "gt";
                } else {
                    // action ��
                    // ��ȡ��һ��״̬
                    State nextState = GOTO(tempState, tempEle);
                    // ��ȡ״̬id
                    int nextStateId = getStateId(nextState);
                    if(nextStateId == -1){
                        printf("cant get state id!!!!!!!!!!!\n");
                        return;
                    }
                    // ���
                    actionTable[i][eleId].direct = nextStateId;
                    actionTable[i][eleId].op = "s";
                }
            } else {
                // acc
                if(tempItem.pId == 0 && tempItem.idx == 0){
                    actionTable[i][getTerminalId("#")].op = "acc";
                } else {
                    // ��Լ
                    for(int m = 0; m < terminalTop; m++){
                        //
                        // �������Ƿ���SLR�Ĺؼ���SLR��Ҫ�ж���һ�����ս���Ƿ��ڸñ�����follow���У�LR�Ͳ���Ҫ��ֱ��true����
                        //
                        if(inFOLLOW(productions[tempItem.pId].leftPart, m)){
                        //if(true){
                            actionTable[i][getTerminalId(terminal[m])].op = "r";
                            actionTable[i][getTerminalId(terminal[m])].direct = tempItem.pId;
                        }
                    }
                }
            }
        }
    }
}

// ��ӡջ�����ַ���
void printSymbolString(WORD *symbolStack, int stackTop, WORD *symbolNotInStack, int waitIn, int notInTop){
    string in="{";
    for(int i = 0; i < stackTop; i++){
        in = in + " " + symbolStack[i].key + " ";
    }
    in+="}";
    string out="{";
    for(int i = waitIn; i < notInTop; i++){
        out = out + " " + symbolNotInStack[i].key + " ";
    }
    out+="}";
    cout << "����ջ: " << endl;
    cout << std::left << std::setw(50) << in;
    cout << std::setw(50) << out;
    cout << endl;
}

// ��ӡ״̬ջ
void printStateString(int *stateStack, int stackTop){
    string states="{";
    for(int i = 0; i < stackTop; i++){
        states = states + " " + to_string(stateStack[i]) + " ";
    }
    states+="}";
    cout << "״̬ջ: " << endl;
    cout << std::left << std::setw(50) << states;
    cout << endl;
}

// �������״̬
void finalize(){

}

// �﷨����
void analysis(WORD target[], int wordNum){
    WORD symbolStack[MAX];  // ����ջ
    int symbolStackTop = 0;  // ����ջ��
    int stateStack[MAX];  // ״̬ջ
    int stateStackTop = 0;  // ״̬ջ��

    // �����#
    target[wordNum].key = "#";
    target[wordNum].code = getTerminalId("#");
    wordNum++;

    // ��״̬0��#��ջ
    symbolStack[symbolStackTop].code = START_END;
    symbolStack[symbolStackTop].key = "#";
    symbolStackTop++;

    stateStack[stateStackTop++] = 0;

    // ��ʼ��Լ
    int wait = 0;

    cout << "------------------״̬����-------------------" << endl;
    printStateString(stateStack, stateStackTop);
    printSymbolString(symbolStack, symbolStackTop, target, wait, wordNum);
    cout << "---------------------------------------------" << endl;

    while(true){
        // ��ȡҪ��ȡ�ķ���
        WORD tempWord = target[wait++];
        int id = getTerminalId(tempWord.key);
        int currentState = stateStack[stateStackTop - 1];
        // �ж��ǲ����ս��
        if(id == -1){  // �Ǳ���
            id = getVtnId(tempWord.key);
            if(id == -1){
                cout << "cant recognize the symbol [" << tempWord.key << "]" << endl;
            }

            AnalysisCell cell = gotoTable[currentState][id];
            if(cell.op.compare("*") == 0){
                cout << "analysis error in goto table (" << currentState << "," << id << ")" << endl;
                return;
            }
            cout << "goto(" << currentState << ", " << tempWord.key << ") = " << cell.direct << endl;
            // ��״̬ѹ��ջ
            stateStack[stateStackTop++] = cell.direct;
            // ������ѹ��ջ
            symbolStack[symbolStackTop++] = tempWord;
        } else {  // �ս��
            AnalysisCell cell = actionTable[currentState][id];
            if(cell.op.compare("acc") == 0){
                symbolStack[symbolStackTop].key = "#";
                symbolStack[symbolStackTop].code = getTerminalId("#");
                symbolStackTop++;
                cout << "---------------------------------------------" << endl;
                printStateString(stateStack, stateStackTop);
                printSymbolString(symbolStack, symbolStackTop, target, wait, wordNum);
                cout << "---------------------------------------------" << endl;
                cout << "����������" << endl;
                return;
            }
            if(cell.op.compare("*") == 0){
                cout << "analysis error in action table (" << currentState << "," << id << ")" << endl;
                cout << "throw error at line " << tempWord.lineNum << ":" << tempWord.position << endl;
                //exit(0);
            }
            // �����ƽ�
            if(cell.op.compare("s") == 0){
                cout << "action(" << currentState << ", " << tempWord.key << ") = " << cell.op << to_string(cell.direct) << endl;
                stateStack[stateStackTop++] = cell.direct;
                symbolStack[symbolStackTop++] = tempWord;
            }
            // ��Լ
            if(cell.op.compare("r") == 0){
                // ��ȡ����ʽ
                Production tempProduction = productions[cell.direct];
                cout << "action(" << currentState << ", " << tempWord.key << ") = " << cell.op << to_string(cell.direct) << endl;
                cout << "use production: {" << tempProduction.leftPart << " -> ";
                for(int k = 0; k <= tempProduction.rightPartLength; k++){
                    cout << tempProduction.rightPart[k];
                }
                cout << "} to reduce" << endl;
                // ����ʶ��ķ��Ÿĳɲ���ʽ��
                wait = wait - 2;
//                target[wait] = createCode(cell.direct, tempProduction.leftPart, symbolStack, symbolStackTop);
                target[wait].key = tempProduction.leftPart;
                target[wait].code = getVtnId(tempProduction.leftPart);
                // ����ջ��״̬ջ��������ʽ�Ҳ����ȸ�Ԫ��
                stateStackTop = stateStackTop - tempProduction.rightPartLength - 1;
                symbolStackTop = symbolStackTop - tempProduction.rightPartLength - 1;
                //cout << endl << target[wait].addr << "-----------" << target[wait].variable << endl
            }
        }

        // ��ӡ��������
        printStateString(stateStack, stateStackTop);
        printSymbolString(symbolStack, symbolStackTop, target, wait, wordNum);
        cout << "---------------------------------------------" << endl;
    }
}



// ��ӡfirst��
void printFIRST(){
    cout << "----------------------- ������FIRST�� --------------------------" << endl;
    for(int i = 0; i < vtnTop; i++){
        cout << std::left << std::setw(7) << "{" + vtn[i] + "}" + "��FIRST���� : ";
        for(int j = 0; j < FIRST[i].top; j++){
            if(j==0) cout<<"{ ";
            cout <<   terminal[FIRST[i].vts[j]] + " ";
        }
        cout<<"}";
        cout << endl;
    }
}

// ��ӡfollow��
void printFOLLOW(){
    cout << "----------------------- ������FOLLOW�� --------------------------" << endl;
    for(int i = 0; i < vtnTop; i++){
        cout << std::left << std::setw(7) << "{" + vtn[i] + "}" + "��FOLLOW���� : ";
        for(int j = 0; j < FOLLOW[i].top; j++){
            if(j==0) cout<<"{ ";
            cout <<  terminal[FOLLOW[i].vts[j]] + " ";
        }
        cout<<"}";
        cout << endl;
    }
}


// �﷨������target�Ǵ�������Ŵ���wordNum��target�ķ��Ÿ���
void grammaticalAnalysis(WORD* target, int wordNum){
        // �趨����ʽ
        productions[productionTop].leftPart = "S'";
        productions[productionTop].rightPart[0] = {"S"};
        productions[productionTop].rightPartLength = 0;
        productionTop++;

//    S -> W(C){S}
        productions[productionTop].leftPart = "S";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"W"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"("};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"C"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {")"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"{"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"S"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"}"};
        productionTop++;

    //S -> WS
        productions[productionTop].leftPart = "S";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"W"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"S"};

        productionTop++;


        // S -> TE(A){S}
        productions[productionTop].leftPart="S";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"T"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"("};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"A"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {")"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"{"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"S"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"}"};
        productionTop++;


        // S ->
//        productions[productionTop].leftPart="S";
//        productions[productionTop].rightPartLength = -1;
//        productionTop++;

        //S -> W(C){S}W'{S}
        productions[productionTop].leftPart="S";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"W"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"("};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"C"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {")"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"{"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"S"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"}"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"K"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"{"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"S"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"}"};
        productionTop++;
//        //W -> else
        productions[productionTop].leftPart="K";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"else"};
        productionTop++;

        //A -> TE
        productions[productionTop].leftPart="A";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"T"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        productionTop++;

        productions[productionTop].leftPart = "W";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"if"};
        productionTop++;

        productions[productionTop].leftPart = "S";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"S"};
        productionTop++;

        productions[productionTop].leftPart = "S";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        //productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {";"};
        productionTop++;

        productions[productionTop].leftPart = "S";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {";"};
        productionTop++;

        productions[productionTop].leftPart = "W";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"while"};
        productionTop++;

        productions[productionTop].leftPart = "E";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"id"};
        productionTop++;

        productions[productionTop].leftPart = "E";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"num"};
        productionTop++;


        productions[productionTop].leftPart = "C";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"U"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        productionTop++;

        productions[productionTop].leftPart = "U";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"=="};
        productionTop++;

        productions[productionTop].leftPart = "U";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"<"};
        productionTop++;

        productions[productionTop].leftPart = "U";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {">"};
        productionTop++;

        productions[productionTop].leftPart = "E";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"O"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        //productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {";"};
        productionTop++;

        productions[productionTop].leftPart = "O";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"+"};
        productionTop++;

        productions[productionTop].leftPart = "O";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"-"};
        productionTop++;

        productions[productionTop].leftPart = "O";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"*"};
        productionTop++;

        productions[productionTop].leftPart = "O";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"/"};
        productionTop++;

        productions[productionTop].leftPart = "O";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"="};
        productionTop++;

        productions[productionTop].leftPart = "E";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"T"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"="};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"E"};
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {";"};
        productionTop++;

        productions[productionTop].leftPart = "T";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"int"};
        productionTop++;

        productions[productionTop].leftPart = "T";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"double"};
        productionTop++;

        productions[productionTop].leftPart = "T";
        productions[productionTop].rightPartLength = -1;
        productions[productionTop].rightPart[++productions[productionTop].rightPartLength] = {"boolean"};
        productionTop++;

        cout << "----------------�����ǲ���ʽ-----------------" << endl;
        for(int i = 0; i < productionTop; i++){
            cout << i << ". " << productions[i].leftPart << " -> ";
            for(int j = 0; j <= productions[i].rightPartLength; j++){
                cout << productions[i].rightPart[j];
            }
            cout << endl;
        }

        // �趨���ս��

        vtn[vtnTop++] = "S";
        vtn[vtnTop++] = "C";
        vtn[vtnTop++] = "W";
        vtn[vtnTop++] = "O";
        vtn[vtnTop++] = "U";
        vtn[vtnTop++] = "E";
        vtn[vtnTop++] = "T";
        vtn[vtnTop++] = "A";
        vtn[vtnTop++] = "K";
        //vtn[vtnTop++] = "S'";

        // �趨�ս��
        terminal[terminalTop++] = "id";
        terminal[terminalTop++] = "num";
        terminal[terminalTop++] = "==";
        terminal[terminalTop++] = ">";
        terminal[terminalTop++] = "<";
        terminal[terminalTop++] = "=";
        terminal[terminalTop++] = "+";
        terminal[terminalTop++] = "-";
        terminal[terminalTop++] = "*";
        terminal[terminalTop++] = "/";
        terminal[terminalTop++] = "if";
        terminal[terminalTop++] = "else";
        terminal[terminalTop++] = "while";
        terminal[terminalTop++] = "int";
        terminal[terminalTop++] = "double";
        terminal[terminalTop++] = "boolean";
        terminal[terminalTop++] = ";";
        terminal[terminalTop++] = "{";
        terminal[terminalTop++] = "}";
        terminal[terminalTop++] = "(";
        terminal[terminalTop++] = ")";
        terminal[terminalTop++] = "#";

        // ����first��
        initFIRST();
        printFIRST();

        // ����follow��
        initFOLLOW();
        printFOLLOW();

        // ��ȡ�淶��
        getCanonicalCollection();

        // ��ʼ��������
        initAnalysisTable();

        // ���������
        developAnalysisTable();

        cout << "---------------�����Ƿ�����------------------" << endl;
        printAnalysisTable();

        // ����

        analysis(target, wordNum);

}







