#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include<malloc.h>

#define NRW 15//保留字个数
#define MAXIDLEN 10 //保留字的最长长度
#define NSW 11
char ch;
char csym[NSW+1] = {
	'.','+','-','*','/','=','.',',',';','(',')','#'
};
int symno[NSW+1]={0,16,17,18,19,20,26,27,28,31,32,35};//终结符号串
//定义各种状态
enum state {
	START,INNUM,INID,INBECOMES,BECOMES,
    GT,GE,NE,LT,LE,END,INCOMMENT,LG
};
char character[10000][100];
//保留字
char* reserveWord[NRW+1]={
    "",
    "program","const","var",
    "integer","long","procedure",
    "if","then","while","do",
    "read","write","begin","end","odd"
};
int reserveno[NRW+1]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
int loc=0;
int loc1=0;
int n=0;
int flagerror=0;
int innernum[10000];
int vnsign[10000];
int vnnum=34;
int vtnum=35;
int firstSet[200][200]={0};
int followSet[200][200]={0};
int M[200][200]={-1};
int temp_first[200]={0};

int express[200][200]={
{128,129,130,26,0},
{129,1,34,28,0},
{130,131,134,138,150,0},
{131,2,132,133,28,0},
{131,-1,0},
{132,34,20,33,0},
{133,27,132,133,0},
{133,-1,0},
{134,3,135,136,0},
{134,-1,0},
{135,34,147,29,137,28},
{147,27,34,147,0},
{147,-1,0},
{136,135,136,0},
{136,-1,0},
{137,4,0},
{137,5,0},
{138,139,130,28,140,0},
{138,-1,0},
{140,139,130,28,140,0},
{140,-1,0},
{139,6,34,158,28,0},
{141,142,0},
{141,144,0},
{141,145,0},
{141,146,0},
{141,148,0},
{141,150,0},
{141,-1,0},
{142,34,143,0},
{143,30,153,0},
{143,31,153,32,0},
{143,-1,0},
{144,7,152,8,141,0},
{145,9,152,10,141,0},
{146,11,31,34,147,32,0},
{148,12,31,153,149,32,0},
{149,27,153,149,0},
{149,-1,0},
{150,13,141,151,14,0},
{151,28,141,151,0},
{151,-1,0},
{152,153,161,153,0},
{152,15,153,0},
{153,16,155,154,0},
{153,17,155,154,0},
{153,155,154,0},
{154,159,155,154,0},
{154,-1,0},
{155,157,156,0},
{156,160,157,156,0},
{156,-1,0},
{157,34,0},
{157,33,0},
{157,31,153,32,0},
{159,16,0},
{159,17,0},
{160,18,0},
{160,19,0},
{161,20,0},
{161,21,0},
{161,22,0},
{161,23,0},
{161,24,0},
{161,25,0},
{158,31,34,29,137,32,0},
{158,-1,0},
};
int pronum=67;

struct Stack
{
    int mData[100];
    int mLen;
};
//初始化栈
void InitStack(Stack &S)
{
    S.mLen = 0;
}
//元素进栈
void Push(Stack &S,int item)
{
    S.mData[S.mLen++] = item;
}
//删除栈顶元素
int Pop(Stack &S)
{
    S.mLen--;
    return S.mData[S.mLen];
}
//返回栈顶元素
int  Peek(Stack &S)
{
    return S.mData[S.mLen-1];
}
//判断栈是否为空
bool EmptyStack(Stack &S)
{
    if(S.mLen == 0) return true;
    return false;
}
//清空栈
void Clear(Stack &S)
{
    for(int i = 0;i<S.mLen;++i)
    {
        Pop(S);
    }
}