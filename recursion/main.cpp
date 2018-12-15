#include "demo.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
void getch(void)
{
    if (cc == ll)
    {
        if (feof(infile))
        {
            printf("\nPROGRAM INCOMPLETE\n");
            exit(1);
        }
        ll = cc = 0;
        printf("%5d  ", cx);
        while ( (!feof(infile)) // added & modified by alex 01-02-09
                && ((ch = getc(infile)) != '\n'))
        {
            printf("%c", ch);
            line[++ll] = ch;
        } // while
        printf("\n");
        line[++ll] = ' ';
    }
    ch = line[++cc];
} // getch

//////////////////////////////////////////////////////////////////////
// gets a symbol from input stream.
void getsym(void)
{
    int i, k;
    char a[MAXIDLEN + 1];

    while (ch == ' '||ch == '\t')
        getch();

    if (isalpha(ch))
    { // symbol is a reserved word or an identifier.
        k = 0;
        do
        {
            if (k < MAXIDLEN)
                a[k++] = ch;
            getch();
        }
        while (isalpha(ch) || isdigit(ch));
        a[k] = 0;
        strcpy(id, a);
        word[0] = id;
        i = NRW;
        while (strcmp(id, word[i--]));
        if (++i)
            sym = wsym[i]; // symbol is a reserved word
        else
            sym = SYM_IDENTIFIER;   // symbol is an identifier
    }
    else if (isdigit(ch))
    { // symbol is a number.
        k = num = 0;
        sym = SYM_NUMBER;
        do
        {
            num = num * 10 + ch - '0';
            k++;
            getch();
        }
        while (isdigit(ch));
        //if (k > MAXNUMLEN)
            //error(25);     // The number is too great.
    }
    else
    { // other tokens
        i = NSYM;
        csym[0] = ch;
        while (csym[i--] != ch);
        if (++i)
        {
            sym = ssym[i];
            getch();
        }
        else
        {
            //printf("Fatal Error: Unknown character.\n");
            exit(1);
        }
    }
} // getsym

type *control;

 void adddnode(var* varl,int t,int value){
    type* newone=(type*)malloc(sizeof(type));
    newone->name=t;
    newone->value=value;
    varl->valuetype.push_back(newone);
}

void adddnode_var(var* varl,int t,int value){
    type* newone=(type*)malloc(sizeof(type));
    newone->name=t;
    newone->value=value;
    varl->vartype=newone;
}

type *p;
char trueid[11];



int printtype(var* varl,int p){
    var* see=varl;
    type *m=varl->valuetype[p];
    if (m->name == SYM_POINTER) {
        printf("pointer( ");
        if (p+1<varl->valuetype.size()) {
            printtype(varl,p+1);
        }
        printf(")");
    } else if (m->name == SYM_VOID) {
        printf("void");
    } else if (m->name == SYM_INT) {
        printf("int");
    } else if (m->name == SYM_ARRAY) {
        printf("array(%d,", m->value);
        if (p+1<varl->valuetype.size()) {
            if(varl->valuetype[p+1]->name==SYM_FUNCTION){
                error=2;
            }
            printtype(varl,p+1);
        }
        printf(")");
    } else if (m->name == NULL) {
        if (p+1<varl->valuetype.size()) {
            printtype(varl,p+1);
        }
    } else if (m->name == SYM_FUNCTION) {
        printf("function(");
        int j;
        if(m->value==0){
            printf("void");
        }
        else {
            for (j = 0; j < m->value - 1; j++) {
                printparam(varl->param[j]);
                printf("x");
            }
            printparam(varl->param[j]);
        }
        printf("=>");
        if (p+1<varl->valuetype.size()) {
            if(varl->valuetype[p+1]->name==SYM_ARRAY){
                error=1;
            }
            printtype(varl,p+1);
        }
        printf(")");
    }
    return error;
}

int printparam(var* param){
    if(param->vartype!= nullptr) {
        if (param->vartype->name == SYM_FUNCTION) {
            printf("function(");
            int j;
            type *m = param->vartype;
            if (m->value == 0) {
                printf("void");
            } else {
                for (j = 0; j < m->value - 1; j++) {
                    printparam(param->param[j]);
                    printf("x");
                }
                printparam(param->param[j]);
            }
            printf("=>");
            if(param->valuetype[0]->name==SYM_ARRAY){
                error=1;
            }
            printtype(param, 0);
            printf(")");
        } else if (param->vartype->name == SYM_POINTER) {
            printf("pointer(");
            printtype(param, 0);
            printf(")");
        } else if (param->vartype->name == SYM_ARRAY) {
            printf("array(%d,", param->vartype->value);
            if(param->valuetype[0]->name==SYM_FUNCTION){
               error=2;
            }
            printtype(param, 0);
            printf(")");
        } else {
            printtype(param, 0);
        }
    }
    else {
        printtype(param, 0);
    }
    return error;
}

int print(var *varl){
    //std::vector<var*> see=varlist;

        printf("%s is type of ",varl->name);
        if(varl->vartype!= nullptr) {
            if (varl->vartype->name == SYM_FUNCTION) {
                printf("function(");
                int j=0;
                type *m = varl->vartype;
                if (m->value == 0) {
                    printf("void");
                } else {
                    printparam(varl->param[j]);
                    for (j = 1; j < m->value ; j++) {
                        printf(" x ");
                        printparam(varl->param[j]);
                    }

                }
                printf("=>");
                if(varl->valuetype[0]->name==SYM_ARRAY){
                    error=1;
                }
                printtype(varl, 0);
                printf(")\n");
            } else if (varl->vartype->name == SYM_POINTER) {
                printf("pointer(");
                printtype(varl, 0);
                printf(")\n");

            } else if (varl->vartype->name == SYM_ARRAY) {
                printf("array(%d,", varl->vartype->value);
                if(varl->valuetype[0]->name==SYM_FUNCTION){
                    error=2;
                }
                printtype(varl, 0);
                printf(")\n");
            } else {
                printtype(varl, 0);
                printf("\n");
            }
        }
        else {
            printtype(varl, 0);
            printf("\n");
        }
    return error;
}


void parameter_declaration(var* varl){
    adddnode(varl,declaration_specifiers(),0);
    init_declarator(varl);
}

int parameter_type_list(var* varl){
    int count=0;
    //var* new1=(var*)malloc(sizeof(var));
    var* new1=new var;
    count++;
    varl->param.push_back(new1);
    var* see=new1;
    parameter_declaration(new1);
    printf("param:");
    print(new1);
    while(sym==SYM_COMMA){
        getsym();
        //var* new2=(var*)malloc(sizeof(var));
        var* new2=new var;
        count++;
        varl->param.push_back(new2);
        parameter_declaration(new2);
        printf("param:");
        print(new2);
    }
    return count;
}

void direct_declarator(var* varl){
    if(sym==SYM_IDENTIFIER){
        char* newid=(char*)malloc(sizeof(char)*12);
        strcpy(newid,id);
        varl->name=newid;
        getsym();
    }
    else if(sym==SYM_LROUND){
        getsym();
        type *point=control;
        init_declarator(varl);
        control=point;
        if(sym==SYM_RROUND)
            getsym();
    }
    while(sym==SYM_LROUND||sym==SYM_LBRACKET) {
        if (sym == SYM_LROUND) {
            getsym();
            int count;
            if(sym==SYM_RROUND){
                count=0;
                //getsym();
            }
            else {
                count=parameter_type_list(varl);
            }
            if(varl->vartype!= nullptr){
                adddnode(varl,SYM_FUNCTION,count);
            }
            else{
                adddnode_var(varl,SYM_FUNCTION,count);
            }
            if (sym == SYM_RROUND) {
                getsym();
            }
        } else {
            getsym();
            if (varl->vartype) {
                adddnode(varl, SYM_ARRAY, num);
            } else {
                adddnode_var(varl, SYM_ARRAY, num);
            }
            getsym();
            if (sym == SYM_RBRACKET)
                getsym();
        }
    }

}

int pointer(){
    int num=0;
    num++;
    //adddnode(varlist[i]->valuetype,SYM_POINTER,0,NULL);
    getsym();
    while(sym==SYM_POINTER) {
        num++;
        getsym();
    }
    return num;
}


void init_declarator(var* varl){
    int point_count=0;
    if(sym==SYM_POINTER){
        point_count=pointer();
    }
    direct_declarator(varl);
    while(point_count--){
        if(varl->vartype!= nullptr){
            adddnode(varl,SYM_POINTER,0);
        }
        else{
            adddnode_var(varl,SYM_POINTER,0);
        }
    }
}


void init_declaratoe_list(int i){
    varlist.clear();
    varnum=0;
    int num=0;
    varnum++;
    var *new1=new var;
    varlist.push_back(new1);
    init_declarator(varlist[0]);
    adddnode(varlist[0],i,0);
    while(sym==SYM_COMMA){
        var *new2=new var;
        varlist.push_back(new2);
        varnum++;
        getsym();
        init_declarator(varlist[varnum-1]);
        adddnode(varlist[varnum-1],i,0);
    }
}

int declaration_specifiers(){
    if(sym==SYM_VOID){
        getsym();
        return SYM_VOID;
    }
    else if(sym==SYM_INT){
        getsym();
        return SYM_INT;
    }
    return 0;
}

void declaration(){
    error=0;
    init_declaratoe_list(declaration_specifiers());
    for(int i=0;i<varlist.size();i++)
        print(varlist[i]);


    //printf("\n");
    printf("type chrcking...\n");
    if(error==1){
        printf("Error 1:Array cannot be the return value of function\n");
    }
    else if(error==2){
        printf("Error 2:Function cannot be a member of array\n");
    }
    else{
        printf("OK...\n");
        for(int i=0;i<varlist.size();i++){
            if(varlist[i]->vartype&&varlist[i]->vartype->name==SYM_FUNCTION){
                printf("%s is a function, para-type is:",varlist[i]->name);
                if(varlist[i]->vartype->value==0){
                   printf("void ");
                }
                else {
                    printparam(varlist[i]->param[0]);
                    for (int j = 1; j < varlist[i]->vartype->value; j++) {
                        printf(" x ");
                        printparam(varlist[i]->param[j]);
                    }
                    printf(", return-type is:");
                    printtype(varlist[i],0);
                }
                printf("\n");
            }
            else {
                int size=1;
                if(varlist[i]->vartype&&varlist[i]->vartype->name==SYM_ARRAY){
                    int j=0;
                    size*=varlist[i]->vartype->value;
                    while(varlist[i]->valuetype[j]&&varlist[i]->valuetype[j]->name==SYM_ARRAY){
                        size*=varlist[i]->valuetype[j]->value;
                        j++;
                    }
                }
                printf("%s's size is: %d, value type is:",varlist[i]->name,size);
                printtype(varlist[i],0);
                printf("\n");
            }

        }
    }
    printf("\n");
    if(sym==SYM_SEMICOLON){
        getsym();
    }
    else{
        printf("need semicolon\n");
    }
}

void translationunit(){
    declaration();

    while(sym==SYM_VOID||sym==SYM_INT)
        declaration();
}

int main() {
    FILE* hbin;
    char s[80];
    printf("Please input source file name: "); // get file name to be compiled
    scanf("%s", s);
    if ((infile = fopen(s, "r")) == NULL)
    {
        printf("File %s can't be opened.\n", s);
        exit(1);
    }

    err = cc = cx = ll = 0; // initialize global variables
    ch = ' ';
    kk = MAXIDLEN;
    getsym();
    translationunit();
    return 0;
}