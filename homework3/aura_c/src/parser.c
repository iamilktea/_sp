#include "aura.h"

AstNode* CreateNode(NodeType type) {
    AstNode* node = (AstNode*)malloc(sizeof(AstNode));
    memset(node, 0, sizeof(AstNode));
    node->type = type;
    return node;
}

void MatchToken(TokenType expected) {
    Token t = GetNextToken();
    if (t.type != expected) {
        printf("Parser Error: Unexpected token '%s'\n", t.lexeme);
        exit(1);
    }
}

AstNode* ParseFactor() {
    Token t = PeekToken();
    if (t.type == TOKEN_NUM) {
        GetNextToken();
        AstNode* node = CreateNode(NODE_NUM);
        node->value = t.value;
        return node;
    } else if (t.type == TOKEN_ID) {
        GetNextToken();
        AstNode* node = CreateNode(NODE_VAR);
        strcpy(node->name, t.lexeme);
        return node;
    } else if (t.type == TOKEN_LPAREN) {
        MatchToken(TOKEN_LPAREN);
        AstNode* node = ParseExpression();
        MatchToken(TOKEN_RPAREN);
        return node;
    }
    printf("Parser Error in Factor\n");
    exit(1);
}

AstNode* ParseTerm() {
    AstNode* left = ParseFactor();
    while (PeekToken().type == TOKEN_MUL || PeekToken().type == TOKEN_DIV) {
        Token op = GetNextToken();
        AstNode* node = CreateNode(NODE_BINOP);
        node->left = left;
        node->op = op.type;
        node->right = ParseFactor();
        left = node;
    }
    return left;
}

AstNode* ParseExpression() {
    AstNode* left = ParseTerm();
    while (PeekToken().type == TOKEN_PLUS || PeekToken().type == TOKEN_MINUS || 
           PeekToken().type == TOKEN_EQ || PeekToken().type == TOKEN_LT || PeekToken().type == TOKEN_GT) {
        Token op = GetNextToken();
        AstNode* node = CreateNode(NODE_BINOP);
        node->left = left;
        node->op = op.type;
        node->right = ParseTerm();
        left = node;
    }
    return left;
}

AstNode* ParseBlock() {
    MatchToken(TOKEN_LBRACE);
    AstNode* node = CreateNode(NODE_BLOCK);
    node->statements = malloc(sizeof(AstNode*) * 100);
    while (PeekToken().type != TOKEN_RBRACE && PeekToken().type != TOKEN_EOF) {
        node->statements[node->stmtCount++] = ParseStatement();
    }
    MatchToken(TOKEN_RBRACE);
    return node;
}

AstNode* ParseStatement() {
    Token t = PeekToken();
    if (t.type == TOKEN_LET) {
        MatchToken(TOKEN_LET);
        AstNode* node = CreateNode(NODE_LET);
        Token id = GetNextToken();
        strcpy(node->name, id.lexeme);
        MatchToken(TOKEN_ASSIGN);
        node->left = ParseExpression();
        MatchToken(TOKEN_SEMI);
        return node;
    } else if (t.type == TOKEN_PRINT) {
        MatchToken(TOKEN_PRINT);
        AstNode* node = CreateNode(NODE_PRINT);
        MatchToken(TOKEN_LPAREN);
        node->left = ParseExpression();
        MatchToken(TOKEN_RPAREN);
        MatchToken(TOKEN_SEMI);
        return node;
    } else if (t.type == TOKEN_IF) {
        MatchToken(TOKEN_IF);
        AstNode* node = CreateNode(NODE_IF);
        MatchToken(TOKEN_LPAREN);
        node->condition = ParseExpression();
        MatchToken(TOKEN_RPAREN);
        node->trueBlock = ParseBlock();
        if (PeekToken().type == TOKEN_ELSE) {
            MatchToken(TOKEN_ELSE);
            node->falseBlock = ParseBlock();
        }
        return node;
    } else if (t.type == TOKEN_WHILE) {
        MatchToken(TOKEN_WHILE);
        AstNode* node = CreateNode(NODE_WHILE);
        MatchToken(TOKEN_LPAREN);
        node->condition = ParseExpression();
        MatchToken(TOKEN_RPAREN);
        node->trueBlock = ParseBlock();
        return node;
    } else if (t.type == TOKEN_ID) {
        AstNode* node = CreateNode(NODE_ASSIGN);
        Token id = GetNextToken();
        strcpy(node->name, id.lexeme);
        MatchToken(TOKEN_ASSIGN);
        node->left = ParseExpression();
        MatchToken(TOKEN_SEMI);
        return node;
    }
    printf("Parser Error: Unknown Statement\n");
    exit(1);
}

AstNode* ParseProgram() {
    AstNode* node = CreateNode(NODE_PROGRAM);
    node->statements = malloc(sizeof(AstNode*) * 1000);
    while (PeekToken().type != TOKEN_EOF) {
        node->statements[node->stmtCount++] = ParseStatement();
    }
    return node;
}
