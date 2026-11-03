#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

int main(int argc, char *argv[]){
    // sprawdzamy czy podano nazwe pliku
    if (argc < 2) {
        printf("Musisz podac nazwe pliku!\n");
        printf("Uzycie: %s nazwa_pliku\n", argv[0]);
        return 1;
    }
    
    char *filename = argv[1];
    
    // otwieramy plik
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Nie moge otworzyc pliku %s\n", filename);
        return 1;
    }
    
    // czytamy plik
    char source_code[1000]; // duzy bufor na kod
    int i = 0;
    int c;
    
    while ((c = fgetc(file)) != EOF) {
        if(i >= 999){
            printf("Plik jest zbyt dlugi (maksymalnie 999 bajtow)\n");
            fclose(file);
            return 1;
        }
        if(c == '\0'){
            printf("Niedozwolony znak w pliku\n");
            fclose(file);
            return 1;
        }
        source_code[i] = (char)c;
        i++;
    }
    source_code[i] = '\0';
    if(ferror(file)){
        printf("Blad odczytu pliku %s\n", filename);
        fclose(file);
        return 1;
    }
    
    fclose(file);
    
    printf("Wczytalem plik: %s\n", filename);
    printf("Kod:\n%s\n", source_code);
    
    // robimy lekser
    Lexer *lexer = create_lexer(source_code);
    Token *tokens = tokenize(lexer);

    // robimy parser
    Parser *parser = create_parser(tokens);
    ASTNode *program = parse_program(parser);
    
    // wykonujemy program
    printf("Wykonuje program...\n");
    interpret_program(program);

    // czyscimy pamiec
    free_tokens(tokens);
    free_lexer(lexer);
    free_node(program);
    free_parser(parser);

    return 0;
}
