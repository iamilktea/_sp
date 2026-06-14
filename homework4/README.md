

## 📘 書籍企劃：現代系統程式設計：從底層架構到安全實踐

**(Modern System Programming: From Low-Level Architecture to Secure Practices)**

**本書受眾**：具備基礎 C/Python 程式設計能力，希望透徹理解「程式碼如何轉化為機器行為」，並渴望掌握伺服器底層運作與系統安全防護的開發者。

### 📑 目錄大綱 (Table of Contents)

* **第一篇：矽晶與邏輯的橋樑**
* 第 1 章：C 語言的底層透視（指標、記憶體佈局與對齊）
* 第 2 章：處理器架構與組合語言基礎


* **第二篇：語言的翻譯官（編譯器與直譯器）**
* 第 3 章：詞法與語法分析：建構抽象語法樹 (AST)
* 第 4 章：控制流的奧秘：從 `while` 迴圈到條件跳轉


* **第三篇：執行環境與虛擬機器**
* 第 5 章：活動紀錄與呼叫堆疊 (Call Stack) 的生與死
* 第 6 章：虛擬機器設計：堆疊機 (Stack VM) vs. 暫存器機 (Register VM)


* **第四篇：作業系統與安全防護**
* 第 7 章：行程 (Process)、執行緒 (Thread) 與記憶體分頁
* 第 8 章：基礎設施保護：降低攻擊面與 Ubuntu 伺服器強化 (Server Hardening)
* 第 9 章：防禦緩衝區溢位 (Buffer Overflow) 與現代記憶體安全機制



---

## 📖 核心示範章節：第 6 章 虛擬機器設計：堆疊機 vs. 暫存器機

在我們理解了編譯器如何將高階語言轉換為中介碼（Intermediate Representation, IR）後，下一個面臨的問題是：**這些指令該由誰來執行？**

現代軟體工程中，虛擬機器（Virtual Machine, VM）扮演著舉足輕重的角色。從 Java 的 JVM、Python 的 PVM，到我們自行設計的語言直譯器，VM 提供了一個獨立於硬體架構的執行環境。在架構設計上，VM 主要分為兩大流派：**堆疊機（Stack-based VM）**與**暫存器機（Register-based VM）**。

### 6.1 堆疊機 (Stack-based VM)：簡潔與優雅的代名詞

堆疊機是最直觀且易於實作的虛擬機器架構。其核心特徵是**指令通常不帶有運算元（Zero-operand instructions）**，所有的運算皆依賴一個全域的「運算堆疊（Operand Stack）」。

#### 運作機制與指令集

在堆疊機中，計算 `A = B + C` 的底層位元組碼（Bytecode）會被展開成如下形式：

```text
LOAD  B    // 將 B 的值推入堆疊頂端
LOAD  C    // 將 C 的值推入堆疊頂端
ADD        // 彈出頂端兩個值，相加後，將結果推回堆疊
STORE A    // 彈出頂端的值，存入變數 A

```

#### 實作考量與 C 語言抽象

實作堆疊機的 C 語言結構非常簡練。我們僅需要一個陣列作為堆疊，以及一個指向堆疊頂端的指標（Stack Pointer, SP）。

```c
typedef struct {
    int stack[MAX_STACK_SIZE];
    int sp;
    int pc; // 程式計數器 (Program Counter)
} StackVM;

void ExecuteStackVM(StackVM* vm, Instruction* bytecode) {
    while (1) {
        Instruction instr = bytecode[vm->pc++];
        switch (instr.opcode) {
            case OP_PUSH:
                vm->stack[++vm->sp] = instr.operand;
                break;
            case OP_ADD: {
                int right = vm->stack[vm->sp--];
                int left = vm->stack[vm->sp--];
                vm->stack[++vm->sp] = left + right;
                break;
            }
            // ... 其他指令
        }
    }
}

```

**優點**：編譯器生成目的碼的演算法非常簡單（只需對 AST 進行後序走訪 Post-order Traversal）。生成的 Bytecode 體積極小。
**缺點**：執行相同的邏輯，堆疊機需要執行更多的指令（頻繁的 Push 與 Pop），導致指令派發（Instruction Dispatch）的成本增加。

### 6.2 暫存器機 (Register-based VM)：效能與硬體的倒影

為了克服堆疊機指令冗長的問題，暫存器機應運而生。Lua 的虛擬機以及 Android 的 Dalvik/ART 虛擬機皆採用此架構。它模仿了真實硬體 CPU 的設計，指令直接指定參與運算的暫存器。

#### 運作機制與指令集

在暫存器機中，同樣的 `A = B + C` 計算，指令會大幅縮減：

```text
ADD R0, R1, R2   // 將暫存器 R1 與 R2 的值相加，結果存入 R0

```

這裡的 `R0`, `R1`, `R2` 通常是映射到虛擬機記憶體空間中的某個索引位置，或者是活動紀錄（Activation Record）中的區域變數陣列。

**優點**：大幅減少了指令數量。由於不需要頻繁操作全域堆疊，記憶體存取次數降低，執行速度通常比堆疊機快上許多。
**缺點**：每個指令的長度變大（需要儲存運算元的位置資訊）。編譯器後端（Backend）的實作難度激增，需要撰寫複雜的「暫存器配置（Register Allocation）」演算法，決定何時將變數溢出（Spill）到記憶體中。

### 6.3 架構抉擇與安全邊界 (Security Boundaries)

在設計自家的虛擬機時，不僅要考量效能，系統安全（System Security）也是現代架構的關鍵。

無論是堆疊機或暫存器機，都必須在執行迴圈（Dispatch Loop）中嚴格實作**邊界檢查（Bounds Checking）**。在上述的 C 語言範例中，如果惡意的 Bytecode 故意執行過多的 `OP_ADD` 而不進行 `OP_PUSH`，`vm->sp` 將會低於 0，導致**堆疊下溢（Stack Underflow）**；反之則會造成**堆疊溢位（Stack Overflow）**。這類漏洞在伺服器端直譯器中，往往是攻擊者越權讀寫記憶體空間的致命突破口。

---

## 🤖 AI 協作寫書指南 (How to use AI to complete this book)

如果你想把這本書完整寫出來，請不要丟一句「幫我寫完」給 AI，那會產出空泛的廢話。請採用「遞增式生成（Incremental Generation）」的策略：

1. **設定 System Prompt（全域設定）**：
開啟一個新的 AI 對話框（如 Claude 或 ChatGPT），輸入：「*你現在是一位擁有 20 年經驗的系統程式設計與資安架構專家。我們正在撰寫一本名為《現代系統程式設計》的書。請使用繁體中文，語氣保持專業、嚴謹且富含技術深度。*」
2. **單節推進（Section by Section）**：
一次只要求寫一小節。例如：「*請根據大綱，為我撰寫『第 8 章 8.2 節：降低攻擊面與 Ubuntu 伺服器強化實務』。請包含具體的 SSH 金鑰設定與 Apache2 權限隔離組態範例。*」
3. **技術審查與校調**：
AI 寫完後，你需要扮演主編。如果程式碼沒有遵守大駝峰命名法，或是架構說明不夠清晰，你要立刻糾正它：「*這段指令碼寫得不錯，但請把 C 語言範例中的函式命名改為 UpperCamelCase，並補充說明這段程式碼在記憶體中的時間複雜度。*」

---
