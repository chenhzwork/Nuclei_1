#include "specific.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include<ctype.h>
#include<time.h>

#define LFT_BRKT '('
#define RGT_BRKT ')'
#define LFT_BRKT_s "("
#define RGT_BRKT_s ")"
#define STRSAME(A,B) (strcmp(A,B) == 0)
#define MAXNUMTOKENS 1024
#define MAXTOKENSIZE 128
#define MAX_STR_LEN 256
#define SINGLE_QUOTE '\''
#define DOUBLE_QUOTE '"'
#define HASHTAG '#'
#define MAXSTR 1024
#define ONE 1
#define ZERO 0
#define TWO 2
#define NAME_SIZE 20
#define MAX_RAND 512
#define LWR_T 't'
#define LWR_M 'm'
#define LWR_P 'p'
#define CAR_s "CAR"
#define CDR_s "CDR"
#define CONS_s "CONS"
#define SET_s "SET"
#define PRINT_s "PRINT"
#define PLUS_s "PLUS"
#define LENGTH_s "LENGTH"
#define LESS_s "LESS"
#define GREATER_s "GREATER"
#define EQUAL_s "EQUAL"
#define IF_s "IF"
#define WHILE_s "WHILE"
#define SUM_s "SUM"
#define ATMS_s "ATMS"
#define PRODUCT_s "PRODUCT"
#define NIL_s "NIL"
#define NIL NULL
#define letter_A 'A'
#define letter_Z 'Z'
#define STR_END '\0'
#define LINE_END '\n'
#define SPACE ' '
#define ERROR(PHRASE) { fprintf(stderr, \
        "Fatal Error %s occurred in %s, line %d\n", PHRASE, \
        __FILE__, __LINE__); \
        exit(EXIT_FAILURE); }

typedef struct var vars;
typedef struct prog Program;

struct var{
    char name[NAME_SIZE];
    lisp* l;
    struct var* next;
    bool isFrom;
};

struct prog{
    char wds[MAXNUMTOKENS][MAXTOKENSIZE];
    int cur_word;
    vars* v;
};

Program* init_Prog(void);
void Prog(Program *p);
void get_line(FILE* fp, Program* p);
void get_str(char tmp_line[MAXSTR], Program* p);
char* get_p_wds(Program* p);
int ini_cur_word(Program* p);
void Instrcts(Program *p);
void Instrct(Program* p);
void Func(Program* p);
void next_cur_word(Program* p);
void Listfunc(Program* p);
void Iofunc(Program* p);
void Set(Program* p);
void Print(Program* p);
void Intfunc(Program* p);
void Boolfunc(Program* p);
void If(Program* p);
void Loop(Program* p);
void Retfunc(Program* p);
void List(Program *p);
bool Var(Program* p);
void Literal(Program* p);
void String(Program* p);
char get_first_c(Program* p);
char get_last_c(Program* p);
char get_next_wds(Program* p);
void push_var(vars** v, char* name, lisp* l);
bool is_name_Exist(char name[NAME_SIZE], vars* v);
bool is_lisp_Exist(lisp* l, vars* v);
void get_uniq_name(Program* p, char* name);
lisp* peek_var(vars* v);
vars* pop_var(vars* v);
void free_var(vars** v);
vars* find_var(char name[NAME_SIZE], Program* p);
bool is_tmp(char name[NAME_SIZE]);
void check_invld_wrd(vars* a, char name[NAME_SIZE]);
void Prog_free(Program* p);

//function parts
void is_Single(char word[MAXTOKENSIZE], int* i, char** tmp);
void is_Double(char word[MAXTOKENSIZE], int* i, char** tmp);
void INTERP_CAR(Program* p);
void INTERP_CDR(Program* p);
lisp* INTERP_CONS_1(Program* p);
lisp* INTERP_CONS_2(Program* p);
void INTERP_LENGTH(Program* p);
void INTERP_SUM(Program* p);
void INTERP_ATMS(Program* p);
void INTERP_PRODUCT(Program* p);
void If_true(Program* p);
void If_false(Program* p);
void Loop_false_con(Program* p);
void Less_1(Program* p, int* index_1);
void Less_2(Program* p, int* index_2);
void Less_result(Program* p, int* index_1, int* index_2);
void Greater_1(Program* p, int* index_1);
void Greater_2(Program* p, int* index_2);
void Greater_result(Program* p, int* index_1, int* index_2);
void Equal_1(Program* p, char str1[MAXSTR]);
void Equal_2(Program* p, char str2[MAXSTR]);
void Equal_result(Program* p, char str1[MAXSTR], char str2[MAXSTR]);
void Plus_1(Program* p, int* index_1);
void Plus_2(Program* p, int* index_2);
void Plus_sum(Program* p, int* index_1, int* index_2);
void Set_tmp(Program* p, char tmp_name[NAME_SIZE]);
void is_LFTBKT(char str[MAXTOKENSIZE], int cursor, int* l_bkt);
void is_DIGIT(char str[MAXTOKENSIZE], int cursor);
void is_RGTBKT(char str[MAXTOKENSIZE], int cursor, int* r_bkt);
void is_SPACE(char str[MAXTOKENSIZE], int cursor);


//test functions
void test_nuclei(void);
void tst_get_wds_get_c(void);
void tst_init_Prog(void);
void tst_p_NIL(void);
void tst_find_var_lisp_name_exist(void);
void tst_isFrom_istmp(void);
void tst_stack(void);
