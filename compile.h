#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NRW 15//保留字个数
#define MAXIDLEN 10 //保留字的最长长度
#define NSW 11
char ch;
char csym[NSW+1] = {
	'!','+','-','*','/','=','.',',',';','(',')','#'
};
int symno[NSW+1]={0,16,17,18,19,20,26,27,28,31,32,35};
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
void lexer();
void praser();
void program();
void proghead();
void block();
void consexpl();
void consdefi();
void varexl();
void conssuff();
void vandefi();
void varsuff();
void procdefi();
void typeil();
void procedh();
void procsuff();
void assipro();
void sentence();
void suffix();
void ifsent();
void read();
void whilesent();
void idsuff();
void write();
void compsent();
void exprsuff();
void sentsuff();
void conditio();
void termsuff();
void express();
void term();
void factsuff();
void argument();
void factor();
void addoper();
void muloper();
void respoper();
void error();