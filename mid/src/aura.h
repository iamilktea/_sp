#ifndef AURA_H
#define AURA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- 詞法分析 (Lexer) ---
typedef enum {
    TOKEN_NUM, TOKEN_ID, TOKEN_LET, TOKEN_PRINT, 
    TOKEN_IF, TOKEN_ELSE, TOKEN_WHILE,
    TOKEN_ASSIGN, TOKEN_PLUS, TOKEN_MINUS, TOKEN_MUL, TOKEN_DIV,
    TOKEN_EQ, TOKEN_LT, TOKEN_GT, 
    TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_LBRACE, TOKEN_RBRACE, TOKEN_SEMI,
    TOKEN_EOF, TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[64];
    int value;
} Token;

// --- 抽象語法樹 (AST) ---
typedef enum {
    NODE_PROGRAM, NODE_BLOCK, NODE_NUM, NODE_VAR, NODE_LET, 
    NODE_ASSIGN, NODE_PRINT, NODE_BINOP, NODE_IF, NODE_WHILE
} NodeType;

typedef struct AstNode {
    NodeType type;
    int value;
    char name[64];
    TokenType op;
    struct AstNode* left;
    struct AstNode* right;
    struct AstNode* condition;
    struct AstNode* trueBlock;
    struct AstNode* falseBlock;
    struct AstNode** statements; 
    int stmtCount;
} AstNode;

// --- 函式宣告 ---
// Lexer
void TokenizeSource(const char* source);
Token GetNextToken();
Token PeekToken();

// Parser
AstNode* ParseProgram();
AstNode* ParseBlock();
AstNode* ParseStatement();
AstNode* ParseExpression();

// Interpreter
void InterpretAst(AstNode* root);
int EvaluateNode(AstNode* node);

#endif
