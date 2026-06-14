#include "aura.h"

// 簡易符號表 (Symbol Table)
static struct { char name[64]; int value; } env[1024];
static int envCount = 0;

void SetVariable(const char* name, int value) {
    for (int i = 0; i < envCount; i++) {
        if (strcmp(env[i].name, name) == 0) {
            env[i].value = value;
            return;
        }
    }
    strcpy(env[envCount].name, name);
    env[envCount].value = value;
    envCount++;
}

int GetVariable(const char* name) {
    for (int i = 0; i < envCount; i++) {
        if (strcmp(env[i].name, name) == 0) {
            return env[i].value;
        }
    }
    printf("Runtime Error: Undefined variable '%s'\n", name);
    exit(1);
}

int EvaluateNode(AstNode* node) {
    if (!node) return 0;
    
    switch (node->type) {
        case NODE_NUM: return node->value;
        case NODE_VAR: return GetVariable(node->name);
        case NODE_BINOP: {
            int left = EvaluateNode(node->left);
            int right = EvaluateNode(node->right);
            if (node->op == TOKEN_PLUS) return left + right;
            if (node->op == TOKEN_MINUS) return left - right;
            if (node->op == TOKEN_MUL) return left * right;
            if (node->op == TOKEN_DIV) return left / right;
            if (node->op == TOKEN_EQ) return left == right;
            if (node->op == TOKEN_LT) return left < right;
            if (node->op == TOKEN_GT) return left > right;
            return 0;
        }
        case NODE_LET:
        case NODE_ASSIGN: {
            int val = EvaluateNode(node->left);
            SetVariable(node->name, val);
            return val;
        }
        case NODE_PRINT:
            printf(">> %d\n", EvaluateNode(node->left));
            return 0;
        case NODE_BLOCK:
            for (int i = 0; i < node->stmtCount; i++) {
                EvaluateNode(node->statements[i]);
            }
            return 0;
        case NODE_IF:
            if (EvaluateNode(node->condition)) {
                EvaluateNode(node->trueBlock);
            } else if (node->falseBlock) {
                EvaluateNode(node->falseBlock);
            }
            return 0;
        case NODE_WHILE:
            while (EvaluateNode(node->condition)) {
                EvaluateNode(node->trueBlock);
            }
            return 0;
        case NODE_PROGRAM:
            for (int i = 0; i < node->stmtCount; i++) {
                EvaluateNode(node->statements[i]);
            }
            return 0;
        default: return 0;
    }
}

void InterpretAst(AstNode* root) {
    EvaluateNode(root);
}
