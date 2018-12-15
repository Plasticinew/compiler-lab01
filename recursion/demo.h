#include <stdio.h>
#include <vector>
#define NRW        2     // number of reserved words
#define TXMAX      500    // length of identifier table
#define MAXNUMLEN  14     // maximum number of digits in numbers
#define NSYM       7     // maximum number of symbols in array ssym and csym
#define MAXIDLEN   10     // length of identifiers

#define MAXADDRESS 32767  // maximum address
#define MAXLEVEL   32     // maximum depth of nesting block
#define CXMAX      500    // size of code array

#define MAXSYM     30     // maximum number of symbols

#define STACKSIZE  1000   // maximum storage



enum symtype
{
    SYM_NULL,
    SYM_INT,
    SYM_VOID,
    SYM_POINTER,
    SYM_LBRACKET,
    SYM_RBRACKET,
    SYM_LROUND,
    SYM_RROUND,
    SYM_COMMA,
    SYM_IDENTIFIER,
    SYM_SEMICOLON,
    SYM_NUMBER,
    SYM_ARRAY,
    SYM_FUNCTION,
    SYM_NAME,
    SYM_RETURNVALUE
};

enum idtype
{
    ID_CONSTANT, ID_VARIABLE, ID_PROCEDURE
};

enum opcode
{
    LIT, OPR, LOD, STO, CAL, INT, JMP, JPC
};

enum oprcode
{
    OPR_RET, OPR_NEG, OPR_ADD, OPR_MIN,
    OPR_MUL, OPR_DIV, OPR_ODD, OPR_EQU,
    OPR_NEQ, OPR_LES, OPR_LEQ, OPR_GTR,
    OPR_GEQ
};


typedef struct
{
    int f; // function code
    int l; // level
    int a; // displacement address
} instruction;

//////////////////////////////////////////////////////////////////////
char* err_msg[] =
        {
/*  0 */    "",
/*  1 */    "Found ':=' when expecting '='.",
/*  2 */    "There must be a number to follow '='.",
/*  3 */    "There must be an '=' to follow the identifier.",
/*  4 */    "There must be an identifier to follow 'const', 'var', or 'procedure'.",
/*  5 */    "Missing ',' or ';'.",
/*  6 */    "Incorrect procedure name.",
/*  7 */    "Statement expected.",
/*  8 */    "Follow the statement is an incorrect symbol.",
/*  9 */    "'.' expected.",
/* 10 */    "';' expected.",
/* 11 */    "Undeclared identifier.",
/* 12 */    "Illegal assignment.",
/* 13 */    "':=' expected.",
/* 14 */    "There must be an identifier to follow the 'call'.",
/* 15 */    "A constant or variable can not be called.",
/* 16 */    "'then' expected.",
/* 17 */    "';' or 'end' expected.",
/* 18 */    "'do' expected.",
/* 19 */    "Incorrect symbol.",
/* 20 */    "Relative operators expected.",
/* 21 */    "Procedure identifier can not be in an expression.",
/* 22 */    "Missing ')'.",
/* 23 */    "The symbol can not be followed by an expression.",
/* 24 */    "The symbol can not be as the beginning of an expression.",
/* 25 */    "The number is too great.",
/* 26 */    "There must be loop to jum out.",
/* 27 */    "There must be an \"if\" for \"else\"",
/* 28 */    "Expression can not be assigned!",
/* 29 */    "",
/* 30 */    "",
/* 31 */    "",
/* 32 */    "There are too many levels."
        };

//////////////////////////////////////////////////////////////////////

struct type{
    int name=SYM_NULL;
    int value=0;
};

struct var{
    char *name= nullptr;
    type* vartype= nullptr;
    std::vector<type*> valuetype;
    std::vector<var*> param;
};
int error;
int printparam(var* param);
/*struct func{
    char *name=NULL;
    var *param[10];
    type *rvalue=NULL;
};*/

std::vector<var*> varlist;
int varnum=0;

char ch;         // last character read
int  sym;        // last symbol read
char id[MAXIDLEN + 1]; // last identifier read
int  num;        // last number read
int  cc;         // character count
int  ll;         // line length
int  kk;
int  err;
int  cx;         // index of current instruction to be generated.
int  level = 0;
int  tx = 0;
int* currenttruelist=NULL;
int* currentfalselist=NULL;
int state=0;
int preread=0;
char preid[MAXIDLEN+1]="";
char line[80];
type *speci;


instruction code[CXMAX];

char* word[NRW + 1] =
        {
                "", /* place holder */
                "int", "void"
        };

int wsym[NRW + 1] =
        {
                SYM_NULL, SYM_INT,SYM_VOID};

int ssym[NSYM + 1] =
        {
                SYM_NULL, SYM_LBRACKET,SYM_RBRACKET,SYM_LROUND,SYM_RROUND,SYM_POINTER,SYM_COMMA, SYM_SEMICOLON
        };

char csym[NSYM + 1] =
        {
                ' ', '[', ']', '(', ')', '*', ',', ';'
        };

#define MAXINS   8
char* mnemonic[MAXINS] =
        {
                "LIT", "OPR", "LOD", "STO", "CAL", "INT", "JMP", "JPC"
        };

typedef struct
{
    char name[MAXIDLEN + 1];
    int  kind;
    int  value;

} comtab;

comtab table[TXMAX];

typedef struct
{
    char  name[MAXIDLEN + 1];
    int   kind;
    short level;
    short address;
} mask;

FILE* infile;
void init_declarator(var* i);
int declaration_specifiers();
// EOF PL0.h
