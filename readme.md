# 总体思路与架构

## 0. 预备工作
产生式
0. S' -> S
1. S -> W(C){S}
2. S -> WS
3. S -> TE(A){S}
4. S -> W(C){S}K{S}
5. K -> else
6. A -> TE
7. W -> if
8. S -> ES
9. S -> E
10. S -> E;
11. W -> while
12. E -> id
13. E -> num
14. C -> EUE
15. U -> ==
16. U -> <
17. U -> >
18. E -> EOE
19. O -> +
20. O -> -
21. O -> *
22. O -> /
23. O -> =
24. E -> TE=E;
25. T -> int
26. T -> double
27. T -> boolean

## 1. 词法分析

### 1.1 数据结构设计
1. 单词设计
2. 符号表单元设计 

### 1.2 词法分析流程

设计一个scan函数，把每一个token识别处理出来，自己定义的标识符单独设置一个符号表

## 2. 语法分析

### 2.1 数据结构设计

1. 分析表单元设计
2. FIRST集和FOLLOW集设计
3. 项目设计
4. 产生式设计
5. 状态设计

### 2.2 重要函数设计

1. FIRST集相关函数设计
```c++
bool checkCollection(FirstOrFollowCollection  cl, int vId); // 检查follow或first集中是否有某个字符

void addCollection(int id, FirstOrFollowCollection  target, bool follow) // 将一个集合加到另一个中，参数follow是指是否加在follow集中

FirstOrFollowCollection  getFIRST(int vId); // 获取并产生某一非终结符的FIRST集

void initFIRST(); // 初始化FIRST集
```

计算非终结符的FIRST集：

对每个非终结符X遍历所有产生式，  
如果产生式右部第一个符号是自己，跳过；  
如果产生式右部是终结符，把它加入X的FIRST集；  
如果产生式右部是非终结符，把它的FIRST集加入X的FIRST集中  

2. FOLLOW集相关设计

```c++
FirstOrFollowCollection  getFOLLOW(int vId) // 获取follow集

void initFOLLOW() // 初始化FOLLOW集
```

把结束符#加入S的FOLLOW集中，   
对每个非终结符X，遍历产生式集；   
如果X在某个产生式右部最后，把产生式左部的FOLLOW加入X的FOLLOW集；  
如果X后面是终结符，把该终结符加入FOLLOW集中；  
如果X后面是非终结符，把该非终结符的FIRST集加入X的FOLLOW集


3. CLOSURE函数设计

把一个状态的所有项目压栈，然后不断出栈
找到一个项目圆点后的第一个字符，如果是非终结符，就遍历产生式，把左端和其一致的产生式右端在开始标好点后加入该状态，
同时进栈，这个过程要注意去重

4. GOTO函数设计
```c++
State GOTO(State state, string x)
```

遍历状态中的每个项目，检测每个项目右部有没有包含目标符号，如果包含，把圆点右移一位后的项目加入新的状态，
最后把这个新状态求闭包

5. getCanonicalCollection函数设计

S'->.S这个项目放进一个初始状态，把初始状态放进状态栈；
遍历所有终结符和非终结符，用GOTO函数不断生成新状态，一边存进状态数组一边压栈；
直到最后栈空为止

6. 构建分析表

遍历当前状态和项目，对项目圆点右部的首个符号X，
如果该项目不是一个规约项目，则：X是非终结符填GOTO表，X是终结符填ACTION表
如果是规约项目，是S'->.S则acc，否则根据SLR特性，要查产生式左部的FOLLOW集，非终结符在里面就填action表

7. 语法分析

初始化把0和#分别压进状态栈和符号栈；
把指针从输入串左边向右移动；
是终结符查action表，是移入则移入状态和符号，是规约则相应出栈，是acc则分析结束；
是非终结符查goto表，移入状态和符号
