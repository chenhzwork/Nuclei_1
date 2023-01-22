#pragma once
#include "lisp.h"
#include <ctype.h>

struct lisp
{
    atomtype atom;
    struct lisp *car;
    struct lisp *cdr;
};

typedef struct stack{
    lisp* l;
    bool is_LFTBRKT;
    struct stack* next;
}stack;

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#define ONE 1
#define ZERO 0
#define ZERO_s '0'
#define NIL NULL
#define SPACE ' '
#define LFTBKT '('
#define RGTBKT ')'
#define STR_END '\0'
#define INT_FORMAT "%i"
#define CHAR_FORMAT "%c"
#define LISTSTRLEN 1000
#define TEN 10
#define NEGATIVE '-'
#define SINGLE_QUOTE '\''

void stack_push(stack** s, lisp* l, bool is_l_BRKT);
stack* stack_pop(stack* s);
void stack_free(stack** s);
lisp* stack_peek(stack* s);
