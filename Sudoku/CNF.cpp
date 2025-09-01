#include <iostream>
#include <algorithm>
#include <string.h>
#include <stdlib.h>
using namespace std;


/*构建用于存储的系统 
CNF{变元数、变元取值、节点数、第一个节点}
|
CNFc{是否成立、第几句子、第一个文字、下一个句子}-CNFl-CNFl;
|
CNFc-CNFl{正负、下一字}-CNFl-CNFl-CNFl;
|
NULL
*/
struct cnfl{
    int pos;
    bool pan;
    int val;
    cnfl *next;
    cnfl *pre;
};
typedef cnfl CNFl;

struct cnfc{
    bool flag;
    int pos;
    CNFl *lit;
    cnfc *next;
    cnfc *pre;
};
typedef cnfc CNFc;

typedef struct{
    int litn;
    CNFc *content;
} CNF;


/*用于构造通过文字索引*/
struct incal{
    incal *pre;
    incal *next;
    CNFl *posi;
};
typedef incal LIT;

struct liter{
    int num;
    liter *next;
    liter *pre;
    LIT *posi;
};
typedef liter LITS;

//构建一个栈，可以一直记录每次改变的指针头和尾
struct changeC{
    CNFc *head;
    CNFc *back;
    changeC *pre;
};
typedef changeC CGC;

struct changeL{
    CNFl *head;
    CNFl *back;
    changeL *pre;
};
typedef changeL CGL;


void addlit(LITS *lits,CNFl *cnfl);
void addLiterature(CNFl * flit,CNFl * plit,LITS *lits);
LITS* findlitnum(LITS *head, int n);
LITS* litsinit(int ln);
CGC* cgcinit();
CGL* cglinit();

CNF* cnfinit(int ln){
    CNF *cnf;
    cnf = (CNF *)malloc(sizeof(CNF));
    
    //根据文件设置变元的数量
    cnf->litn=ln;
    cnf->content=NULL;

    //初始化第一个句子，并赋予编号,封掉尾部,封掉头部，建立双向链表
    CNFc *firstcal;
    firstcal = (CNFc *)malloc(sizeof(CNFc));
    firstcal->next=NULL;
    firstcal->pre=NULL;
    firstcal->pos=0;
    cnf->content = firstcal;

    return cnf;
}



void addClause(CNF* cnf,LITS *lits){
    //找到尾部

    CNFc *last=cnf->content->next,*pre=cnf->content;
    while(last!=NULL){
        pre=last;
        last = last->next;
    }
        
    //添加句头：新序号
    CNFc *newclause = (CNFc *)malloc(sizeof(CNFc));
    newclause->pos=pre->pos+1;
    newclause->next=NULL;
    newclause->pre=pre;
    //cout<<"success in creating a new cal\n";
    //链接生成的句子到已有尾端
    pre->next=newclause;
    
    //初始化第一个文字，并赋予编号,在最后用NULL
    CNFl *firstlit;
    firstlit = (CNFl *)malloc(sizeof(CNFl));
    firstlit->next=NULL;
    firstlit->pre=NULL;
    firstlit->pos=newclause->pos;
    newclause->lit = firstlit;

    //开始递归生成句子
    addLiterature(newclause->lit,NULL, lits);
    

    return;
}


void addLiterature(CNFl * flit,CNFl * plit,LITS *lits){
    CNFl *last = flit;
    //新建一个节点
    CNFl *nextlit;
    nextlit = (CNFl *)malloc(sizeof(CNFl));
    nextlit->next=NULL;
    nextlit->pre=plit;
    last->next=nextlit;
    nextlit->pos=flit->pos;
    
    //接受输入并处理负数，并存入本节点
    int lit;
    scanf("%d",&lit);
    if(lit<0){
        lit=-lit;
        last->pan=0;
    }
    last->val=lit;

    //加入变元为主干的链表
    LITS *litstoconnect;
    litstoconnect = findlitnum(lits,lit);
    addlit(litstoconnect,flit);
    
    //cout<<"success in add a lit\n";
    // cout<<"success in find "<<litstoconnect->num<<"\n";
    //cout<<"success in add to lit\n";

    //接受看是否结束输入,空格代表还有，新建一个节点嵌套,回车代表没了，直接跳出
    char conti;
    scanf("%c",&conti);
    if(conti=='\n'){
        return;
    }
    else{
        addLiterature(last->next,last,lits);
    }

    return;
}





//初始化文字为主干的链表
LITS* litsinit(int ln){
    LITS *lits;
    lits = (LITS *)malloc(sizeof(LITS));
    
    //初始化第一个文字，并赋予编号,封掉尾部,封掉头部，建立双向链表
    lits->num=1;
    lits->posi=NULL;
    lits->pre=NULL;
    lits->next=NULL;
    LITS *next=lits,*pre=NULL;

    //初始化所有的变元
    for(int i=2;i<=ln;i++){
        pre=next;
        next->next=(LITS *)malloc(sizeof(LITS));
        next=next->next;
        next->num=i;
        next->pre=pre;
        next->posi=NULL;
        next->next=NULL;

        
        cout<<"init\n";
    }


    return lits;
}

//查询到特定的枝干
LITS* findlitnum(LITS *head, int n){
    LITS *next=head;
    while(next!=NULL){
        cout<<"finding"<<next->num<<"\n";
        if(next->num==n){
            return next;
        }
        else{
            next=next->next;
        }
    }
    return NULL;
}

void addlit(LITS *lits,CNFl *cnfl){
    //如果没生成过，先生成一个空间
    cout<<lits->num<<"\n";
    if(lits->posi==0){
        cout<<"creating a new posi\n";
        lits->posi = (LIT *)malloc(sizeof(LIT));
        lits->posi->next=NULL;
        lits->posi->posi=cnfl;
        lits->posi->pre=NULL;
    }

    else{//否则进行查尾
        LIT *next = lits->posi,*pre=NULL;
        while(next!=NULL){
            pre=next;
            next=next->next;
        }
        
        cout<<"finnish searching bottom\n";
        pre->next = (LIT *)malloc(sizeof(LIT));
        pre->next->pre = pre;
        pre->next->posi=cnfl;
        pre->next->next=NULL;
    }
    return;
}

//两种栈的维护
CGC* cgcinit(void){
    CGC* top = (CGC *)malloc(sizeof(CGC));
    top->pre=NULL;
    top->head=NULL;
    top->back=NULL;
}

void addcgc(CGC* top,CNFc *head,CNFc *back){
    CGC *newcgc = (CGC *)malloc(sizeof(CGC));
    newcgc->head=head;
    newcgc->back=back;
    newcgc->pre=top->pre;
    top->pre=newcgc;
}

void pushcgc(CGC* top){
    CGC *tool=top->pre;
    tool->head->next=tool->back;
    top->pre=tool->pre;
    free(tool);
}


CGL* cglinit(void){
    CGL* top = (CGL *)malloc(sizeof(CGL));
    top->pre=NULL;
    top->head=NULL;
    top->back=NULL;
}

void addcgl(CGL* top,CNFl *head,CNFl *back){
    CGL *newcgl = (CGL *)malloc(sizeof(CGL));
    newcgl->head=head;
    newcgl->back=back;
    newcgl->pre=top->pre;
    top->pre=newcgl;
}

void pushcgl(CGL* top){
    CGL *tool=top->pre;
    tool->head->next=tool->back;
    top->pre=tool->pre;
    free(tool);
}

void delateclause(CNF *cnf,CGC *cgc,int n){
    CNFc *next=cnf->content;
    while(next!=NULL&&next->pos<n-1){
        next=next->next;
    }   
    addcgc(cgc,next,next->next);
    addcgc(cgc,next->next,next->next->next);
    next->next=next->next->next;
}

