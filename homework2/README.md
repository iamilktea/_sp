# Aura Language Interpreter

Aura 是一個專為學習「編譯器原理」與「系統程式」而設計的極簡程式語言。本專案從零開始打造了一個完整的**前端管線 (Frontend Pipeline)** 與**樹狀直譯器 (Tree-Walk Interpreter)**。

本系統會將 Aura 原始碼轉換為 Token 序列，建構出抽象語法樹 (AST)，並由直譯器直接走訪執行，無需編譯為目的碼或依賴虛擬機。

##  核心特色 (Features)

* **完整的直譯管線**：包含詞法分析 (Lexer)、語法分析 (Parser) 與直譯器 (Interpreter)。
* **AST 樹狀走訪**：透過遞迴走訪抽象語法樹來執行指令與進行數學運算。
* **強大的控制流**：支援巢狀的 `if / else` 條件判斷與 `while` 迴圈結構。
* **環境變數管理**：具備符號表 (Symbol Table) 機制，支援變數宣告 (`let`) 與重新賦值。
* **輕量化設計**：純 Python 實作，零第三方套件依賴 (Zero dependencies)。

##  專案架構 (Project Structure)

\`\`\`text
_cm/
├── src/
│   ├── lexer.py         # 負責將原始碼字串解析為 Token 陣列
│   ├── parser.py        # 負責將 Token 轉換為抽象語法樹 (AST)
│   ├── interpreter.py   # 負責走訪 AST 並執行對應的運算與邏輯
│   └── main.py          # 系統進入點，串接上述三個模組
├── examples/
│   └── test.au          # 語言測試範例腳本
└── README.md
\`\`\`

##  快速開始 (Quick Start)

### 環境要求
* Python 3.8 或以上版本

### 執行程式
你可以透過 `src/main.py` 來直譯並執行 Aura 程式碼：

\`\`\`bash
# 執行範例檔案
python src/main.py examples/test.au
\`\`\`

##  語法範例 (Syntax Example)

以下是一個計算階乘的 Aura 程式範例：

\`\`\`typescript
let n = 5;
let result = 1;

// 使用 while 迴圈計算 5 的階乘
while (n > 0) {
    result = result * n;
    n = n - 1;
}

// 條件判斷與輸出
if (result == 120) {
    print(result);  // 預期輸出: >> 120
} else {
    print(0);
}
\`\`\`

## 🎯 作業目標對應 (Issue Tracker)
本專案的實作符合系統程式設計之要求，成功構建出可執行的解譯器架構。
* Resolves #5
