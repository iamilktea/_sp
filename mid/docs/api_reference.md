# Aura 直譯器核心 API 參考手冊 (API Reference)

本文檔詳述 Aura 語言核心引擎中所使用的公開函式與介面規格。所有內部實作皆嚴格遵循大駝峰式命名規範。

---

## 1. 詞法分析模組 (Lexer - `src/lexer.c`)

負責將輸入的原始碼字串切割並轉換為具備靜態型態標籤的 Token 序列。

### 核心函式

#### `void TokenizeSource(const char* source)`
* **用途**：掃描完整的原始碼字串，將其解析為 Token 陣列儲存於全域緩衝區中。
* **參數**：
  - `source`: 指向 Aura 原始碼字串的常數指標。
* **內部行為**：自動過濾空白字元與以 `//` 開頭的單行註解。若遇到無法識別的字元，將直接輸出 `Lexer Error` 並中止行程。

#### `Token GetNextToken()`
* **用途**：自全域 Token 緩衝區中提取下一個待處理的 Token，並將內部的讀取索引往前移動。
* **回傳值**：返回一個 `Token` 結構體。若已達檔案結尾，則持續返回 `TOKEN_EOF`。

#### `Token PeekToken()`
* **用途**：預覽目前內部的下一個 Token，但**不移動**讀取索引。
* **回傳值**：返回即將被處理的 `Token` 結構體。常用於語法分析中的超前預讀（Lookahead）。

---

## 2. 語法分析模組 (Parser - `src/parser.c`)

採用遞迴下降分析法（Recursive Descent Parsing），將平面的 Token 序列組織成階層化的抽象語法樹 (AST)。

### 核心函式

#### `AstNode* ParseProgram()`
* **用途**：語法分析的總進入點，負責解析整個檔案的敘述句。
* **回傳值**：返回根節點型態為 `NODE_PROGRAM` 的 `AstNode` 指標，內部包含所有陳述句節點的指標陣列。

#### `AstNode* ParseStatement()`
* **用途**：分支解析單一陳述句（陳述句包含 `let` 宣告、重新賦值、`print` 輸出、`if` 分支與 `while` 迴圈）。
* **回傳值**：對應陳述句型態的 AST 節點指標。

#### `AstNode* ParseExpression()`
* **用途**：依據運算子優先順序（Operator Precedence），解析包含加減、比較（`==`, `<`, `>`）的四則運算式。
* **回傳值**：表達式二元樹的根節點指標。

---

## 3. 核心直譯引擎 (Interpreter - `src/interpreter.c`)

負責動態走訪 AST 節點，維護執行時期的符號表（Symbol Table）。

### 核心函式

#### `void InterpretAst(AstNode* root)`
* **用途**：直譯器的外部進入點。
* **參數**：
  - `root`: 語法分析器生成的 AST 根節點指標。

#### `int EvaluateNode(AstNode* node)`
* **用途**：遞迴走訪並計算特定 AST 節點的值。
* **參數**：
  - `node`: 當前需評估的 AST 節點指標。
* **回傳值**：該節點計算後的整數結果。若是條件判斷式，則返回 `1`（真）或 `0`（偽）。
* **內部行為**：
  - 遇到 `NODE_LET` 或 `NODE_ASSIGN` 時，會將計算結果寫入環境變數陣列（Symbol Table）。
  - 遇到 `NODE_VAR` 時，會自符號表中查找並返回對應數值，若找不到則觸發 `Runtime Error`。
