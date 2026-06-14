# Aura Compiler & Virtual Machine

Aura 是一個專為學習「編譯器原理」與「系統程式」而設計的極簡程式語言。本專案包含了一個從頭打造的編譯器管線（Compiler Pipeline）以及一個基於堆疊的虛擬機（Stack-based Virtual Machine）。

本專案將 Aura 原始碼轉換為抽象語法樹 (AST)，編譯成自定義的位元組碼 (Bytecode)，最後交由虛擬機直接執行。

## ✨ 核心特色 (Features)

* **完整的編譯管線**：包含詞法分析 (Lexer)、語法分析 (Parser)、抽象語法樹建構 (AST) 與程式碼生成 (Code Generator)。
* **靜態強型態 (MVP 階段)**：目前專注於整數 (`int`) 運算與布林邏輯。
* **控制流支援**：完整支援 `if/else` 條件分支與 `while` 迴圈結構。
* **變數與作用域**：支援變數宣告 (`let`) 與重新賦值。
* **堆疊虛擬機 (Stack VM)**：內建具備程式計數器 (PC) 的虛擬機，負責解析並執行編譯後的指令集。

## 📂 專案架構 (Project Structure)

\`\`\`text
_cm/
├── src/               # 編譯器與虛擬機原始碼
│   ├── lexer.py       # 詞法分析器：將字串轉換為 Token
│   ├── parser.py      # 語法分析器：將 Token 轉換為 AST 節點
│   ├── compiler.py    # 程式碼生成：走訪 AST 並產出 Bytecode
│   ├── vm.py          # 堆疊虛擬機：執行 Bytecode
│   └── main.py        # 系統進入點
├── examples/          # Aura 語言的測試範例程式
│   ├── factorial.au   # 階乘計算範例
│   └── simple_if.au   # 條件判斷範例
└── README.md          # 專案說明文件
\`\`\`

## 🚀 快速開始 (Quick Start)

### 環境要求
* Python 3.8 或以上版本
* 無需安裝任何第三方套件 (Zero dependencies)

### 執行程式
你可以透過 `src/main.py` 來編譯並執行 Aura 程式碼：

\`\`\`bash
# 執行範例檔案
python src/main.py examples/factorial.au
\`\`\`

## 📝 語法範例 (Syntax Example)

以下是一個計算階乘的 Aura 程式範例 (`factorial.au`)：

\`\`\`typescript
let n = 5;
let result = 1;

// 計算 5 的階乘
while (n > 0) {
    result = result * n;
    n = n - 1;
}

if (result == 120) {
    print(result);  // 預期輸出: 120
} else {
    print(0);
}
\`\`\`

## 🛠️ 未來展望 (Roadmap)
- [ ] 支援函數定義與呼叫 (Call Stack 實作)
- [ ] 加入字串 (String) 型別支援
- [ ] 實作更完善的語法錯誤提示 (Error Handling)
