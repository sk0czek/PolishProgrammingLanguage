#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

// definiowanie typow tokenow, ktore lekser moze rozpoznac
typedef enum {
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH, TOKEN_NUMBER, TOKEN_PRINT, TOKEN_INPUT, TOKEN_IF, TOKEN_ASSIGN, TOKEN_LBRACE, TOKEN_RBRACE, TOKEN_LPAREN, TOKEN_RPAREN , TOKEN_SEMICOLON, TOKEN_END, TOKEN_IDENTIFIER
} TokenType;

// struktra reprezentujaca stan lexera, zawiera zrodlowy ciag znakow oraz aktualna pozycje
typedef struct {
    char *source;
    int position;
} Lexer;

// struktura reprezentujaca tokem, zawiera typ tokenu oraz jego warrtosc
typedef struct {
    TokenType type;
    char *value;
} Token;

// funkcja tworzaca token
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

// funkcja zwalniająca pamiec uzyta przez token
void free_token(Token *token){
    if(token) {
        free(token->value);
        free(token);
    }
}

// funkcja zwalniajaca pamiec dla calej tablicy tokenow
void free_tokens(Token *tokens) {
    int i = 0;
    while(tokens[i].type != TOKEN_END) {
        if(tokens[i].value) {
            free(tokens[i].value);
        }
        i++;
    }
    free(tokens);
}

// funkcja tworzaca nowy lexer
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

//funkcja zwalniajaca pamiec uzywana przez lexer
void free_lexer(Lexer *lexer){
    if(lexer){
        free(lexer->source);
        free(lexer);
    }
}

// funkcja generujaca nastenpny token na podstawie zrodlowego ciagu znakow
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
            int start = lexer->position;
            while(isalnum((unsigned char)lexer->source[lexer->position])){
                lexer->position++;
            }

            int length = lexer->position - start;
            char *identifier = malloc(length + 1);
            if(!identifier){
                printf("Brak pamieci na identyfikator\n");
                exit(1);
            }
            int j = 0;
            for(int i = start; i < lexer->position; i++) {
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

            if(strcmp(identifier, "if") == 0){
                free(identifier);
                return create_token(TOKEN_IF, "if");
            }

            Token *token = create_token(TOKEN_IDENTIFIER, identifier);
            free(identifier);
            return token;
        }
        
        if(isdigit((unsigned char)current)){
            int start = lexer->position;

            while(isdigit((unsigned char)lexer->source[lexer->position])){
                lexer->position++;
            }

            int length = lexer->position - start;
            char *number = malloc(length + 1);
            if(!number){
                printf("Brak pamieci na liczbe\n");
                exit(1);
            }
            int j = 0;
            for(int i = start; i < lexer->position; i++) {
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
    Token tokens[1000]; // duzy bufor na tokeny
    int token_count = 0;

    Token *token;

    while ((token = lexer_next_token(lexer))->type != TOKEN_END) {
        if (token_count >= 999) {
            printf("Zbyt wiele tokenow (maksymalnie 999)\n");
            free_token(token);
            exit(1);
        }

        // Kopiowanie tokenu
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

    // Przechowywanie tokenu TOKEN_END
    tokens[token_count].type = TOKEN_END;
    tokens[token_count].value = NULL;
    token_count++;
    free_token(token);

    // kopiujemy do dynamicznej tablicy
    Token *result = malloc(token_count * sizeof(Token));
    if(!result){
        printf("Brak pamieci na tablice tokenow\n");
        exit(1);
    }
    for(int i = 0; i < token_count; i++) {
        result[i] = tokens[i];
    }

    return result;
}
