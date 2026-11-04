#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

int main(int argc, char *argv[]){
    if (argc < 2) {
        printf("Musisz podac nazwe pliku!\n");
        printf("Uzycie: %s nazwa_pliku\n", argv[0]);
        return 1;
    }
    
    char *filename = argv[1];
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Nie moge otworzyc pliku %s\n", filename);
        return 1;
    }
    
    size_t capacity = 1024;
    char *source_code = malloc(capacity);
    if(!source_code){
        printf("Brak pamieci na kod zrodlowy\n");
        fclose(file);
        return 1;
    }
    size_t i = 0;
    int c;
    
    while ((c = fgetc(file)) != EOF) {
        if(c == '\0'){
            printf("Niedozwolony znak w pliku\n");
            free(source_code);
            fclose(file);
            return 1;
        }
        // Reserve space for the terminating null byte.
        if(i == capacity - 1){
            if(capacity > SIZE_MAX / 2){
                printf("Plik jest zbyt duzy\n");
                free(source_code);
                fclose(file);
                return 1;
            }
            size_t new_capacity = capacity * 2;
            char *new_source_code = realloc(source_code, new_capacity);
            if(!new_source_code){
                printf("Brak pamieci na kod zrodlowy\n");
                free(source_code);
                fclose(file);
                return 1;
            }
            source_code = new_source_code;
            capacity = new_capacity;
        }
        source_code[i] = (char)c;
        i++;
    }
    source_code[i] = '\0';
    if(ferror(file)){
        printf("Blad odczytu pliku %s\n", filename);
        free(source_code);
        fclose(file);
        return 1;
    }
    
    fclose(file);
    
    printf("Wczytalem plik: %s\n", filename);
    printf("Kod:\n%s\n", source_code);
    
    Lexer *lexer = create_lexer(source_code);
    free(source_code);
    Token *tokens = tokenize(lexer);

    Parser *parser = create_parser(tokens);
    ASTNode *program = parse_program(parser);
    
    printf("Wykonuje program...\n");
    interpret_program(program);

    free_tokens(tokens);
    free_lexer(lexer);
    free_node(program);
    free_parser(parser);

    return 0;
}
