#ifndef AURA_H
#define AURA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- 詞法分析 (Lexer) ---
typedef enum {
    TOKEN_NUM, TOKEN_ID, TOKEN_LET, TOKEN_PRINT, 
    TOKEN_ASSIGN, TOKEN_PLUS, TOKEN_MINUS, TOKEN_SEMI, TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[64];
    int value;
} Token;

// --- 抽象語法樹 (AST) ---
typedef enum {
    NODE_NUM, NODE_VAR, NODE_LET, NODE_ASSIGN, NODE_PRINT, NODE_BINOP
} NodeType;

typedef struct AstNode {
    NodeType type;
    int value;
    char name[64];
    char op;
    struct AstNode* left;
    struct AstNode* right;
} AstNode;

// --- 函式宣告 ---
// 檔案處理
char* ReadSourceFile(const char* filepath);

// 詞法分析
void TokenizeSource(const char* source);
Token GetNextToken();

// 語法分析
AstNode* ParseProgram();
AstNode* ParseStatement();
AstNode* ParseExpression();

// 直譯執行
void InterpretAst(AstNode* root);
int EvaluateNode(AstNode* node);

#endif
