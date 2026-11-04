#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>

typedef enum {
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH, TOKEN_NUMBER, TOKEN_PRINT, TOKEN_INPUT, TOKEN_IF, TOKEN_ASSIGN, TOKEN_LBRACE, TOKEN_RBRACE, TOKEN_LPAREN, TOKEN_RPAREN , TOKEN_SEMICOLON, TOKEN_END, TOKEN_IDENTIFIER
} TokenType;

typedef struct {
    char *source;
    size_t position;
} Lexer;

typedef struct {
    TokenType type;
    char *value;
} Token;

Token *create_token(TokenType type, const char *value){
    Token *token = malloc(sizeof(Token));
    if(!token){
        printf("Brak pamieci na token\n");
        exit(1);
    }
    token->type = type;
    token->value = malloc(strlen(value) + 1);
    if(!token->value){
        printf("Brak pamieci na wartosc tokenu\n");
        free(token);
        exit(1);
    }
    strcpy(token->value, value);
    return token;
}

void free_token(Token *token){
    if(token) {
        free(token->value);
        free(token);
    }
}

void free_tokens(Token *tokens) {
    size_t i = 0;
    while(tokens[i].type != TOKEN_END) {
        if(tokens[i].value) {
            free(tokens[i].value);
        }
        i++;
    }
    free(tokens);
}

Lexer *create_lexer(const char *source){
    Lexer *lexer = malloc(sizeof(Lexer));
    if(!lexer){
        printf("Brak pamieci na lexer\n");
        exit(1);
    }
    lexer->source = malloc(strlen(source) + 1);
    if(!lexer->source){
        printf("Brak pamieci na kod zrodlowy\n");
        free(lexer);
        exit(1);
    }
    strcpy(lexer->source, source);
    lexer->position = 0;
    return lexer;
}

void free_lexer(Lexer *lexer){
    if(lexer){
        free(lexer->source);
        free(lexer);
    }
}

Token *lexer_next_token(Lexer *lexer){
    while(lexer->source[lexer->position] != '\0'){
        char current = lexer->source[lexer->position];

        if(isspace((unsigned char)current)){
            lexer->position++;
            continue;
        } 

        if(current == '/' && lexer->source[lexer->position + 1] == '/'){
            while(lexer->source[lexer->position] != '\0' && lexer->source[lexer->position] != '\n'){
                lexer->position++;
            }
            continue;
        }

        if(isalpha((unsigned char)current)){
            size_t start = lexer->position;
            while(isalnum((unsigned char)lexer->source[lexer->position])){
                lexer->position++;
            }

            size_t length = lexer->position - start;
            char *identifier = malloc(length + 1);
            if(!identifier){
                printf("Brak pamieci na identyfikator\n");
                exit(1);
            }
            size_t j = 0;
            for(size_t i = start; i < lexer->position; i++) {
                identifier[j] = lexer->source[i];
                j++;
            }
            identifier[j] = '\0';

            if(strcmp(identifier, "wyswietl") == 0){
                free(identifier);
                return create_token(TOKEN_PRINT, "wyswietl");
            }

            if(strcmp(identifier, "podaj") == 0){
                free(identifier);
                return create_token(TOKEN_INPUT, "podaj");
            }

            if(strcmp(identifier, "jezeli") == 0){
                free(identifier);
                return create_token(TOKEN_IF, "jezeli");
            }

            Token *token = create_token(TOKEN_IDENTIFIER, identifier);
            free(identifier);
            return token;
        }
        
        if(isdigit((unsigned char)current)){
            size_t start = lexer->position;

            while(isdigit((unsigned char)lexer->source[lexer->position])){
                lexer->position++;
            }

            size_t length = lexer->position - start;
            char *number = malloc(length + 1);
            if(!number){
                printf("Brak pamieci na liczbe\n");
                exit(1);
            }
            size_t j = 0;
            for(size_t i = start; i < lexer->position; i++) {
                number[j] = lexer->source[i];
                j++;
            }
            number[j] = '\0';

            Token *token = create_token(TOKEN_NUMBER, number);
            free(number);
            return token;
        }

        switch(current){
            case '+': lexer->position++; return create_token(TOKEN_PLUS, "+");
            case '-': lexer->position++; return create_token(TOKEN_MINUS, "-");
            case '*': lexer->position++; return create_token(TOKEN_STAR, "*");
            case '/': lexer->position++; return create_token(TOKEN_SLASH, "/");
            case '(': lexer->position++; return create_token(TOKEN_LPAREN, "(");
            case ')': lexer->position++; return create_token(TOKEN_RPAREN, ")");
            case '{': lexer->position++; return create_token(TOKEN_LBRACE, "{");
            case '}': lexer->position++; return create_token(TOKEN_RBRACE, "}");
            case ';': lexer->position++; return create_token(TOKEN_SEMICOLON, ";");
            case '=': lexer->position++; return create_token(TOKEN_ASSIGN, "=");
        }

        printf("Nieznany znak: %c\n", current);
        exit(1);
    }
    return create_token(TOKEN_END, "");
}

Token *tokenize(Lexer *lexer) {
    size_t capacity = 128;
    Token *tokens = malloc(capacity * sizeof(Token));
    if(!tokens){
        printf("Brak pamieci na tablice tokenow\n");
        exit(1);
    }
    size_t token_count = 0;

    Token *token;

    while ((token = lexer_next_token(lexer))->type != TOKEN_END) {
        // Reserve space for the end-of-input token.
        if (token_count == capacity - 1) {
            if(capacity > SIZE_MAX / 2 / sizeof(Token)){
                printf("Zbyt wiele tokenow\n");
                free_token(token);
                exit(1);
            }
            size_t new_capacity = capacity * 2;
            Token *new_tokens = realloc(tokens, new_capacity * sizeof(Token));
            if(!new_tokens){
                printf("Brak pamieci na tablice tokenow\n");
                free_token(token);
                exit(1);
            }
            tokens = new_tokens;
            capacity = new_capacity;
        }

        tokens[token_count].type = token->type;
        if (token->value) {
            tokens[token_count].value = malloc(strlen(token->value) + 1);
            if(!tokens[token_count].value){
                printf("Brak pamieci na wartosc tokenu\n");
                free_token(token);
                exit(1);
            }
            strcpy(tokens[token_count].value, token->value);
        } else {
            tokens[token_count].value = NULL;
        }
        token_count++;

        free_token(token);
    }

    tokens[token_count].type = TOKEN_END;
    tokens[token_count].value = NULL;
    free_token(token);

    return tokens;
}
