
# Aura Language Interpreter (C Edition)

本專案是一個以純 C 語言 (C99) 從零打造的程式語言直譯器。實作包含了詞法分析器 (Lexer)、遞迴下降語法分析器 (Parser)、抽象語法樹 (AST) 建構，以及樹狀走訪直譯器 (Tree-Walk Interpreter)。

##  編譯與執行

使用內建的 Makefile 一鍵編譯：

\`\`\`bash
# 1. 編譯專案
make

# 2. 執行範例程式
./aura examples/test.au
\`\`\`

## 系統架構
所有的函式皆嚴格遵守 C99 標準與 UpperCamelCase 命名規範。
* `src/lexer.c`: 將字串轉換為 Token 陣列。
* `src/parser.c`: 建立與組織 AST 節點。
* `src/interpreter.c`: 維護 Symbol Table 並執行 AST 邏輯。
