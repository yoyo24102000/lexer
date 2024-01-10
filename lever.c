#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKEN_LEN 256

typedef enum {
    TOKEN_WORD,
    TOKEN_SEMICOLON,
    TOKEN_NEWLINE,
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELIF,
    TOKEN_ELSE,
    TOKEN_FI,
    TOKEN_SINGLE_QUOTE,
    TOKEN_COMMENT,
    TOKEN_EOF
} TokenType;

typedef struct {
    char *input;
    char *saveptr; // Used by strtok_r to maintain state
    Token current_token;
} Lexer;

void init_lexer(Lexer *lexer, char *input) {
    lexer->input = strdup(input); // Make a copy of the input string
    lexer->saveptr = NULL;
    lexer->current_token.type = TOKEN_EOF;
}

void cleanup_lexer(Lexer *lexer) {
    free(lexer->input); // Free the copied input string
}

void read_word(Lexer *lexer) {
    char *token = strtok_r(NULL, " \t\n", &(lexer->saveptr));
    if (token != NULL) {
        strncpy(lexer->current_token.value, token, MAX_TOKEN_LEN - 1);
        lexer->current_token.value[MAX_TOKEN_LEN - 1] = '\0';
        lexer->current_token.type = TOKEN_WORD;
    } else {
        lexer->current_token.type = TOKEN_EOF;
    }
}

void next_token(Lexer *lexer) {
    if (lexer->saveptr == NULL) {
        // First call, initialize strtok_r with the input string
        lexer->saveptr = lexer->input;
    }

    switch (lexer->input[0]) {
        case '\0':
            lexer->current_token.type = TOKEN_EOF;
            break;
        case ';':
            consume_char(lexer);
            lexer->current_token.type = TOKEN_SEMICOLON;
            break;
        case '\n':
            consume_char(lexer);
            lexer->current_token.type = TOKEN_NEWLINE;
            break;
        default:
            read_word(lexer);
    }
}

void print_token(Token token) {
    printf("Type: %d, Value: %s\n", token.type, token.value);
}

int main() {
    char input[] = "if echo 'Hello world'; then # This is a comment\n echo Goodbye; fi";
    Lexer lexer;
    init_lexer(&lexer, input);

    do {
        next_token(&lexer);
        print_token(lexer.current_token);
    } while (lexer.current_token.type != TOKEN_EOF);

    cleanup_lexer(&lexer);

    return 0;
}
