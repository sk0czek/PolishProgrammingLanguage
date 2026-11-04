#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "lexer.h"

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

ASTNode *create_node(void){
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node){
        printf("Brak pamieci na wezel AST\n");
        exit(1);
    }
    
    node->type = 0;
    node->print_expression = NULL;
    node->input_identifier = NULL;
    node->assign_identifier = NULL;
    node->assign_expression = NULL;
    node->if_condition = NULL;
    node->if_body = NULL;
    node->expr_type = 0;
    node->expr_left = NULL;
    node->expr_op = 0;
    node->expr_right = NULL;
    node->expr_number = 0;
    node->expr_identifier = NULL;
    node->next = NULL;
    
    return node;
}

void free_node(ASTNode *node){
    while(node){
        ASTNode *next_node = node->next;
        
        if(node->type == 1) { // print
            free_node(node->print_expression);
        }
        else if(node->type == 2) { // input
            if(node->input_identifier) free(node->input_identifier);
        }
        else if(node->type == 3) { // assignment
            if(node->assign_identifier) free(node->assign_identifier);
            free_node(node->assign_expression);
        }
        else if(node->type == 4) { // if
            free_node(node->if_condition);
            free_node(node->if_body);
        }
        else if(node->type == 5) { // expression
            if(node->expr_type == 1) { // binary
                free_node(node->expr_left);
                free_node(node->expr_right);
            }
            else if(node->expr_type == 3) { // identifier
                if(node->expr_identifier) free(node->expr_identifier);
            }
        }
        
        free(node);
        node = next_node;
    }
}

typedef struct {
    Token *tokens;
    size_t position;
} Parser;

Parser *create_parser(Token *tokens){
    Parser *parser = malloc(sizeof(Parser));
    if(!parser){
        printf("Brak pamieci na parser\n");
        exit(1);
    }
    parser->tokens = tokens;
    parser->position = 0;
    return parser;
}

void free_parser(Parser *parser){
    if(parser){
        free(parser);
    }
}

Token *current_token(Parser *parser){
    return &parser->tokens[parser->position];
}

Token *next_token(Parser *parser){
    parser->position++;
    return current_token(parser);
}

Token *expect_token(Parser *parser, TokenType type){
    Token *token = current_token(parser);
    if(token->type != type){
        printf("Oczekiwano token typu %d, ale dostano %d\n", type, token->type);
        exit(1);
    }
    next_token(parser);
    return token; 
}

ASTNode *parse_expression(Parser *parser);
ASTNode *parse_statement(Parser *parser);

// Parse the magnitude separately so INT_MIN remains a valid literal.
static ASTNode *parse_number(Parser *parser, int negative){
    Token *token = current_token(parser);
    char *end;
    errno = 0;
    unsigned long value = strtoul(token->value, &end, 10);
    unsigned long limit = negative ? (unsigned long)(-(INT_MIN + 1)) + 1 : (unsigned long)INT_MAX;
    if(errno == ERANGE || *end != '\0' || value > limit){
        printf("Liczba poza zakresem typu int: %s\n", token->value);
        exit(1);
    }

    ASTNode *node = create_node();
    node->type = 5; // expression
    node->expr_type = 2; // number
    if(negative){
        node->expr_number = value == limit ? INT_MIN : -(int)value;
    }
    else {
        node->expr_number = (int)value;
    }
    next_token(parser);
    return node;
}

static ASTNode *parse_primary(Parser *parser){
    Token *token = current_token(parser);
    if(token->type == TOKEN_NUMBER){
        return parse_number(parser, 0);
    }
    else if (token->type == TOKEN_IDENTIFIER){
        ASTNode *node = create_node();
        node->type = 5; // expression
        node->expr_type = 3; // identifier
        node->expr_identifier = malloc(strlen(token->value) + 1);
        if(!node->expr_identifier){
            printf("Brak pamieci na nazwe zmiennej\n");
            exit(1);
        }
        strcpy(node->expr_identifier, token->value);
        next_token(parser);
        return node;
    }
    else if(token->type == TOKEN_LPAREN){
        next_token(parser);
        ASTNode *node = parse_expression(parser);
        expect_token(parser, TOKEN_RPAREN);
        return node;
    }
    else {
        printf("Nieoczekiwany token w wyrazeniu: %s\n", token->value ? token->value : "koniec pliku");
        exit(1);
    }
}

static ASTNode *parse_unary(Parser *parser){
    TokenType type = current_token(parser)->type;
    if(type == TOKEN_PLUS || type == TOKEN_MINUS){
        next_token(parser);
        if(type == TOKEN_PLUS){
            return parse_unary(parser);
        }
        if(current_token(parser)->type == TOKEN_NUMBER){
            return parse_number(parser, 1);
        }

        // Represent unary minus as 0 - operand.
        ASTNode *zero = create_node();
        zero->type = 5; // expression
        zero->expr_type = 2; // number

        ASTNode *node = create_node();
        node->type = 5; // expression
        node->expr_type = 1; // binary
        node->expr_op = '-';
        node->expr_left = zero;
        node->expr_right = parse_unary(parser);
        return node;
    }
    return parse_primary(parser);
}

// Multiplication and division bind more tightly than addition and subtraction.
static ASTNode *parse_term(Parser *parser){
    ASTNode *left = parse_unary(parser);
    while(current_token(parser)->type == TOKEN_STAR || current_token(parser)->type == TOKEN_SLASH){
        char op = current_token(parser)->value[0];
        next_token(parser);

        ASTNode *node = create_node();
        node->type = 5; // expression
        node->expr_type = 1; // binary
        node->expr_op = op;
        node->expr_left = left;
        node->expr_right = parse_unary(parser);
        left = node;
    }
    return left;
}

ASTNode *parse_expression(Parser *parser){
    ASTNode *left = parse_term(parser);
    while(current_token(parser)->type == TOKEN_PLUS || current_token(parser)->type == TOKEN_MINUS){
        char op = current_token(parser)->value[0];
        next_token(parser);

        ASTNode *node = create_node();
        node->type = 5; // expression
        node->expr_type = 1; // binary
        node->expr_op = op;
        node->expr_left = left;
        node->expr_right = parse_term(parser);
        left = node;
    }
    return left;
}

ASTNode *parse_print_statement(Parser *parser){
    expect_token(parser, TOKEN_PRINT);
    expect_token(parser, TOKEN_LPAREN);
    ASTNode *expression = parse_expression(parser);
    expect_token(parser, TOKEN_RPAREN);
    expect_token(parser, TOKEN_SEMICOLON);

    ASTNode *node = create_node();
    node->type = 1; // print
    node->print_expression = expression;

    return node;
}

ASTNode *parse_input_statement(Parser *parser){
    expect_token(parser, TOKEN_INPUT);
    expect_token(parser, TOKEN_LPAREN);
    Token *identifier = expect_token(parser, TOKEN_IDENTIFIER); 
    expect_token(parser, TOKEN_RPAREN);
    expect_token(parser, TOKEN_SEMICOLON);

    ASTNode *node = create_node();
    node->type = 2; // input
    node->input_identifier = malloc(strlen(identifier->value) + 1);
    if(!node->input_identifier){
        printf("Brak pamieci na nazwe zmiennej\n");
        exit(1);
    }
    strcpy(node->input_identifier, identifier->value);
    
    return node;
}

ASTNode *parse_assignment(Parser *parser){
    Token *identifier = expect_token(parser, TOKEN_IDENTIFIER);
    expect_token(parser, TOKEN_ASSIGN);
    ASTNode *expression = parse_expression(parser);
    expect_token(parser, TOKEN_SEMICOLON);

    ASTNode *node = create_node();
    node->type = 3; // assignment
    node->assign_identifier = malloc(strlen(identifier->value) + 1);
    if(!node->assign_identifier){
        printf("Brak pamieci na nazwe zmiennej\n");
        exit(1);
    }
    strcpy(node->assign_identifier, identifier->value);
    node->assign_expression = expression;

    return node;
}

ASTNode *parse_if_statement(Parser *parser){
    expect_token(parser, TOKEN_IF); 
    expect_token(parser, TOKEN_LPAREN);
    ASTNode *condition = parse_expression(parser);
    expect_token(parser, TOKEN_RPAREN);
    expect_token(parser, TOKEN_LBRACE);

    ASTNode *body = NULL;
    ASTNode *last_statement = NULL;
    
    while(current_token(parser)->type != TOKEN_RBRACE){
        if(current_token(parser)->type == TOKEN_END){
            printf("Brak zamykajacego nawiasu }\n");
            exit(1);
        }
        ASTNode *statement = parse_statement(parser);
        if(!body){
            body = statement;
        }
        else {
            last_statement->next = statement;
        }
        last_statement = statement;
    }

    expect_token(parser, TOKEN_RBRACE);

    ASTNode *node = create_node();
    node->type = 4; // if
    node->if_condition = condition;
    node->if_body = body;

    return node;
}

ASTNode *parse_statement(Parser *parser){
    Token *token = current_token(parser);
    if(token->type == TOKEN_PRINT){
        return parse_print_statement(parser);
    }
    else if(token->type == TOKEN_INPUT){
        return parse_input_statement(parser);
    }
    else if(token->type == TOKEN_IDENTIFIER){
        return parse_assignment(parser);
    }
    else if(token->type == TOKEN_IF){
        return parse_if_statement(parser);
    }
    else {
        printf("Nieoczekiwany token w instrukcji: %s\n", token->value ? token->value : "koniec pliku");
        exit(1);
    }
}

ASTNode *parse_program(Parser *parser) {
    ASTNode *program = NULL;
    ASTNode *last_statement = NULL;
    
    while (1) {
        if (current_token(parser)->type == TOKEN_END) {
            break;
        }
        ASTNode *statement = parse_statement(parser);
        if (!statement) break;
        
        if (!program) {
            program = statement;
        } else {
            last_statement->next = statement;
        }
        last_statement = statement;
    }
    return program;
}
