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
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

typedef struct {
    char *input;
    int position;
    Token current_token;
} Lexer;

void init_lexer(Lexer *lexer, char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->current_token.type = TOKEN_EOF;
}

char peek_char(Lexer *lexer) {
    return lexer->input[lexer->position];
}

void consume_char(Lexer *lexer) {
    lexer->position++;
}

void skip_whitespace(Lexer *lexer) {
    while (peek_char(lexer) == ' ' || peek_char(lexer) == '\t') {
        consume_char(lexer);
    }
}

void read_word(Lexer *lexer) {
    int i = 0;
    while ((peek_char(lexer) >= 'a' && peek_char(lexer) <= 'z') ||
           (peek_char(lexer) >= 'A' && peek_char(lexer) <= 'Z') ||
           (peek_char(lexer) >= '0' && peek_char(lexer) <= '9')) {
        lexer->current_token.value[i++] = peek_char(lexer);
        consume_char(lexer);
    }
    lexer->current_token.value[i] = '\0';
    lexer->current_token.type = TOKEN_WORD;
}

void read_single_quote(Lexer *lexer) {
    consume_char(lexer); // Consume the opening single quote
    int i = 0;
    while (peek_char(lexer) != '\'' && peek_char(lexer) != '\0') {
        lexer->current_token.value[i++] = peek_char(lexer);
        consume_char(lexer);
    }
    consume_char(lexer); // Consume the closing single quote
    lexer->current_token.value[i] = '\0';
    lexer->current_token.type = TOKEN_SINGLE_QUOTE;
}

void read_comment(Lexer *lexer) {
    while (peek_char(lexer) != '\n' && peek_char(lexer) != '\0') {
        consume_char(lexer);
    }
    lexer->current_token.type = TOKEN_COMMENT;
}

void next_token(Lexer *lexer) {
    skip_whitespace(lexer);

    if (peek_char(lexer) == '\0') {
        lexer->current_token.type = TOKEN_EOF;
        return;
    }

    switch (peek_char(lexer)) {
        case ';':
            consume_char(lexer);
            lexer->current_token.type = TOKEN_SEMICOLON;
            break;
        case '\n':
            consume_char(lexer);
            lexer->current_token.type = TOKEN_NEWLINE;
            break;
        case '#':
            read_comment(lexer);
            break;
        case '\'':
            read_single_quote(lexer);
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

    return 0;
}
