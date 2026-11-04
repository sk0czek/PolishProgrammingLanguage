#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<limits.h>
#include "lexer.h"
#include "parser.h"

typedef struct {
    char *name;
    int value;
} Variable;

Variable *create_variable(const char *name, int value){
    Variable *variable = malloc(sizeof(Variable));
    if(!variable){
        printf("Brak pamieci na zmienna\n");
        exit(1);
    }
    variable->name = malloc(strlen(name) + 1);
    if(!variable->name){
        printf("Brak pamieci na nazwe zmiennej\n");
        free(variable);
        exit(1);
    }
    strcpy(variable->name, name);
    variable->value = value;

    return variable;
}

void free_variable(Variable *variable){
    if(variable){
        free(variable->name);
        free(variable);
    }
}

typedef struct {
    Variable **variables;
    int count;
} Environment;

Environment *create_environment(void){
    Environment *env = malloc(sizeof(Environment));
    if(!env){
        printf("Brak pamieci na srodowisko\n");
        exit(1);
    }
    env->variables = NULL;
    env->count = 0;

    return env;
}

void free_environment(Environment *env){
    if(env){
        for(int i = 0; i < env->count; i++){
            free_variable(env->variables[i]);
        }
        free(env->variables);
        free(env);
    }
}

Variable *find_variable(Environment *env, const char *name){
    for(int i = 0; i < env->count; i++){
        if(strcmp(env->variables[i]->name, name) == 0){
            return env->variables[i];
        }
    }
    return NULL;
}

void add_variables(Environment *env, Variable *variable){
    Variable **variables = realloc(env->variables, (env->count + 1) * sizeof(Variable *));
    if(!variables){
        printf("Brak pamieci na tablice zmiennych\n");
        free_variable(variable);
        exit(1);
    }
    env->variables = variables;
    env->variables[env->count] = variable;
    env->count++;
}

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
        long long result;
        
        if(node->expr_op == '+') {
            result = (long long)left + right;
        }
        else if(node->expr_op == '-') {
            result = (long long)left - right;
        }
        else if(node->expr_op == '*') {
            result = (long long)left * right;
        }
        else if(node->expr_op == '/') {
            if(right == 0){
                printf("Dzielenie przez zero\n");
                exit(1);
            }
            if(left == INT_MIN && right == -1){
                printf("Wynik poza zakresem typu int\n");
                exit(1);
            }
            return left / right;
        }
        else {
            printf("Nieznany operator: %c\n", node->expr_op);
            exit(1);
        }
        if(result < INT_MIN || result > INT_MAX){
            printf("Wynik poza zakresem typu int\n");
            exit(1);
        }
        return (int)result;
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
        char input[128];
        printf("Podaj wartosc dla %s: ", node->input_identifier);
        fflush(stdout);
        if(scanf("%127s", input) != 1){
            printf("Nie udalo sie odczytac liczby\n");
            exit(1);
        }
        int c = getchar();
        if(c != EOF && !isspace((unsigned char)c)){
            printf("Podana liczba jest zbyt dluga\n");
            exit(1);
        }
        char *end;
        errno = 0;
        long number = strtol(input, &end, 10);
        if(errno == ERANGE || *end != '\0' || number < INT_MIN || number > INT_MAX){
            printf("Nieprawidlowa liczba: %s\n", input);
            exit(1);
        }
        int value = (int)number;

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
        if(condition != 0) {
            ASTNode *current = node->if_body;
            while(current){
                interpret_statement(env, current);
                current = current->next;
            }
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
