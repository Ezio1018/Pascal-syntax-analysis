#include "compile.h"
void lexer(FILE *fp)
{   
    int currState=START;//定义现态为初态
    int num=0; //当前识别中的数字
    int len=0; //当前识别中的数字长度
    char identifier[MAXIDLEN+1];//当前识别中的标识符或者关键字
    int  index=0;   //当前识别中的标识符或关键字的下标

    ch=fgetc(fp);
    while (ch!=EOF)
    {
        switch (currState)
        {
        case START:
            if(ch==' '||ch=='\t'||ch=='\r'||ch=='\n'){

            }
            else if(ch=='{'){//识别注释
                currState=INCOMMENT;
            }
            else if(isdigit(ch)){
                currState=INNUM;//从START态接收到字符则跳为INBUM态
                num=ch-'0';//存储该数字内容
                len++;//数字长度加1
            }
            else if(isalpha(ch)){
                currState=INID;
                identifier[index++]=ch;
            }
            else if(ch==':'){
                currState=INBECOMES;
            }
            else if(ch=='>'){
                currState=GT;
            }
            else if(ch=='<'){
                currState=LT;
            }
            else{
                currState=START;
                int sym=0;
                int flag=0;
                for (int i=1; i <= NSW; i++)
                {
                    if (ch == csym[i]){
                        sym=symno[i];
                        flag=1;
                    }
                }  
                if(flag=1){
                    printf("(%d,%c)\n", sym, ch);
                    innernum[loc++]=sym;
                    character[loc1][0]=ch;
                    character[loc1++][1]='\0';
                }
                else{
                    printf("错位字符");
                    exit(1);
                }
            }
        break;
        case INCOMMENT:
            if(ch=='}'){//读到右括号表示注释结束
                currState=START;
            }
        break;
        case INNUM:
            if(isdigit(ch)){
                num=num*10+ch-'0';
            }
            else if(isalpha(ch)){
                printf("error");
                len=0;
                num=0;
                // exit(1);
            }
            else{
                ungetc(ch,fp);
                currState=START;
                printf("(33,%d)\n",num);
                innernum[loc++]=33;
                strcpy(character[loc1++],"shuzi");
                len=0;
                num=0;
                
            }
        break;
        case INID:{
            if(isalpha(ch) || isdigit(ch)){
                //如果后面接字符串或者数字继续往标识符数组中添加
                if(index>MAXIDLEN){
                    printf("标识符过长");
                    exit(1);
                }
                identifier[index++]=ch;
            }
            else{
                int sym=34;
                currState=START;
                ch=ungetc(ch,fp);
                identifier[index]='\0';
                for(int i=1;i<=NRW;i++){
                    int flag=1;//判断是否找到
                    for(int j=0;;j++){
                    //如果到达'\0'说明匹配
                    if(reserveWord[i][j]=='\0' && identifier[j]=='\0'){
                        sym=reserveno[i];
                        flag=0;
                        break;
                        }
                    //如果不一致说明不匹配
                    if(reserveWord[i][j]!=identifier[j])break;
                    }
                    if(flag=0){
                        break;
                    }
                }
                printf("(%d,%s)\n", sym,identifier);
                innernum[loc++]=sym;
                strcpy(character[loc1++],identifier);
                index=0;

                
            }
        }
        break;
        case INBECOMES:
            if(ch=='='){
                currState=BECOMES;
            }
            else{
                currState=START;
                ch = ungetc(ch, fp); // 回退该字符，重新识别
                printf("(%d,:)\n",29);
                innernum[loc++]=29;
                strcpy(character[loc1++],":");
            }
        break;
        case BECOMES:
            currState=START;
            ungetc(ch, fp); // 回退该字符，重新识别
            printf("(%d,:=)\n", 30);
            innernum[loc++]=30;
            // character[loc++]=":=";
            strcpy(character[loc1++],":=");

        break;
        case GT:
            if(ch=='='){
                currState=GE;
            }
            else{
                currState = START;
                ch = ungetc(ch, fp); // 回退该字符，重新识别
                printf("(%d,>)\n", 24);
                innernum[loc++]=24;
                strcpy(character[loc1++],">");
            }
        break;
        case LT:
            if(ch=='='){
                currState=LE;
            }
            else if(ch=='>'){
                currState=LG;
            }
            else{
                currState = START;
                ch = ungetc(ch, fp); // 回退该字符，重新识别
                printf("(%d,<)\n", 22);
                innernum[loc++]=22;
                strcpy(character[loc1++],"<");

                
            }
        break;
        case LG:
            currState = START;
            ch = ungetc(ch, fp); // 回退该字符，重新识别
            printf("(%d,<>)\n", 21);
            innernum[loc++]=21;
            strcpy(character[loc1++],"<>");


        break;
        case GE:
            currState = START;
            ch = ungetc(ch, fp); // 回退该字符，重新识别
            printf("(%d,>=)\n", 25);
            innernum[loc++]=25;
            strcpy(character[loc1++],">=");


        break;
        case LE:
            currState = START;
            ch = ungetc(ch, fp); // 回退该字符，重新识别
            printf("(%d,<=)\n", 23);
            innernum[loc++]=23;
            strcpy(character[loc1++],"<=");



        break;
        }
        ch=fgetc(fp);
    }
    if(index!=0){
        int sym=34;
        identifier[index]='\0';
        for(int i=1;i<=NRW;i++){
            int flag=1;//判断是否找到
            for(int j=0;;j++){
            //如果到达'\0'说明匹配
            if(reserveWord[i][j]=='\0' && identifier[j]=='\0'){
                sym=reserveno[i];
                flag=0;
                break;
                }
            //如果不一致说明不匹配
            if(reserveWord[i][j]!=identifier[j])break;
            }
            if(flag=0){
                break;
            }
        }
        printf("(%d,%s)\n", sym,identifier);
        innernum[loc++]=sym;
        strcpy(character[loc1++],identifier);
        index=0;
    }
}

void error(){
    printf("第%d\n个字符",n-1);
    printf("字符内码%d\n",innernum[n-1]);
    printf("字符值%s\n",character[n-1]);
    exit(1);
}
void praser(){
    program();
}
void program()
{
    printf("program-->proghead block.\n");
    proghead();
    block();
    if(innernum[n]!=26)
        error();
}
void proghead()
{
  
    printf("<程序首部>→PROGRAM 标识符；\n");
    if(innernum[n++]==1){
        if(innernum[n++]==34){
            if(innernum[n++]==28){
            }
            else{
                printf("proghead\n");    
                error();
            }
        }
        else{
            printf("proghead\n");    
            error();
        }
    }
    else{
        printf("proghead\n");    
        error();
    }
}
void block()
{
    printf("program-->consexpl varexl procdefi compsent\n");
    consexpl();
    varexl();
    procdefi();
    compsent();
}
void consexpl()
{
    printf("consexpl-->CONST<consdefi><conssuff> |ε\n");
    if(innernum[n]==2){
        n+=1;
        consdefi();
        conssuff();
        if(innernum[n++]!=28){
            error();
        }
    }
}
void consdefi()
{
    printf("consdefi-->标识符=无符号整数\n");
    if(innernum[n]==34&&innernum[n+1]==20&&innernum[n+2]==33){
        n+=3;
    }
    else{
        error();
        return;
    }
}
void conssuff()
{
    if(innernum[n]==27){
    printf("常量定义后缀-->,<常量定义><常量定义后缀> |\n");
        n+=1;
        consdefi();
        conssuff();
    }
}

void varexl()
{
    if(innernum[n]==3){
    printf("<变量说明部分>→VAR<变量定义><变量定义后缀> |ε\n");
        n=n+1;
        vandefi();
        varsuff();
    }
}

void vandefi()
{
    if(innernum[n++]==34){
    printf("<变量定义>→标识符<标识符后缀>：<类型>；\n");
    idsuff();
    if(innernum[n]==29){
        n+=1;
        typeil();
        if(innernum[n]==28){
            n+=1;
        }
        else{
            printf("vandefi\n");
            error();
        }
    }
    else{
        printf("vandefi\n");
        error();
    }
    }

}

void idsuff()
{
    if(innernum[n]==27){
        printf("<标识符后缀>→，标识符<标识符后缀> |ε\n");
        n+=1;
        if(innernum[n++]==34){
        idsuff();
        }
        else{
            printf("idsuff\n");
            error();
        }
    }
}

void typeil()
{
    if(innernum[n]==4||innernum[n]==5){
    printf("<类型>→INTEGER | LONG\n");

        n+=1;
    }
    else{
        flagerror=1;
    }
    if(flagerror==1)
    {
        printf("typeil\n");
        error();
        return;
    }
}

void varsuff()
{
    if(innernum[n]==34){
    printf("<变量定义后缀>→<变量定义><变量定义后缀> |ε\n");

        vandefi();
        varsuff();
    }
    
}
void procdefi()
{
    if(innernum[n]==6){
    printf("<过程说明部分>→<过程首部><分程序>；<过程说明部分后缀>|ε\n");

        n+=1;
        procedh();
        block();
        if(innernum[n]==28){
            n+=1;
        }
        else{
            printf("procedh\n");
            error();
        }
        procsuff();
    }
}
void procedh()
{
    if(innernum[n++]==6){
    printf("<过程首部>→PROCEDURE标识符<参数部分>；\n");

    argument();
    if(innernum[n++]==28){
    }
    else{
        printf("procedh\n");
        error();
    }
    }
    else{
        printf("procedh\n");
        error();
    }
}
void argument()
{
    
    if(innernum[n]==31){
    printf("<参数部分>→(标识符： <类型>)|ε\n");

        n+=1;
        if(innernum[n++]==34){
            if(innernum[n++]==29){
                typeil();
            }
            else{
                printf("argument\n");
                error();
            }
        }
        else{
            printf("argument\n");
            error();
        }
    }
}

void procsuff()
{
    if(innernum[n]==6){
    printf("<过程说明部分后缀>→<过程首部><分程序>；<过程说明部分后缀>|ε\n");

    procedh();
    block();
    if(innernum[n++]!=28){
        printf("procsuff\n");
        error();
    }
    procsuff();
    }
}
void compsent()
{
    if(innernum[n++]==13){
    printf("<复合语句>→BEGIN<语句><语句后缀>END\n");

        sentence();
        sentsuff();
        if(innernum[n++]!=14){
            printf("compsent1\n");
            error();
        }
    }
    else{
        printf("compsent2\n");
        error();
    }
}
void sentence()
{
    printf("<语句>→<赋值或调用语句>|<条件语句>|<当型循环语句>|<读语句>|<写语句>|<复合语句>|ε\n");
    if(innernum[n]==34)
    {assipro();return;}
    if(innernum[n]==7)
    {ifsent();return;}
    if(innernum[n]==9)
    {whilesent();return;}
    if(innernum[n]==11)
    {read();return;}
    if(innernum[n]==12)
    {write();return;}
    if(innernum[n]==13)
    {compsent();return;}
}
void assipro()
{
    if(innernum[n++]=34){
    printf("<赋值或调用语句>→标识符<后缀>\n");

        suffix();
    }
    else{
        printf("assipro\n");    
        error();
    }
  
}
void ifsent()
{
    if(innernum[n++]==7){
    printf("<条件语句>→IF<条件>THEN<语句>\n");

        conditio();
        if(innernum[n++]==8){
            sentence();
        }
        else{
            printf("ifsent\n");    
            error();
        }
    }
    else{
        printf("ifsent\n");    
        error();
    }
}
void whilesent()
{
    if(innernum[n++]==9){
    printf("<当型循环语句>→WHILE<条件>DO <语句>\n");

        conditio();
        if(innernum[n++]=10){
            sentence();
        }
        else{
            printf("whilesent\n");    
            error();
        }
    }
    else{
        printf("whilesent\n");    
        error();
    }
}

void read()
{
    if(innernum[n++]==11){
    printf("<读语句>→READ（标识符<标识符后缀>）\n");
        if(innernum[n++]==31){
            if(innernum[n++]==34){
                idsuff();
                if(innernum[n++]==32){

                }
                else{
                printf("read\n");    
                    error();
                }
            }
            else{
                printf("read\n");    
                error();
            }
        }
        else{
            printf("read\n");    
            error();
        }
    }
    else{
        printf("read\n");    
        error();
    }
}
void write()
{
    if(innernum[n++]==12){
    printf("<写语句>→WRITE（<表达式><表达式后缀>）\n");

        if(innernum[n++]==31){
            express();
            exprsuff();
            if(innernum[n++]==32){

            }
            else{
                printf("write\n");    
                error();
            }
        }
        else{
            printf("write\n");    
            error();
        }
    }
    else{
        printf("write\n");    
        error();
    }
}


void suffix()
{
    printf("<后缀>→：=<表达式>|(<表达式>)|ε\n");
    if(innernum[n]==30){
        n+=1;
         express();
    }
    else if(innernum[n]==31){
        n+=1;
        express();
        if(innernum[n++]!=32){
            printf("suffix\n");    
            error();
        }
    }
    else{
        
    }
}
void express()
{
    printf("<表达式>→+<项><项后缀>|-<项><项后缀>|<项><项后缀>\n");
    if(innernum[n]== 16){
        n+=1;
        term();
        termsuff();
    }
    else if(innernum[n]==17){
        n+=1;
        term();
        termsuff();
    }
    else {
        term();
        termsuff();
    }
}
void term()
{
    printf("<项>→<因子><因子后缀>\n");
    factor();
    factsuff();

}
void termsuff()
{
    if(innernum[n]==17||innernum[n]==16){
    printf("<项后缀>→<加型运算符><项><项后缀>|ε\n");

        addoper();
        term();
        termsuff();
    }
}

void factor()
{
    printf("<因子>→标识符|无符号整数|（<表达式>）\n");
    if(innernum[n]==33||innernum[n]==34){
        n=n+1;
    }
    else if(innernum[n]==31){
        n=n+1;
        express();
        if(innernum[n]==32){

        }
        else{
            printf("factor\n");    
            error();
        }
    }
    else{
        printf("factor\n");    
        error();
    }
}

void factsuff()
{
     if(innernum[n]==18||innernum[n]==19){
        printf("<因子后缀>→<乘型运算符><因子><因子后缀>|e\n");
        muloper();
        factor();
        factsuff();
    }
    
}

void muloper()
{
    printf("<乘型运算型>→*|/\n");
    n=n+1;
}

void addoper()
{
    printf("<加型运算符>→+|-\n");
    n+=1;
}

void sentsuff()
{
    if(innernum[n]==28){
        printf("<语句后缀>→；<语句><语句后缀>|ε\n");
        n=n+1;
        sentence();
        sentsuff();
    }
}

void conditio()
{
    printf("<条件>→<表达式><关系运算符><表达式>|ODD<表达式>\n");
    if(innernum[n]==15){
        express();
    }
    else{
        express();
        respoper();
        express();
    }
}

void respoper()
{
    printf("<关系运算符>→ =|<>|<|<=|>|>=\n");
    if(innernum[n]==20||innernum[n]==21|innernum[n]==22||innernum[n]==23||innernum[n]==24||innernum[n]==25){
        n+=1;
    }
    else{
        printf("respoper\n");    
        error();
    }
}


void exprsuff()
{
    printf("<表达式后缀>→，<表达式><表达式后缀>|ε\n");
    if(innernum[n]==27){
        n+=1;
        express();
        exprsuff();
    }
}

int main()
{
    //获取待检验文件的指针
    FILE *fp = fopen("compile\\source.txt", "r");
    if (!fp)
    {
        printf("文件不存在");
    }
    lexer(fp);
    printf("字符总数为%d\n",loc);
    praser();
    return 0;
}
