#ifndef LEXER_H
#define LEXER_H

#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<stdio.h>

typedef enum {
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH, TOKEN_NUMBER, TOKEN_PRINT, TOKEN_INPUT, TOKEN_IF, TOKEN_ASSIGN, TOKEN_LBRACE, TOKEN_RBRACE, TOKEN_LPAREN, TOKEN_RPAREN , TOKEN_SEMICOLON, TOKEN_END, TOKEN_IDENTIFIER
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

typedef struct {
    char *source;
    size_t position;
} Lexer;

Token *create_token(TokenType type, const char *value);
void free_token(Token *token);
void free_tokens(Token *tokens);
Lexer *create_lexer(const char *source);
void free_lexer(Lexer *lexer);
Token *lexer_next_token(Lexer *lexer);
Token *tokenize(Lexer *lexer);

#endif
