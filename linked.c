#include "specific.h"

// Returns element 'a' - this is not a list, and
// by itself would be printed as e.g. "3", and not "(3)"
lisp *lisp_atom(const atomtype a)
{
    lisp *l = (lisp *)ncalloc(ONE, sizeof(lisp));
    l->car = NIL;
    l->cdr = NIL;
    l->atom = a;
    return l;
}

// Returns a list containing the car as 'l1'
// and the cdr as 'l2'- data in 'l1' and 'l2' are reused,
// and not copied. Either 'l1' and/or 'l2' can be NULL
lisp *lisp_cons(const lisp *l1, const lisp *l2)
{
    if (!l1 && !l2)
    {
        return NIL;
    }
    lisp *new_l = lisp_atom(ZERO);
    new_l->car = (lisp *)l1;
    new_l->cdr = (lisp *)l2;
    new_l->atom = ZERO;
    return new_l;
}

// Returns the car (1st) component of the list 'l'.
// Does not copy any data.
lisp *lisp_car(const lisp *l)
{
    if (!l)
    {
        return NIL;
    }
    return l->car;
}

// Returns the cdr (all but the 1st) component of the list 'l'.
// Does not copy any data.
lisp *lisp_cdr(const lisp *l)
{
    if (!l)
    {
        return NIL;
    }
    return l->cdr;
}

// Returns the data/value stored in the cons 'l'
atomtype lisp_getval(const lisp *l)
{
    if (!l)
    {
        return ZERO;
    }
    return l->atom;
}

// Returns a boolean depending up whether l points to an atom (not a list)
bool lisp_isatomic(const lisp *l)
{
    if (!l)
    {
        return false;
    }
    if (lisp_car(l) == NIL && lisp_cdr(l) == NIL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Returns a deep copy of the list 'l'
lisp *lisp_copy(const lisp *l)
{
    if (!l)
    {
        return NIL;
    }
    lisp *new_l = lisp_atom(lisp_getval(l));
    new_l->cdr = lisp_copy(lisp_cdr(l));
    new_l->car = lisp_copy(lisp_car(l));
    return new_l;
}

// Returns number of components in the list.
int lisp_length(const lisp *l)
{
    if (!l)
    {
        return ZERO;
    }
    if (lisp_isatomic(l))
    {
        return ZERO;
    }
    int length = lisp_length(lisp_cdr(l)) + ONE;
    return length;
}

// Returns stringified version of list
void lisp_tostring(const lisp *l, char *str)
{
    if (!l)
    {
        snprintf(str, LISTSTRLEN, CHAR_FORMAT, LFTBKT);
        snprintf(++str, LISTSTRLEN, CHAR_FORMAT, RGTBKT);
        return;
    }
    if (lisp_isatomic(l))
    {
        snprintf(str, LISTSTRLEN, INT_FORMAT, lisp_getval(l));
        return;
    }
    if (lisp_car(l))
    {
        snprintf(str, LISTSTRLEN, CHAR_FORMAT, LFTBKT);
        lisp_tostring(lisp_car(l), ++str);
        while (*str != STR_END)
        {
            str++;
        }
    }
    lisp *tmp_l = lisp_cdr(l);
    while (tmp_l)
    {
        snprintf(str, LISTSTRLEN, CHAR_FORMAT, SPACE);
        lisp_tostring(lisp_car(tmp_l), ++str);
        tmp_l = lisp_cdr(tmp_l);
        while (*str != STR_END)
        {
            str++;
        }
    }
    snprintf(str, LISTSTRLEN, CHAR_FORMAT, RGTBKT);
}

// Clears up all space used
// Double pointer allows function to set 'l' to NULL on success
void lisp_free(lisp **l)
{
    if (*l)
    {
        lisp_free(&((*l)->car));
        lisp_free(&((*l)->cdr));
        free(*l);
        (*l) = NIL;
    }
}

void test(void)
{
    // char str[LISTSTRLEN];

    // // another case
    // lisp *l1 = lisp_cons(lisp_atom(5), NIL);
    // assert(l1);
    // assert(lisp_length(l1) == 1);
    // lisp_tostring(l1, str);
    // assert(strcmp(str, "(5)") == 0);
    // assert(lisp_getval(lisp_car(l1)) == 5);
    // assert(lisp_isatomic(l1) == false);
    // assert(lisp_isatomic(lisp_car(l1)) == true);

    // // testing double-digit integer
    // lisp *l2 = lisp_cons(lisp_atom(17), l1);
    // assert(l2);
    // assert(lisp_length(l2) == 2);
    // lisp_tostring(l2, str);
    // assert(strcmp(str, "(17 5)") == 0);

    // // testing triple bracket
    // lisp *l3 = lisp_cons(lisp_atom(3), lisp_cons(lisp_atom(4), lisp_cons(lisp_atom(6), NIL)));
    // assert(l3);
    // assert(lisp_length(l3) == 3);
    // lisp_tostring(l3, str);
    // assert(strcmp(str, "(3 4 6)") == 0);
    // lisp *l4 = lisp_cons(lisp_atom(9), NIL);
    // lisp *l5 = lisp_cons(l4, l3);
    // lisp *l6 = lisp_cons(l5, l2);
    // assert(lisp_length(l6) == 3);
    // lisp_tostring(l6, str);
    // assert(strcmp(str, "(((9) 3 4 6) 17 5)") == 0);
    // lisp_free(&l6);

    stack* s = NIL;
    lisp *lt1 = lisp_cons(lisp_atom(3), NIL);
    stack_push(&s, lt1, false);
    lisp *lt2 = lisp_cons(lisp_atom(1), NIL);
    stack_push(&s, lt2, false);
    lisp *lt3 = lisp_cons(lisp_atom(7), NIL);
    stack_push(&s, lt3, false);
    lisp* lt = stack_peek(s);
    s = stack_pop(s);
    stack_push(&s, lt, false);
    stack_free(&s);
}

lisp *lisp_fromstring(const char *str){
    stack* s = NIL;
    char* cursor = (char*) str;
    if(*cursor == STR_END){
        return NIL;
    }
    if(*cursor == LFTBKT && *(cursor + ONE) == RGTBKT){
        return NIL;
    }
    while(*cursor != STR_END) {
        if(*cursor == SINGLE_QUOTE){
            cursor++;
        }
        if(*cursor == LFTBKT){
            lisp* l = lisp_atom(ZERO);
            stack_push(&s, l, true);
            cursor++;
            if(*cursor == SPACE){
                return NIL;
            }
        }
        else if(*cursor == SPACE){
            cursor++;
            if(*cursor == SPACE){
                return NIL;
            }
        }
        else if (*cursor == NEGATIVE || isdigit(*cursor)){
            int value = ZERO;
            bool isNegative = false;
            if(*cursor == NEGATIVE){
                isNegative = true;
                cursor++;
            }
            while(*cursor != SPACE && *cursor != RGTBKT && *cursor != SINGLE_QUOTE){
                if(*cursor == LFTBKT){
                    return NIL;
                }
                value = value * TEN + (*cursor - ZERO_s);
                cursor++;
            }
            if(*cursor == SPACE){
                if(*(cursor + ONE) != LFTBKT && !isdigit(*(cursor + ONE))
                    && *(cursor + ONE) != NEGATIVE){
                    return NIL;
                }
            }
            if(isNegative == true){
                value = -value;
            }
            lisp* l_car = lisp_atom(value);
            lisp* l = lisp_cons(l_car, NIL);
            stack_push(&s, l, false);
        }
        else if (*cursor == RGTBKT) {
            bool stop = false;
            lisp* tmp_l = NIL;
            while (s->is_LFTBRKT == false && stop == false) {
                tmp_l = stack_peek(s);
                s = stack_pop(s);
                if(s->is_LFTBRKT == false){
                    s->l->cdr = tmp_l;
                }else if(s->is_LFTBRKT == true) {
                    s->l->car = tmp_l;
                    s->is_LFTBRKT = false;
                    stop = true;
                }
            }
            cursor++;
        }
    }
    lisp* l = s->l->car;
    s->l->car = NIL; 
    stack_free(&s);
    return l;
}

//Add element to top
void stack_push(stack** s, lisp* l, bool is_l_BRKT){
    stack* new_s = (stack*) calloc(ONE, sizeof(stack));
    new_s->l = l;
    new_s->is_LFTBRKT = is_l_BRKT;
    new_s->next = *s;
    *s = new_s;
}

//Take element from top
stack* stack_pop(stack* s){
    if(!s){
        return NIL;
    }
    s->l = NIL;
    stack* p = s;
    s = s->next;
    free(p);
    return s;
}

//Clears all space used
void stack_free(stack** s){
    if(!(*s)){
        return;
    }
    while((*s) != NIL){
        stack* tmp = (*s)->next;
        lisp_free(&((*s)->l));
        free(*s);
        *s = tmp;
    }
}

//Copy top element into l (but don't pop it)
lisp* stack_peek(stack* s){
    if(!s || !(s->l)){
        return NIL;
    }
    return s->l;
}

// Allow a user defined function 'func' to be applied to
// each atom in the list l.
// The user-defined 'func' is passed a pointer to a cons,
// and will maintain an accumulator of the result.
void lisp_reduce(void (*func)(lisp* l, atomtype* n), lisp* l, atomtype* acc){
    if(!l){
        return;
    }
    if(lisp_isatomic(l)) {
        func(l, acc);
        return;
    }
    lisp_reduce(func, l->car, acc);
    lisp_reduce(func, l->cdr, acc);
}

// Multiplies getval() of all atoms
void times(lisp* l, atomtype* accum)
{
    *accum = *accum * lisp_getval(l);
}

// To count number of atoms in list, including sub-lists
void atms(lisp* l, atomtype* accum)
{
    // Could just add one (since each node must be atomic),
    // but prevents unused warning for variable 'l'...
    *accum = *accum + lisp_isatomic(l);
}

void sum(lisp* l, atomtype* accum)
{
    *accum = *accum + lisp_getval(l);
}
