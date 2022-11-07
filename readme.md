# 总体思路与架构

## 0. 预备工作
产生式
0. S' -> S
1. S -> W(C){S}
S -> W(C){S}W'{S}
3. W' -> else
2. S -> TE(A){S}
3. S ->
3. A -> TE
2. W -> if
3. S -> ES
4. S -> E
5. S -> E;
6. W -> while
7. E -> id
8. E -> num
9. C -> EUE
10. U -> ==
11. U -> <
12. U -> >
13. E -> EOE
14. O -> +
15. O -> -
16. O -> *
17. O -> /
18. O -> =
19. E -> TE=E;
20. T -> int
21. T -> double
22. T -> boolean

## 1. 词法分析

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
2. FOLLOW集相关设计

```c++
FirstOrFollowCollection  getFOLLOW(int vId) // 获取follow集

void initFOLLOW() // 初始化FOLLOW集
```

