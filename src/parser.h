#ifndef PARSER_H
#define PARSER_H

#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<stdio.h>

typedef struct ASTNode {
    int type; // 1=print, 2=input, 3=assignment, 4=if, 5=expression
    
    struct ASTNode *print_expression;
    
    char *input_identifier;
    
    char *assign_identifier;
    struct ASTNode *assign_expression;
    
    struct ASTNode *if_condition;
    struct ASTNode *if_body;
    
    int expr_type; // 1=binary, 2=number, 3=identifier
    struct ASTNode *expr_left;
    char expr_op;
    struct ASTNode *expr_right;
    int expr_number;
    char *expr_identifier;
    
    struct ASTNode *next;
} ASTNode;

typedef struct {
    Token *tokens;
    size_t position;
} Parser;

ASTNode *create_node(void);
void free_node(ASTNode *node);
Parser *create_parser(Token *tokens);
void free_parser(Parser *parser);
Token *current_token(Parser *parser);
Token *next_token(Parser *parser);
Token *expect_token(Parser *parser, TokenType type);
ASTNode *parse_expression(Parser *parser);
ASTNode *parse_print_statement(Parser *parser);
ASTNode *parse_input_statement(Parser *parser);
ASTNode *parse_if_statement(Parser *parser);
ASTNode *parse_assignment(Parser *parser);
ASTNode *parse_statement(Parser *parser);
ASTNode *parse_program(Parser *parser);

#endif
