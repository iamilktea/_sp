
#include <assert.h>
#include "../src/aura.h"

// 簡單的單元測試
int main() {
    const char* code = "let x = 10;";
    TokenizeSource(code);
    
    assert(GetNextToken().type == TOKEN_LET);
    assert(GetNextToken().type == TOKEN_ID);
    assert(GetNextToken().type == TOKEN_ASSIGN);
    assert(GetNextToken().type == TOKEN_NUM);
    assert(GetNextToken().type == TOKEN_SEMI);
    
    printf("Lexer 測試全部通過！\n");
    return 0;
}
