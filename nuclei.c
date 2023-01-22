#include "nuclei.h"

int main(int argc, char* argv[]){
    if(argc == ZERO){
        exit(EXIT_FAILURE);
    }
    FILE *fp = fopen(argv[ONE], "r+");
    if (!fp){
        printf("Error opening file");
        exit(EXIT_FAILURE);
    }
    srand((unsigned)time(NULL));
    test_nuclei();
    Program* p = init_Prog();
    get_line(fp, p);
    Prog(p);
    Prog_free(p);
#ifndef INTERP
    fprintf(stderr, "Parsed OK.\n");
#endif
    fclose(fp);
    return 0;
}

//read file
void get_line(FILE* fp, Program* p){
    char tmp_line[MAXSTR];
    while(fgets(tmp_line, MAXSTR, fp)){
        get_str(tmp_line, p);
    }
}

void get_str(char tmp_line[MAXSTR], Program* p){
    char* tmp = tmp_line;
    if(*tmp == HASHTAG){
        return;
    }
    while(*tmp != LINE_END && *tmp != STR_END){
        int i = ZERO;
        char word[MAXTOKENSIZE] = {ZERO};
        while(*tmp == SPACE){
            tmp++;
        }
        if(*tmp == SINGLE_QUOTE){
            word[i++] = *tmp;
            tmp++;
            while(*tmp != SINGLE_QUOTE){
                word[i++] = *tmp;
                tmp++;
            }
            word[i++] = SINGLE_QUOTE;
            tmp++;
        }
        else if(*tmp == DOUBLE_QUOTE){
            word[i++] = *tmp;
            tmp++;
            while(*tmp != DOUBLE_QUOTE){
                word[i++] = *tmp;
                tmp++;
            }
            word[i++] = DOUBLE_QUOTE;
            tmp++;
        }
        else if(*tmp == LFT_BRKT){
            word[i++] = *tmp;
            tmp++;
        }
        else if(*tmp == RGT_BRKT){
            word[i++] = *tmp;
            tmp++;
        }else{
            while(*tmp != SPACE && *tmp != LINE_END && *tmp != LFT_BRKT
                  && *tmp != RGT_BRKT && *tmp != STR_END){
                word[i++] = *tmp;
                tmp++;
            }
        }
        word[i] = STR_END;
        strcpy(p->wds[p->cur_word++], word);
    }
}

//initial global variable p
Program* init_Prog(void){
    Program* p = (Program*)calloc(ONE, sizeof(Program));
    p->v = NIL;
    p->cur_word = ZERO;
    return p;
}

//get current word
char* get_p_wds(Program* p){
    if(p == NIL){
        return 0;
    }
    return p->wds[p->cur_word];
}

//initial current word to 0 and return it
int ini_cur_word(Program* p){
    if(p == NIL){
        return 0;
    }
    p->cur_word = ZERO;
    return p->cur_word;
}

//make next instruct word as current
void next_cur_word(Program* p){
    p->cur_word = p->cur_word + ONE;
}

//start the program
void Prog(Program *p){
    if(!STRSAME(LFT_BRKT_s, p->wds[ini_cur_word(p)])){
        ERROR("Need left bracket to begin with.")
    }
    next_cur_word(p);
    Instrcts(p);
}

void Instrcts(Program *p){
    if(STRSAME(get_p_wds(p), RGT_BRKT_s)){
        if(get_next_wds(p) == STR_END){
            return;
        }
        next_cur_word(p);
        return;
    }
    Instrct(p);
    Instrcts(p);
}

//get next words from the string
char get_next_wds(Program* p){
    int cur = p->cur_word;
    char* next_val = p->wds[cur + ONE];
    return *next_val;
}

//select category
void Instrct(Program* p){
    if(STRSAME(LFT_BRKT_s, get_p_wds(p))) {
        next_cur_word(p);
    }else{
        ERROR("Expecting a RIGHT bracket to end instruct or a LEFT bracket to begin a new one.")
    }
    Func(p);
    if(STRSAME(RGT_BRKT_s, get_p_wds(p))) {
        next_cur_word(p);
        return;
    }else{
        ERROR("Expecting a RIGHT bracket.")
    }
}

//categorise the instruction
void Func(Program* p){
    if(STRSAME(get_p_wds(p), CAR_s) || STRSAME(get_p_wds(p), CDR_s)
       || STRSAME(get_p_wds(p), CONS_s) || STRSAME(get_p_wds(p), PLUS_s) ||
       STRSAME(get_p_wds(p), LENGTH_s) || STRSAME(get_p_wds(p), LESS_s) ||
       STRSAME(get_p_wds(p), GREATER_s) || STRSAME(get_p_wds(p), EQUAL_s) ||
       STRSAME(get_p_wds(p), PRODUCT_s) || STRSAME(get_p_wds(p), ATMS_s) ||
       STRSAME(get_p_wds(p), SUM_s)){
        Retfunc(p);
    }else if(STRSAME(get_p_wds(p), SET_s) || STRSAME(get_p_wds(p), PRINT_s)){
        Iofunc(p);
    }else if(STRSAME(get_p_wds(p), IF_s)){
        If(p);
    }else if(STRSAME(get_p_wds(p), WHILE_s)){
        Loop(p);
    }
    else{
        ERROR("Expecting correct instruct.")
    }
}

void Retfunc(Program* p){
    if(STRSAME(get_p_wds(p), CAR_s) || STRSAME(get_p_wds(p), CDR_s)
       || STRSAME(get_p_wds(p), CONS_s)){
        Listfunc(p);
    }
    if(STRSAME(get_p_wds(p), PLUS_s) || STRSAME(get_p_wds(p), LENGTH_s) ||
        STRSAME(get_p_wds(p), PRODUCT_s) || STRSAME(get_p_wds(p), ATMS_s) ||
        STRSAME(get_p_wds(p), SUM_s)){
        Intfunc(p);
    }
    if(STRSAME(get_p_wds(p), LESS_s) || STRSAME(get_p_wds(p), GREATER_s)
        || STRSAME(get_p_wds(p), EQUAL_s)){
        Boolfunc(p);
    }
}

void If(Program* p){
    next_cur_word(p);
    if(STRSAME(get_p_wds(p), LFT_BRKT_s)){
        next_cur_word(p);
        Boolfunc(p);
    }else{
        ERROR("Expecting a LEFT bracket.")
    }
    if(STRSAME(get_p_wds(p), RGT_BRKT_s)){
        next_cur_word(p);
    }else{
        ERROR("Expecting a RIGHT bracket.")
    }
    If_true(p);
    If_false(p);
}

void If_true(Program* p){
    if(STRSAME(get_p_wds(p), LFT_BRKT_s)){
        next_cur_word(p);
#ifdef INTERP
        if(lisp_getval(peek_var(p->v)) == ONE){
#endif
        Instrcts(p);
#ifdef INTERP
        }else{
            int l_bkt = ONE;
            int r_bkt = ZERO;
            while(l_bkt != r_bkt){
                if(STRSAME(get_p_wds(p), LFT_BRKT_s)){
                    l_bkt++;
                }
                if(STRSAME(get_p_wds(p), RGT_BRKT_s)){
                    r_bkt++;
                }
                next_cur_word(p);
            }
        }
#endif
    }else{
        ERROR("Expecting a LEFT bracket.")
    }
}

void If_false(Program* p){
    if(STRSAME(get_p_wds(p), LFT_BRKT_s)){
        next_cur_word(p);
#ifdef INTERP
        if(lisp_getval(p->v->l) == ZERO){
#endif
        Instrcts(p);
#ifdef INTERP
        }else{
            int l_bkt = ONE;
            int r_bkt = ZERO;
            while(l_bkt != r_bkt){
                if(STRSAME(get_p_wds(p), LFT_BRKT_s)){
                    l_bkt++;
                }
                if(STRSAME(get_p_wds(p), RGT_BRKT_s)){
                    r_bkt++;
                }
                next_cur_word(p);
            }
        }
#endif
    }else{
        ERROR("Expecting a LEFT bracket.")
    }
}

void Loop(Program* p){
#ifdef INTERP
    int loop_break = ONE;
#endif
    next_cur_word(p);
    if(STRSAME(get_p_wds(p), LFT_BRKT_s)) {
        next_cur_word(p);
    }else{
        ERROR("Expecting a LEFT bracket.")
    }
#ifdef INTERP
    int tmp_cur_wds = p->cur_word;
    while (loop_break == ONE) {
#endif
    Boolfunc(p);
#ifdef INTERP
    loop_break = lisp_getval(p->v->l);
#endif
    if (STRSAME(get_p_wds(p), RGT_BRKT_s)) {
        next_cur_word(p);
    }else{
        ERROR("Expecting a RIGHT bracket.")
    }
    if (STRSAME(get_p_wds(p), LFT_BRKT_s)) {
#ifdef INTERP
        if (loop_break == ONE) {                //True condition
#endif
        next_cur_word(p);
        Instrcts(p);
#ifdef INTERP
        p->cur_word = tmp_cur_wds;
        } else if (loop_break == ZERO) {        //False condition
            Loop_false_con(p);
#endif
    } else {
        ERROR("Expecting a LEFT bracket.")
    }
#ifdef INTERP
    }}
#endif
}

#ifdef INTERP
void Loop_false_con(Program* p){
    int l_bkt_num = ZERO;
    int r_bkt_num = ZERO;
    if (STRSAME(get_p_wds(p), LFT_BRKT_s)) {
        l_bkt_num++;
    }
    if (STRSAME(get_p_wds(p), RGT_BRKT_s)) {
        r_bkt_num++;
    }
    while (r_bkt_num < l_bkt_num) {
        if (STRSAME(get_p_wds(p), LFT_BRKT_s)) {
            l_bkt_num++;
        }
        if (STRSAME(get_p_wds(p), RGT_BRKT_s)) {
            r_bkt_num++;
        }
        if (r_bkt_num == l_bkt_num) {
            return;
        }
        next_cur_word(p);
    }
}
#endif

void Boolfunc(Program* p){
#ifdef INTERP
    int index_1 = ZERO;
    int index_2 = ZERO;
#endif
    if(STRSAME(get_p_wds(p), LESS_s)){
        next_cur_word(p);
        List(p);
#ifdef INTERP
        Less_1(p, &index_1);
#endif
        List(p);
#ifdef INTERP
        Less_2(p, &index_2);
        Less_result(p, &index_1, &index_2);
#endif
    }
    else if(STRSAME(get_p_wds(p), GREATER_s)){
        next_cur_word(p);
        List(p);
#ifdef INTERP
        Greater_1(p, &index_1);
#endif
        List(p);
#ifdef INTERP
        Greater_2(p, &index_2);
        Greater_result(p, &index_1, &index_2);
#endif
    }
    else if(STRSAME(get_p_wds(p), EQUAL_s)){
            next_cur_word(p);
            List(p);
#ifdef INTERP
        char str1[MAXSTR];
        char str2[MAXSTR];
        Equal_1(p, str1);
#endif
        List(p);
#ifdef INTERP
        Equal_2(p, str2);
        Equal_result(p, str1, str2);
#endif
    }
}

#ifdef INTERP
void Less_1(Program* p, int* index_1){
    if(is_tmp(p->v->name)){
        *index_1 = lisp_getval(peek_var(p->v));
    }else {
        char tmp_name[NAME_SIZE];
        strcpy(tmp_name, p->v->name);
        p->v = pop_var(p->v);
        vars *a = find_var(tmp_name, p);
        check_invld_wrd(a, tmp_name);
        *index_1 = lisp_getval(a->l);
    }
}

void Less_2(Program* p, int* index_2){
    if(is_tmp(p->v->name)){
        *index_2 = lisp_getval(peek_var(p->v));
    }else {
        char tmp_name1[NAME_SIZE];
        strcpy(tmp_name1, p->v->name);
        p->v = pop_var(p->v);
        vars *b = find_var(tmp_name1, p);
        check_invld_wrd(b, tmp_name1);
        *index_2 = lisp_getval(b->l);
    }
}

void Less_result(Program* p, int* index_1, int* index_2){
    bool result = *index_1 < *index_2 ? true : false;
    char rdm_name[NAME_SIZE];
    get_uniq_name(p, rdm_name);
    push_var(&(p->v), rdm_name, lisp_atom(result));
}

void Greater_1(Program* p, int* index_1){
    if(is_tmp(p->v->name)){
        *index_1 = lisp_getval(peek_var(p->v));
    } else {
        char tmp_name[NAME_SIZE];
        strcpy(tmp_name, p->v->name);
        p->v = pop_var(p->v);
        vars *a = find_var(tmp_name, p);
        check_invld_wrd(a, tmp_name);
        *index_1 = lisp_getval(a->l);
    }
}

void Greater_2(Program* p, int* index_2){
    if(is_tmp(p->v->name)){
        *index_2 = lisp_getval(peek_var(p->v));
    }else {
        char tmp_name1[NAME_SIZE];
        strcpy(tmp_name1, p->v->name);
        p->v = pop_var(p->v);
        vars *b = find_var(tmp_name1, p);
        check_invld_wrd(b, tmp_name1);
        *index_2 = lisp_getval(b->l);
    }
}

void Greater_result(Program* p, int* index_1, int* index_2){
    bool result = *index_1 > *index_2 ? true : false;
    char rdm_name[NAME_SIZE];
    get_uniq_name(p, rdm_name);
    push_var(&(p->v), rdm_name, lisp_atom(result));
}

void Equal_1(Program* p, char str1[MAXSTR]){
    if(is_tmp(p->v->name)){
        lisp_tostring(peek_var(p->v), str1);
    }else{
        char tmp_name[NAME_SIZE];
        strcpy(tmp_name, p->v->name);
        p->v = pop_var(p->v);
        vars *a = find_var(tmp_name, p);
        check_invld_wrd(a, tmp_name);
        lisp_tostring(a->l, str1);
    }
}

void Equal_2(Program* p, char str2[MAXSTR]){
    if(is_tmp(p->v->name)){
        lisp_tostring(peek_var(p->v), str2);
    }else {
        char tmp_name1[NAME_SIZE];
        strcpy(tmp_name1, p->v->name);
        p->v = pop_var(p->v);
        vars *b = find_var(tmp_name1, p);
        check_invld_wrd(b, tmp_name1);
        lisp_tostring(b->l, str2);
    }
}

void Equal_result(Program* p, char str1[MAXSTR], char str2[MAXSTR]){
    bool result = strcmp(str1, str2);
    if(result == ZERO){
        result = ONE;
    }else{
        result = ZERO;
    }
    lisp* l = lisp_atom(result);
    char rdm_name[NAME_SIZE];
    get_uniq_name(p, rdm_name);
    push_var(&(p->v), rdm_name, l);
}

void Plus_1(Program* p, int* index_1){
    if (is_tmp(p->v->name)) {
            *index_1 = lisp_getval(peek_var(p->v));
        } else {
            char tmp_name[NAME_SIZE];
            strcpy(tmp_name, p->v->name);
            p->v = pop_var(p->v);
            vars *a = find_var(tmp_name, p);
            check_invld_wrd(a, tmp_name);
            *index_1 = lisp_getval(a->l);
        }
}

void Plus_2(Program* p, int* index_2){
    if (is_tmp(p->v->name)) {
            *index_2 = lisp_getval(peek_var(p->v));
        } else {
            char tmp_name1[NAME_SIZE];
            strcpy(tmp_name1, p->v->name);
            p->v = pop_var(p->v);
            vars *b = find_var(tmp_name1, p);
            check_invld_wrd(b, tmp_name1);
            *index_2 = lisp_getval(b->l);
        }
}

void Plus_sum(Program* p, int* index_1, int* index_2){
    int sum = *index_1 + *index_2;
        lisp *l = lisp_atom(sum);
        char rdm_name[NAME_SIZE];
        get_uniq_name(p, rdm_name);
        push_var(&(p->v), rdm_name, l);
}
#endif

void Intfunc(Program* p) {
    if (STRSAME(get_p_wds(p), PLUS_s)) {
        next_cur_word(p);
        List(p);
#ifdef INTERP
        int index_1 = ZERO;
        int index_2 = ZERO;
        Plus_1(p, &index_1);
#endif
        List(p);
#ifdef INTERP
        Plus_2(p, &index_2);
        Plus_sum(p, &index_1, &index_2);
#endif
    } else if (STRSAME(get_p_wds(p), LENGTH_s)) {
        next_cur_word(p);
        List(p);
#ifdef INTERP
        INTERP_LENGTH(p);
#endif
    } else if (STRSAME(get_p_wds(p), PRODUCT_s)) {
        next_cur_word(p);
        List(p);
#ifdef INTERP
       INTERP_PRODUCT(p);
#endif
    } else if (STRSAME(get_p_wds(p), ATMS_s)) {
        next_cur_word(p);
        List(p);
#ifdef INTERP
        INTERP_ATMS(p);
#endif
    } else if (STRSAME(get_p_wds(p), SUM_s)) {
        next_cur_word(p);
        List(p);
#ifdef INTERP
        INTERP_SUM(p);
#endif
    }
}

#ifdef INTERP
void INTERP_LENGTH(Program* p){
    int index = ZERO;
    if (is_tmp(p->v->name)) {
        index = lisp_length(p->v->l);
    } else {
        char tmp_name[NAME_SIZE];
        strcpy(tmp_name, p->v->name);
        p->v = pop_var(p->v);
        vars *a = find_var(tmp_name, p);
        check_invld_wrd(a, tmp_name);
        index = lisp_length(a->l);
    }
    lisp *l = lisp_atom(index);
    char rdm_name[NAME_SIZE];
    get_uniq_name(p, rdm_name);
    push_var(&(p->v), rdm_name, l);
}

void INTERP_PRODUCT(Program* p){
    lisp *l = NIL;
    if (is_tmp(p->v->name)) {
        l = peek_var(p->v);
    } else {
        char tmp_name[NAME_SIZE];
        strcpy(tmp_name, p->v->name);
        p->v = pop_var(p->v);
        vars *a = find_var(tmp_name, p);
        check_invld_wrd(a, tmp_name);
        l = a->l;
    }
    atomtype acc = ONE;
    lisp_reduce(times, l, &acc);
    lisp *lr = lisp_atom(acc);
    char rdm_name[NAME_SIZE];
    get_uniq_name(p, rdm_name);
    push_var(&(p->v), rdm_name, lr);
}

void INTERP_ATMS(Program* p){
    lisp *l = NIL;
    if (is_tmp(p->v->name)) {
        l = peek_var(p->v);
    } else {
        char tmp_name[NAME_SIZE];
        strcpy(tmp_name, p->v->name);
        p->v = pop_var(p->v);
        vars *a = find_var(tmp_name, p);
        check_invld_wrd(a, tmp_name);
        l = a->l;
    }
    atomtype acc = ZERO;
    lisp_reduce(atms, l, &acc);
    lisp *lr = lisp_atom(acc);
    char rdm_name[NAME_SIZE];
    get_uniq_name(p, rdm_name);
    push_var(&(p->v), rdm_name, lr);
}

void INTERP_SUM(Program* p){
    lisp *l = NIL;
    if (is_tmp(p->v->name)) {
        l = peek_var(p->v);
    } else {
        char tmp_name[NAME_SIZE];
        strcpy(tmp_name, p->v->name);
        p->v = pop_var(p->v);
        vars *a = find_var(tmp_name, p);
        check_invld_wrd(a, tmp_name);
        l = a->l;
    }
    atomtype acc = ZERO;
    lisp_reduce(sum, l, &acc);
    lisp *lr = lisp_atom(acc);
    char rdm_name[NAME_SIZE];
    get_uniq_name(p, rdm_name);
    push_var(&(p->v), rdm_name, lr);
}
#endif

void Iofunc(Program* p){
    if(STRSAME(get_p_wds(p), SET_s)){
        Set(p);
    }
    else if(STRSAME(get_p_wds(p), PRINT_s)){
        Print(p);
    }else{
        ERROR("Expecting correct instruct.")
    }
}

#ifdef INTERP
vars* find_var(char name[NAME_SIZE], Program* p){
    vars* tmp = p->v;
    while(tmp != NIL){
        if(STRSAME(tmp->name, name)){
            return tmp;
        }
        tmp = tmp->next;
    }
    return NIL;
}

void check_invld_wrd(vars* a, char name[NAME_SIZE]){
    if(a == NIL){
        char error_s[MAX_STR_LEN];
        snprintf(error_s, MAX_STR_LEN, "No word %s founded.", name);
        ERROR(error_s);
    }
}

bool is_tmp(char name[NAME_SIZE]){
    if(name[ZERO] == LWR_T && name[ONE]
        == LWR_M && name[TWO] == LWR_P){
        return true;
    }
    else{
        return false;
    }
}

bool is_lisp_Exist(lisp* l, vars* v){
    if(l == NIL) {
        return false;
    }
    vars* tmp = v;
    char str1[MAXSTR];
    lisp_tostring(l, str1);
    char str2[MAXSTR];
    lisp_tostring(tmp->l, str2);
    if(STRSAME(str1, str2)){
        return true;
    }
    return false;
}
#endif

void Set(Program* p) {
    next_cur_word(p);
#ifdef INTERP
    char tmp_name[NAME_SIZE];
#endif
    if (Var(p)) {
#ifdef INTERP
    strcpy(tmp_name, get_p_wds(p));
#endif
        next_cur_word(p);
    }
    List(p);
#ifdef INTERP
    if (is_name_Exist(tmp_name, p->v)) {
        vars* name_v = find_var(tmp_name, p);
        if (is_tmp(p->v->name)) {
            char rdm_name[NAME_SIZE];
            get_uniq_name(p, rdm_name);
            strcpy(name_v->name, rdm_name);
            strcpy(p->v->name, tmp_name);
        } else {
            char tmp_name1[NAME_SIZE];
            strcpy(tmp_name1, p->v->name);
            p->v = pop_var(p->v);
            vars *b = find_var(tmp_name1, p);
            check_invld_wrd(b, tmp_name1);
            char rdm_name[NAME_SIZE];
            get_uniq_name(p, rdm_name);
            strcpy(name_v->name, rdm_name);
            push_var(&(p->v), tmp_name, b->l);
            b->isFrom = true;
        }
    } else {
        Set_tmp(p, tmp_name);
    }
#endif
}

#ifdef INTERP
void Set_tmp(Program* p, char tmp_name[NAME_SIZE]){
    if (is_tmp(p->v->name)) {
        strcpy(p->v->name, tmp_name);
    }
    else {
        char tmp_name1[NAME_SIZE];
        strcpy(tmp_name1, p->v->name);
        p->v = pop_var(p->v);
        vars *a = find_var(tmp_name1, p);
        check_invld_wrd(a, tmp_name1);
        push_var(&(p->v), tmp_name, a->l);
        p->v->isFrom = true;
    }
}
#endif

void Print(Program* p) {
    next_cur_word(p);
    if (get_first_c(p) == DOUBLE_QUOTE) {
        String(p);
#ifdef INTERP
        fprintf(stderr, "%s\n", get_p_wds(p));
#endif
        next_cur_word(p);
    }else {
        List(p);
#ifdef INTERP
        if(is_tmp(p->v->name)){
            char tmp_str[MAX_STR_LEN];
            lisp_tostring(p->v->l, tmp_str);
            //p->v = pop_var(p->v);
            fprintf(stderr, "%s\n", tmp_str);
        }else {
            char tmp_name[NAME_SIZE];
            strcpy(tmp_name, p->v->name);
            p->v = pop_var(p->v);
            vars *a = find_var(tmp_name, p);
            check_invld_wrd(a, tmp_name);
            char tmp_str[MAX_STR_LEN];
            lisp_tostring(a->l, tmp_str);
            fprintf(stderr, "%s\n", tmp_str);
        }
#endif
    }
}

void String(Program* p){
    if(get_first_c(p) == DOUBLE_QUOTE && get_last_c(p) == DOUBLE_QUOTE){
        return;
    }
    else{
        ERROR("Expecting a correct string with Double quoted.")
    }
}

#ifdef INTERP
void INTERP_CAR(Program* p){
    if(is_tmp(p->v->name)){
        char rdm_name[NAME_SIZE];
        get_uniq_name(p, rdm_name);
        lisp* l = lisp_car(peek_var(p->v));
        push_var(&(p->v), rdm_name, l);
        p->v->isFrom = true;
    }else {
        char tmp_name[NAME_SIZE];
        strcpy(tmp_name, p->v->name);
        p->v = pop_var(p->v);
        vars *a = find_var(tmp_name, p);
        check_invld_wrd(a, tmp_name);
        lisp* l = lisp_car(a->l);
        char rdm_name[NAME_SIZE];
        get_uniq_name(p, rdm_name);
        push_var(&(p->v), rdm_name, l);
        p->v->isFrom = true;
    }
}

void INTERP_CDR(Program* p){
    if(is_tmp(p->v->name)){
        char rdm_name[NAME_SIZE];
        get_uniq_name(p, rdm_name);
        lisp* l = lisp_cdr(peek_var(p->v));
        //p->v = pop_var(p->v);
        push_var(&(p->v), rdm_name, l);
        p->v->isFrom = true;
    }else{
        char tmp_name[NAME_SIZE];
        strcpy(tmp_name, p->v->name);
        p->v = pop_var(p->v);
        vars* a = find_var(tmp_name, p);
        check_invld_wrd(a, tmp_name);
        char rdm_name[NAME_SIZE];
        lisp* l = lisp_cdr(a->l);
        get_uniq_name(p, rdm_name);
        push_var(&(p->v), rdm_name, l);
        p->v->isFrom = true;
    }
}

lisp* INTERP_CONS_1(Program* p){
    if(is_tmp(p->v->name)){
            return lisp_copy(p->v->l);
    }else {
            char tmp_name[NAME_SIZE];
            strcpy(tmp_name, p->v->name);
            p->v = pop_var(p->v);
            vars *car = find_var(tmp_name, p);
            check_invld_wrd(car, tmp_name);
            return lisp_copy(car->l);
    }
}

lisp* INTERP_CONS_2(Program* p){
    if(is_tmp(p->v->name)){
            return lisp_copy(p->v->l);
    }else {
            char tmp_name1[NAME_SIZE];
            strcpy(tmp_name1, p->v->name);
            p->v = pop_var(p->v);
            vars *cdr = find_var(tmp_name1, p);
            check_invld_wrd(cdr, tmp_name1);
            return lisp_copy(cdr->l);
    }
}
#endif

void Listfunc(Program* p){
    if(STRSAME(get_p_wds(p), CAR_s)){
        next_cur_word(p);
        List(p);
#ifdef INTERP
        INTERP_CAR(p);
#endif
    }else if(STRSAME(get_p_wds(p), CDR_s)){
        next_cur_word(p);
        List(p);
#ifdef INTERP
        INTERP_CDR(p);
#endif
    }else if(STRSAME(get_p_wds(p), CONS_s)){
#ifdef INTERP
        lisp* l1 = NIL;
        lisp* l2 = NIL;
#endif
        next_cur_word(p);
        List(p);
#ifdef INTERP
        l1 = INTERP_CONS_1(p);
#endif
        List(p);
#ifdef INTERP
        l2 = INTERP_CONS_2(p);
        lisp* l = lisp_cons(l1, l2);
        char rdm_name[NAME_SIZE];
        get_uniq_name(p, rdm_name);
        push_var(&(p->v), rdm_name, l);
#endif
    }
}

void List(Program *p){
    if(STRSAME(get_p_wds(p), NIL_s)){
#ifdef INTERP
        char tmp_name[NAME_SIZE];
        get_uniq_name(p, tmp_name);
        push_var(&(p->v), tmp_name, NIL);
#endif
        next_cur_word(p);
        return;
    }
    else if(STRSAME(get_p_wds(p), LFT_BRKT_s)){
        next_cur_word(p);
        Retfunc(p);
        if(STRSAME(get_p_wds(p), RGT_BRKT_s)){
            next_cur_word(p);
            return;
        }else{
            ERROR("Expecting a RIGHT bracket.")
        }
    }
    else if(get_first_c(p) == SINGLE_QUOTE){
        Literal(p);
#ifdef INTERP
        char rdm_name[NAME_SIZE];
        get_uniq_name(p, rdm_name);
        lisp* l = lisp_fromstring(p->wds[p->cur_word]);
        push_var(&(p->v), rdm_name, l);
#endif
        next_cur_word(p);
        return;
    }
    else if(Var(p)){
#ifdef INTERP
        push_var(&(p->v), get_p_wds(p), NIL);
#endif
        next_cur_word(p);
        return;
    }
}

bool Var(Program* p){
    if(get_first_c(p) >= letter_A && get_first_c(p) <= letter_Z) {
        return true;
    }
    else{
        ERROR("Expecting a letter from A to Z or a number.")
    }
}

void Literal(Program* p){
    if(get_first_c(p) == SINGLE_QUOTE && get_last_c(p) == SINGLE_QUOTE) {
#ifdef INTERP
        char str[MAXTOKENSIZE];
        strcpy(str, get_p_wds(p));
        int cursor = ONE;           //check the first character after the single quote
        if (isdigit(str[cursor]) || str[cursor] == NEGATIVE){
            cursor++;
            while(str[cursor] != SINGLE_QUOTE){
                if(isdigit(str[cursor]) == ZERO){
                    ERROR("Expecting a legal string.")
                }
                cursor++;
            }
        }
        int l_bkt = ZERO;
        int r_bkt = ZERO;
        while (str[cursor] != SINGLE_QUOTE) {
            is_LFTBKT(str, cursor, &l_bkt);
            is_DIGIT(str, cursor);
            is_RGTBKT(str, cursor, &r_bkt);
            is_SPACE(str, cursor);
            cursor++;
        }
        if (l_bkt == r_bkt) {
#endif
        return;
#ifdef INTERP
        }else{
            ERROR("Expecting a legal string.")
        }
#endif
    }
    else{
        ERROR("Expecting a correct instruct with Single quoted.")
    }
}

#ifdef INTERP
void is_LFTBKT(char str[MAXTOKENSIZE], int cursor, int* l_bkt){
    if (str[cursor] == LFT_BRKT) {
        if (str[cursor + ONE] != NEGATIVE && isdigit(str[cursor + ONE]) == ZERO &&
        str[cursor + ONE] != LFT_BRKT) {
            ERROR("Expecting a legal string.")
        }
        *l_bkt = *l_bkt + ONE;
    }
}

void is_DIGIT(char str[MAXTOKENSIZE], int cursor){
    if (isdigit(str[cursor])) {
        if (str[cursor + ONE] != SPACE && str[cursor + ONE] != RGT_BRKT &&
        isdigit(str[cursor + ONE]) == ZERO) {
            ERROR("Expecting a legal string.")
        }
    }
}

void is_RGTBKT(char str[MAXTOKENSIZE], int cursor, int* r_bkt){
    if (str[cursor] == RGT_BRKT) {
        if (str[cursor + ONE] != RGT_BRKT && str[cursor + ONE] != SINGLE_QUOTE &&
        str[cursor + ONE] != SPACE) {
            ERROR("Expecting a legal string.")
        }
        *r_bkt = *r_bkt + ONE;
    }
}

void is_SPACE(char str[MAXTOKENSIZE], int cursor){
    if (str[cursor] == SPACE) {
        if (isdigit(str[cursor + ONE]) == ZERO && str[cursor + ONE] != LFT_BRKT &&
        str[cursor + ONE] != NEGATIVE) {
            ERROR("Expecting a legal string.")
        }
    }
}
#endif

char get_first_c(Program* p){
    char c = p->wds[p->cur_word][ZERO];
    return c;
}

char get_last_c(Program* p){
    int l = (int) strlen(get_p_wds(p)) - ONE;
    char c = p->wds[p->cur_word][l];
    return c;
}

#ifdef INTERP
void get_uniq_name(Program* p, char* name){
    snprintf(name, NAME_SIZE, "%s_%i", "tmp", rand() % MAX_RAND);
    if(is_name_Exist(name, p->v) == false){
        return;
    }else{
        while(is_name_Exist(name, p->v) == true){
            snprintf(name, NAME_SIZE, "%s_%i", "tmp", rand() % MAX_RAND);
        }
        return;
    }
}

void push_var(vars** v, char* name, lisp* l){
    vars* new_v = (vars*) calloc(ONE, sizeof(vars));
    strcpy(new_v->name, name);
    new_v->l = l;
    new_v->isFrom = false;
    new_v->next = *v;
    *v = new_v;
}

lisp* peek_var(vars* v){
    if(!v || !(v->l)) {
        return NIL;
    }
    return v->l;
}

vars* pop_var(vars* v){
    if(!v){
        return NIL;
    }
    v->l = NIL;
    vars* tmp_v = v;
    v = v->next;
    free(tmp_v);
    return v;
}

bool is_name_Exist(char name[NAME_SIZE], vars* v){
    vars* tmp = v;
    while(tmp != NIL){
        if(STRSAME(tmp->name, name)){
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}
#endif

void free_var(vars** v){
    if(!(*v)){
        return;
    }
    while((*v) != NIL){
        vars* next_v = (*v)->next;
#ifdef INTERP
        lisp_free(&((*v)->l));
#endif
        free(*v);
        *v = next_v;
    }
}

void Prog_free(Program* p){
    if(!p){
        return;
    }
    vars* tmp = p->v;
    while(tmp != NIL){
        if(tmp->isFrom == true){
            tmp->l = NIL;
        }
        tmp = tmp->next;
    }
#ifdef INTERP
    free_var(&(p->v));
#endif
    free(p);
}

void test_nuclei(void){
    tst_get_wds_get_c();
    tst_init_Prog();
    tst_p_NIL();
#ifdef INTERP
    tst_find_var_lisp_name_exist();
    tst_isFrom();
#endif
}

#ifdef INTERP
void tst_find_var_lisp_name_exist(void){
    Program *p = init_Prog();
    lisp* l1 = lisp_fromstring("(1)");
    push_var(&(p->v), "tst1", l1);
    lisp* l2 = lisp_fromstring("(2)");
    assert(is_lisp_Exist(l1, p->v) == true);
    assert(is_lisp_Exist(NIL, p->v) == false);
    assert(is_lisp_Exist(l2, p->v) == false);
    assert(is_name_Exist("tst1", p->v) == true);
    assert(is_name_Exist("tst2", p->v) == false);
    push_var(&(p->v), "tst2", l2);
    char result[MAXSTR];
    vars* tst = find_var("tst1", p);
    lisp_tostring(tst->l, result);
    assert(STRSAME(result, "(1)"));
    Prog_free(p);
}

void tst_isFrom(void){
    char tmp[MAX_STR_LEN];
    Program *p = init_Prog();
    strcpy(tmp, "((SET F '(2)')(SET G F))");
    get_str(tmp, p);
    Prog(p);
    vars *g = find_var("G", p);
    assert(g->isFrom == true);
    Prog_free(p);
}
#endif

void tst_p_NIL(void){
    Program *p = NIL;
    assert(get_p_wds(p) == ZERO);
    Prog_free(p);
}

void tst_init_Prog(void){
    Program *p = init_Prog();
    assert(p->v == NIL);
    assert(p->cur_word == ZERO);
    Prog_free(p);
}

void tst_get_wds_get_c(void){
    Program* p = init_Prog();
    char tst_str[MAX_STR_LEN] = "(PRINT (CONS '1' (CONS '2' NIL)))";
    get_str(tst_str, p);
    p->cur_word = ini_cur_word(p);
    assert(STRSAME(get_p_wds(p), "("));
    next_cur_word(p);
    assert(STRSAME(get_p_wds(p), "PRINT"));
    next_cur_word(p);
    next_cur_word(p);
    next_cur_word(p);
    assert(STRSAME(get_p_wds(p), "'1'"));
    char c1 = get_first_c(p);
    char c2 = get_last_c(p);
    assert(c1 == SINGLE_QUOTE);
    assert(c2 == SINGLE_QUOTE);
    Prog_free(p);
}
