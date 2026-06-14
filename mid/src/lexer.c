#include "aura.h"

static Token tokens[2048];
static int tokenCount = 0;
static int currentIndex = 0;

void AddToken(TokenType type, const char* lexeme) {
    tokens[tokenCount].type = type;
    strncpy(tokens[tokenCount].lexeme, lexeme, 63);
    if (type == TOKEN_NUM) {
        tokens[tokenCount].value = atoi(lexeme);
    }
    tokenCount++;
}

void TokenizeSource(const char* source) {
    int i = 0;
    while (source[i] != '\0') {
        if (isspace(source[i])) { i++; continue; }
        
        // 略過註解
        if (source[i] == '/' && source[i+1] == '/') {
            while (source[i] != '\n' && source[i] != '\0') i++;
            continue;
        }

        if (isdigit(source[i])) {
            char buffer[64] = {0};
            int j = 0;
            while (isdigit(source[i])) buffer[j++] = source[i++];
            AddToken(TOKEN_NUM, buffer);
            continue;
        }

        if (isalpha(source[i])) {
            char buffer[64] = {0};
            int j = 0;
            while (isalnum(source[i]) || source[i] == '_') buffer[j++] = source[i++];
            
            if (strcmp(buffer, "let") == 0) AddToken(TOKEN_LET, buffer);
            else if (strcmp(buffer, "print") == 0) AddToken(TOKEN_PRINT, buffer);
            else if (strcmp(buffer, "if") == 0) AddToken(TOKEN_IF, buffer);
            else if (strcmp(buffer, "else") == 0) AddToken(TOKEN_ELSE, buffer);
            else if (strcmp(buffer, "while") == 0) AddToken(TOKEN_WHILE, buffer);
            else AddToken(TOKEN_ID, buffer);
            continue;
        }

        char c = source[i];
        char next = source[i+1];
        if (c == '=' && next == '=') { AddToken(TOKEN_EQ, "=="); i += 2; continue; }
        if (c == '=') { AddToken(TOKEN_ASSIGN, "="); i++; continue; }
        if (c == '+') { AddToken(TOKEN_PLUS, "+"); i++; continue; }
        if (c == '-') { AddToken(TOKEN_MINUS, "-"); i++; continue; }
        if (c == '*') { AddToken(TOKEN_MUL, "*"); i++; continue; }
        if (c == '/') { AddToken(TOKEN_DIV, "/"); i++; continue; }
        if (c == '<') { AddToken(TOKEN_LT, "<"); i++; continue; }
        if (c == '>') { AddToken(TOKEN_GT, ">"); i++; continue; }
        if (c == '(') { AddToken(TOKEN_LPAREN, "("); i++; continue; }
        if (c == ')') { AddToken(TOKEN_RPAREN, ")"); i++; continue; }
        if (c == '{') { AddToken(TOKEN_LBRACE, "{"); i++; continue; }
        if (c == '}') { AddToken(TOKEN_RBRACE, "}"); i++; continue; }
        if (c == ';') { AddToken(TOKEN_SEMI, ";"); i++; continue; }

        printf("Lexer Error: Unknown character '%c'\n", c);
        exit(1);
    }
    AddToken(TOKEN_EOF, "EOF");
}

Token GetNextToken() {
    if (currentIndex < tokenCount) return tokens[currentIndex++];
    return tokens[tokenCount - 1];
}

Token PeekToken() {
    if (currentIndex < tokenCount) return tokens[currentIndex];
    return tokens[tokenCount - 1];
}
