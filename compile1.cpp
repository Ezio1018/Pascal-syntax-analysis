#include "compile1.h"

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



bool check(int X,int Y){
    if(firstSet[X][Y]==1){
        return false;
    }
    else{
        return true;
    }
}
bool check1(int X,int Y){
    if(followSet[X][Y]==1){
        return false;
    }
    else{
        return true;
    }
}
bool check2(int X,int Y){
    if(M[X][Y]==1){
        return false;
    }
    else{
        return true;
    }
}
bool haveEmpty(int X){
    if(X<=vtnum &&X>=0){//终结符
        return false;
    }
    else if(firstSet[X][0]==1){//e
        return true;
    }
    else{
        return false;
    }
}
void first(int X){ 
    if(X<=35&&X>=0){
        firstSet[X][X]=1;
    }
    for(int i=0;i<pronum;i++){
        if(express[i][0]==X){//找到对应的产生式
        int p=1;//第一个位置
        if(express[i][p]==-1)//如果是e
        {   
            firstSet[X][0]=1;
        }
        else{
            if(express[i][p]<=vtnum&&express[i][p]>0){//如果是终结符
                firstSet[X][express[i][p]]=1;
            }
            else//如果是非终结符
            {  
            // if(i==17){
            //     printf("1111111111111\n");
            //     }
            int flag1=0;
            for(int j=1;j<=vtnum;j++){
                if(firstSet[express[i][p]][j]==1){//查找FirstY的所有集合){
                    firstSet[X][j]=1;//填入FirstX中
                }
            }
            while (express[i][p+1]!=0)
            {
                if(haveEmpty(express[i][p])){
                    first(express[i][p+1]);                    
                    for(int j=1;j<=vtnum;j++){
                        if(firstSet[express[i][p+1]][j]==1){//查找FirstY的所有集合){
                            firstSet[X][j]=1;//填入FirstX中
                        }
                    }
                    p++;
                }
                else{
                    flag1=1;
                    break;
                }
            }
            if(flag1==0){//都可以推到空串
                firstSet[X][0]=1;
            }
            }
    }}
    }
}

void multi_first(int X[]){
    for(int i=0;i<200;i++){
        temp_first[i]=0;
    }
    first(X[1]);
    for(int j=1;j<=vtnum;j++){
        if(firstSet[X[1]][j]==1){//查找FirstY的所有集合){
            temp_first[j]=1;//填入FirstX中
        }
    }
    int p=1;
    int flag1=0;
    while (X[p+1]!=0)
    {
        if(haveEmpty(X[p])){
            first(X[p+1]);
            for(int j=1;j<=vtnum;j++){
                if(firstSet[X[p+1]][j]==1){//查找FirstY的所有集合){
                    temp_first[j]=1;//填入FirstX中
                }
            }
            p++;
        }
        else{
            flag1=1;
            break;
        }
       
    }
    if(flag1==0){//都可以推到空串
        temp_first[0]=1;
    }   
}


void follow1(int x){
    for(int zz=0;zz<200;zz++){
        followSet[zz][vtnum]=1;
    }
    for(int i=0;i<pronum;i++){
        int p=1;
        while (express[i][p]!=0)
        {
            p++;
        }
        int len=p;
        p=1;
        while (express[i][p]!=0)
        {
            if(express[i][p]==x){
            if(express[i][p+1]==0||haveEmpty(express[i][p+1])){
                for(int j=0;j<vtnum;j++){
                    if(followSet[express[i][0]][j]==1){//查找FirstY的所有集合){
                    followSet[x][j]=1;//填入FirstX中
                    }
                }
            }
            if(express[i][p+1]!=0){
             
                int a[200];
                for(int ii=p+1;ii<=len;ii++){

                    a[ii-p]=express[i][ii];
                  
                }
                multi_first(a);
                for(int kk=1;kk<vtnum;kk++){
                    if(temp_first[kk]==1){
                        
                        followSet[x][kk]=1;
                    }
                }
            }
            break;
            }
            p++;
        }

    }
}

void select(int i){
    int left=express[i][0];
    if(express[i][1]==-1){
        M[left][0]=i;
        for(int j=0;j<=vtnum;j++){
            if(followSet[left][j]==1){
                M[left][j]=i;
            }
        }
    }
    if(express[i][1]>0&&express[i][1]<=vtnum){
        M[left][express[i][1]]=i;
    }
    else{
        multi_first(express[i]);
        if(temp_first[0]==1){
            for(int j=0;j<=vtnum;j++){
            if(followSet[left][j]==1){
                M[left][j]=i;
            }
        }
        }
        for(int j=0;j<=vtnum;j++){
            if(temp_first[j]==1){
                M[left][j]=i;
            }
        }
    }
}

void table(){
    for(int i=0;i<pronum;i++){
        select(i);
    }
}

void work(){
    printf("\n分析过程:\n\n分析栈              输入串 \n ");
    int flag=1;
    Stack s1=Stack();//两个栈
    Stack s2=Stack();
    Push(s1,-2);
    Push(s1,128);
    Push(s2,-2);
    for(int i=loc-1;i>=0;i--){
        Push(s2,innernum[i]);
    }   
    while (flag==1)
    {
        for(int i=0;i<s2.mLen;i++){//
            if(s2.mData[i]==-2){//如果到底部
                printf("输入串#");//打印输入串
            }
            else{
                printf(" %d",s2.mData[i]);
            }
        }
        printf("\n");
        for(int i=0;i<s1.mLen;i++){
            if(s1.mData[i]==-2){//如果底部
                printf("符号栈#");//打印符号栈
            }
            else{
                printf(" %d",s1.mData[i]);
            }
        }
        printf("\n");
        if(Peek(s1)<=35&&Peek(s1)>=0){//如果栈顶为终结符
            if(Peek(s1)==Peek(s2)){//如果符号相同则都弹出
                Pop(s1);
                Pop(s2);
            }
            else{
                flag=0;
                printf("error1");
            }
        }
        else if(Peek(s1)==-2){//如果栈顶为#
            if(Peek(s1)==Peek(s2)){//如果都到#了则语法分析成功
                flag=0;
                printf("yes");
            }
            else{//否则报错
                flag=0;
                printf("error2");
            }
        }
        else if(M[Peek(s1)][Peek(s2)]!=-1){//如果M表不为空
            int p=1;
            int top=Peek(s1);
            Pop(s1);
            while(express[M[top][Peek(s2)]][p]!=0){//找到M表的产生式放入
                p++;
            }
            for(int kk=p-1;kk>=1;kk--){
                if(express[M[top][Peek(s2)]][kk]==-1){
                    break;
                }
                Push(s1,express[M[top][Peek(s2)]][kk]);//倒序推入栈中
            }
            p=0;
            printf("所用产生式");
            while (express[M[top][Peek(s2)]][p]!=0)
            {
                printf("%d ",express[M[top][Peek(s2)]][p]);
                p++;
            }
            printf("\n");
            // printf("\n");

        }
        else{
            flag=0;
            printf("error3");
        }
            printf("\n");

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
    for(int i=1;i<=vnnum;i++){
        vnsign[i]=i+127;
    }
    first(130);
    for(int i=1;i<=vnnum;i++)
    first(vnsign[i]);
    for(int i=1;i<=vnnum;i++)
    first(vnsign[i]);
    for(int i=1;i<=vnnum;i++)
    first(vnsign[i]);
    for(int i=1;i<=vnnum;i++)
    first(vnsign[i]);
    // follow();
    follow1(155);
    for(int i=1;i<=vnnum;i++)
    follow1(vnsign[i]);
    for(int i=1;i<=vnnum;i++)
    follow1(vnsign[i]);
    for(int i=1;i<=vnnum;i++)
    follow1(vnsign[i]);
    for(int i=1;i<=vnnum;i++)
    follow1(vnsign[i]);
    for(int i=1;i<=vnnum;i++)
    follow1(vnsign[i]);
    for(int i=1;i<=vnnum;i++)
    follow1(vnsign[i]);
       for(int i=1;i<=vnnum;i++)
    follow1(vnsign[i]);
    for(int i=1;i<=vnnum;i++)
    follow1(vnsign[i]);
    printf("first集合：\n");
    for(int i=128;i<162;i++){
        printf("%d ",i);
    for(int j=0;j<=35;j++){
            if(firstSet[i][j]==1)
            printf("%d ",j);
        }
        printf("\n");
    }
        printf("\n");
    printf("follow集合：\n");
    for(int i=0;i<200;i++)
        for(int j=0;j<200;j++)
            M[i][j]=-1;
    for(int i=128;i<162;i++){
        printf("%d ",i);
        for(int j=0;j<=35;j++){
            if(followSet[i][j]==1)
            printf("%d ",j);
        }
        printf("\n");
    }
    table();
        printf("\n");
    printf("M表：\n");
    
     for(int i=128;i<162;i++){
        for(int j=0;j<=35;j++){
            printf("%d ",M[i][j]);
        }
        printf("\n");
    }
    work();
    return 0;
}
