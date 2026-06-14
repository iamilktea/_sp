#include <assert.h>
#include "../src/aura.h"

int main() {
    printf("[單元測試] 開始驗證 Parser 語法分析器...\n");

    // 1. 定義一段測試 Aura 原始碼
    const char* testSource = "let a = 5; print(a);";

    // 2. 執行詞法分析將 Token 置入緩衝區
    TokenizeSource(testSource);

    // 3. 呼叫語法分析器建構 AST
    AstNode* root = ParseProgram();

    // 4. 斷言 (Assert) 驗證樹狀結構是否絕對正確
    assert(root != NULL);
    assert(root->type == NODE_PROGRAM);
    assert(root->stmtCount == 2); // 預期包含兩個陳述句

    // 5. 驗證第一個陳述句: let a = 5;
    AstNode* letNode = root->statements[0];
    assert(letNode != NULL);
    assert(letNode->type == NODE_LET);
    assert(strcmp(letNode->name, "a") == 0);
    
    // 驗證賦值表達式右側是否為數字 5
    assert(letNode->left != NULL);
    assert(letNode->left->type == NODE_NUM);
    assert(letNode->left->value == 5);

    // 6. 驗證第二個陳述句: print(a);
    AstNode* printNode = root->statements[1];
    assert(printNode != NULL);
    assert(printNode->type == NODE_PRINT);
    
    // 驗證 print 內部的運算式是否為變數 a
    assert(printNode->left != NULL);
    assert(printNode->left->type == NODE_VAR);
    assert(strcmp(printNode->left->name, "a") == 0);

    printf("🎉 [測試通過] Parser 成功將 Token 序列建構為標準 AST 結構！\n");
    return 0;
}
