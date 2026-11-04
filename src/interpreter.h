#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "lexer.h"
#include "parser.h"

typedef struct {
    char *name;
    int value;
} Variable;

typedef struct {
    Variable **variables;
    int count;
} Environment;

Variable *create_variable(const char *name, int value);
void free_variable(Variable *variable);
Environment *create_environment(void);
void free_environment(Environment *env);
Variable *find_variable(Environment *env, const char *name);
void add_variables(Environment *env, Variable *variable);
int evaluate_expression(Environment *env, ASTNode *node);
void interpret_statement(Environment *env, ASTNode *node);
void interpret_program(ASTNode *program);

#endif 