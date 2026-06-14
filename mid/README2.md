# Aura Language Interpreter

Aura 是一個以純 C 語言 (C99) 從零打造的程式語言直譯器。
實作包含了詞法分析器、語法分析器與樹狀走訪直譯器。

## 🚀 快速開始

### 1. 編譯專案
\`\`\`bash
make
\`\`\`

### 2. 執行範例程式
\`\`\`bash
./aura tests/test_scripts/math_test.au
\`\`\`

## 📂 專案架構
- `src/`: 核心原始碼
- `docs/`: EBNF 語法與架構文件
- `tests/`: 單元測試與 `.au` 測試腳本
- `scripts/`: 自動化建置腳本
