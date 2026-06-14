# 課程：系統程式 -- 筆記、習題與報告

欄位 | 內容
-----|--------
學期 | 114 學年下學期
學生 |  陳建瑋
學號末兩碼 | 58
教師 | [陳鍾誠](https://www.nqu.edu.tw/educsie/index.php?act=blog&code=list&ids=4)
學校科系 | [金門大學資訊工程系](https://www.nqu.edu.tw/educsie/index.php)
課程教材 | https://github.com/ccc114b/cpu2os


🤖 AI 輔助開發聲明 (AI Assistance Declaration)
本專案及過往各次平時作業的開發過程中，有使用 AI 輔助開發與學習。

使用工具：Google Gemini (網頁版對話)、Antigravity 

使用方式：

協助理解底層系統概念（如 Call Stack、Mutex、Race Condition）。

提供 C 語言語法除錯（Debug）與架構重構（Refactoring）建議。

輔助生成基礎程式碼骨架（如 Lexer, Parser 等），並由本人進行後續的邏輯修改與驗證。

AI 對話紀錄：

[Gemini](https://gemini.google.com/share/3c189915f769)


## 📚 平時作業匯總與學習說明

本學期的學習主軸涵蓋了「編譯器前端與後端原理」、「作業系統並行控制」以及「Linux 底層系統呼叫」。以下為歷次作業的實作總整與概念說明：

### 🎯 習題 1：擴充 `while` 語法與底層函數呼叫機制
* **作業連結**：[hw1](https://github.com/iamilktea/_sp/tree/master/homework1)
* **核心概念**：
  探討編譯器前端如何解析控制流，以及程式在記憶體中如何進行函數切換與變數管理。
* **實作亮點**：
  1. 釐清 `while` 迴圈在底層其實是由「條件跳轉 (Conditional Jump)」與「無條件跳轉」搭配虛擬標籤所構成。
  2. 深入剖析呼叫堆疊 (Call Stack) 與活動紀錄 (Activation Record) 的生命週期，理解 Base Pointer (BP) 與 Stack Pointer (SP) 是如何精準保存區域變數與返回位址。

### 🎯 習題 2：自定義程式語言 (Aura) 與直譯器實作
* **作業連結**：[hw2](https://github.com/iamilktea/_sp/tree/master/homework2)
* **核心概念**：
  從無到有設計一套微型程式語言，並獨立完成其前端管線與執行引擎。
* **實作亮點**：
  1. 定義嚴謹的 EBNF 語法，支援變數宣告 (`let`)、`if/else` 條件判斷與 `while` 迴圈。
  2. 手刻詞法分析器 (Lexer) 將字串轉為 Token，並利用遞迴下降演算法 (Recursive Descent) 成功建構出抽象語法樹 (AST)。
  3. 捨棄複雜的目的碼生成，採用直接樹狀走訪直譯 (Tree-Walk Interpretation)，透過維護全域符號表 (Symbol Table) 來執行與儲存變數。

### 🎯 習題 3：AI 輔助底層專案開發
* **作業連結**：[hw3](https://github.com/iamilktea/_sp/tree/master/homework3)
* **核心概念**：
  利用 AI IDE 或命令列 AI 工具來構建與重構系統程式。
* **實作亮點**：
  我選擇將原先的 Aura 直譯器專案，透過 AI 的輔助，**徹底改寫為標準的 C99 底層版本**。並嚴格遵守系統開發的命名規範與指標操作，深刻體會到 AI 作為「結對編程 (Pair Programming)」夥伴在加速底層開發與除錯上的強大產能。

### 🎯 習題 4：現代系統程式設計專書撰寫
* **作業連結**：[hw4](https://github.com/iamilktea/_sp/tree/master/homework4)
* **核心概念**：
  採用架構驅動 (Architecture-Driven) 的方式，有系統地梳理與輸出系統程式知識。
* **實作亮點**：
  探討了虛擬機器設計中的「堆疊機 (Stack VM)」與「暫存器機 (Register VM)」架構差異，並深入研究了在設計執行環境時，記憶體邊界檢查 (Bounds Checking) 對於防止緩衝區溢位等安全漏洞的必要性。

### 🎯 習題 5：作業系統並行控制 (Concurrency)
* **作業連結**：[hw5](https://github.com/iamilktea/_sp/tree/master/homework5)
* **核心概念**：
  處理多執行緒 (Multi-threading) 下的共享資源競爭 (Race Condition) 與死鎖 (Deadlock) 問題。
* **實作亮點**：
  1. **銀行存提款**：實作 POSIX `pthread_mutex_t` 來保護臨界區，確保 10 萬次並行存提款後餘額正確歸零。
  2. **生產者消費者**：利用互斥鎖與條件變數 (`pthread_cond_t`)，解決了忙碌等待 (Busy Waiting) 造成的效能浪費。
  3. **哲學家用餐**：透過實作「資源分級定序策略」，規定獲取鎖的嚴格順序，破除循環等待條件，完美防範死鎖發生。

### 🎯 習題 6：Linux 行程與檔案系統 API (期中專案：AuraShell)
* **作業連結**：[hw6](https://github.com/iamilktea/_sp/tree/master/homework6)
* **核心概念**：
  理解 UNIX「萬物皆檔案」的哲學，實作自己的命令列直譯器。
* **實作亮點**：
  1. 利用 `fork()` 與 `execvp()` 實現「細胞分裂與大腦移植」，啟動外部指令。
  2. 利用 `open()` 與 `dup2()` 實作標準輸出重導向 (`>`)，篡改檔案描述子 (File Descriptor) 表的指向。
  3. 利用非阻塞的 `waitpid` 清理背景執行 (`&`) 的子行程，有效防止殭屍行程 (Zombie Process) 癱瘓系統資源。

---

## 💡 學期總結心得
這學期的系統程式課程，讓我從「只會呼叫高階 API 的應用程式開發者」，蛻變成「能透徹理解底層記憶體與行程運作的系統工程師」。

從手刻 AST 樹、與 Segmentation Fault 奮戰，到設計出安全的執行緒架構，再到看見自己寫的 Shell 成功重導向輸出，這個從無到有的過程極具挑戰性但也充滿成就感。將這些抽象的理論化為實際可運作的 C 語言程式碼，不僅破除了我對底層系統的恐懼，也為我未來在進行伺服器架構設計、資安防護以及高效能開發上，打下了極為堅實的基礎。
