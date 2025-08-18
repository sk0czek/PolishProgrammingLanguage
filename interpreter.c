#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include "lexer.h"
#include "parser.h"

typedef struct {
    char *name;
    int value;
} Variable;

// tworzy nowa zmienna
Variable *create_variable(const char *name, int value){
    Variable *variable = malloc(sizeof(Variable));
    variable->name = malloc(strlen(name) + 1);
    strcpy(variable->name, name);
    variable->value = value;

    return variable;
}

// zwalnia pamiec przydzielona na zmienna
void free_variable(Variable *variable){
    if(variable){
        free(variable->name);
        free(variable);
    }
}

// tablica wskaznikow do zmiennych
typedef struct {
    Variable **variables;
    int count;
} Environment;

// tworzy srodowisko
Environment *create_environment(){
    Environment *env = malloc(sizeof(Environment));
    env->variables = NULL;
    env->count = 0;

    return env;
}

// zwalnia pamiec przydzielona na srodowisko
void free_environment(Environment *env){
    if(env){
        for(int i = 0; i < env->count; i++){
            free_variable(env->variables[i]);
        }
        free(env->variables);
        free(env);
    }
}

// szuka zmiennej w srodowisku po nazwie
Variable *find_variable(Environment *env, const char *name){
    for(int i = 0; i < env->count; i++){
        if(strcmp(env->variables[i]->name, name) == 0){
            return env->variables[i];
        }
    }
    return NULL;
}

// dodaje nowa zmienna do srodowiska
void add_variables(Environment *env, Variable *variable){
    env->variables = realloc(env->variables, (env->count + 1) * sizeof(Variable *));
    env->variables[env->count] = variable;
    env->count++;
}

// ocenia wartosc wyrazenia
int evaluate_expression(Environment *env, ASTNode *node){
    if(node->type != 5) {
        printf("To nie jest wyrazenie!\n");
        exit(1);
    }
    
    if(node->expr_type == 2) { // number
        return node->expr_number;
    }
    else if(node->expr_type == 3) { // identifier
        Variable *variable = find_variable(env, node->expr_identifier);     
        if(variable){
            return variable->value;
        }
        else {
            printf("Nieznana zmienna: %s\n", node->expr_identifier);
            exit(1);
        }
    }
    else if(node->expr_type == 1) { // binary
        int left = evaluate_expression(env, node->expr_left);
        int right = evaluate_expression(env, node->expr_right);
        
        if(node->expr_op == '+') {
            return left + right;
        }
        else if(node->expr_op == '-') {
            return left - right;
        }
        else if(node->expr_op == '*') {
            return left * right;
        }
        else if(node->expr_op == '/') {
            return left / right;
        }
        else {
            printf("Nieznany operator: %c\n", node->expr_op);
            exit(1);
        }
    }
    else {
        printf("Nieznany typ wyrazenia\n");
        exit(1);
    }
}

void interpret_statement(Environment *env, ASTNode *node){
    if(node->type == 1) { // print
        int value = evaluate_expression(env, node->print_expression);
        printf("%d\n", value);
    }
    else if(node->type == 2) { // input
        int value;
        printf("Podaj wartosc dla %s: ", node->input_identifier);
        scanf("%d", &value);

        Variable *variable = find_variable(env, node->input_identifier);
        if(variable){
            variable->value = value;
        }
        else {
            add_variables(env, create_variable(node->input_identifier, value));
        }
    }
    else if(node->type == 3) { // assignment
        int value = evaluate_expression(env, node->assign_expression);
        Variable *variable = find_variable(env, node->assign_identifier);
        if(variable){
            variable->value = value;
        }
        else {
            add_variables(env, create_variable(node->assign_identifier, value));
        }
    }
    else if(node->type == 4) { // if
        int condition = evaluate_expression(env, node->if_condition);
        if(condition != 0) { // jezeli warunek jest prawdziwy
            interpret_statement(env, node->if_body);
        }
    }
    else {
        printf("Nieznany typ instrukcji\n");
    }
}

void interpret_program(ASTNode *program){
    Environment *env = create_environment();
    ASTNode *current = program;

    while(current){
        interpret_statement(env, current);
        current = current->next;
    }

    free_environment(env); 
}
