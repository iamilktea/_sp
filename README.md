# 課程：系統程式 -- 筆記、習題與報告

欄位 | 內容
-----|--------
學期 | 114 學年下學期
學生 |  陳建瑋
學號末兩碼 | 58
教師 | [陳鍾誠](https://www.nqu.edu.tw/educsie/index.php?act=blog&code=list&ids=4)
學校科系 | [金門大學資訊工程系](https://www.nqu.edu.tw/educsie/index.php)
課程教材 | https://github.com/ccc114b/cpu2os


# 系統程式設計 (System Programming) 期末學習歷程總整

**學生姓名：** [填入你的姓名]

**學號：** [填入你的學號]

**學期：** 2026 Spring

---

## 🤖 AI 輔助開發聲明 (AI Assistance Declaration)

本專案及過往各次平時作業的開發過程中，**有使用 AI 輔助開發與學習**。

* **使用工具**：Google Gemini (網頁版對話)、[若有使用 Cursor / Antigravity 請在此補充]
* **使用方式**：
1. 協助理解底層系統概念（如 Call Stack、Mutex、Race Condition）。
2. 提供 C 語言語法除錯（Debug）與架構重構（Refactoring）建議。
3. 輔助生成基礎程式碼骨架（如 Lexer, Parser 等），並由本人進行後續的邏輯修改與驗證。


* **AI 對話紀錄/截圖參考**：
* [請在此貼上你與 Gemini 對話的 Share Link，或是附上截圖資料夾連結]
* *附註：相關截圖檔案放置於本儲存庫的 `/ai_logs/` 資料夾中。*



---

## 📚 平時作業匯總與學習說明

本學期的學習主軸涵蓋了「編譯器前端與後端原理」、「作業系統並行控制」以及「Linux 底層系統呼叫」。以下為歷次作業的實作總整與概念說明：

### 🎯 習題 1 & 2：編譯器基礎與自訂義語言 (Aura Interpreter)

* **作業連結**：[填入你的 Aura 專案 GitHub 連結，例如: `./aura_c/`]
* **核心概念**：
這兩次作業探討了程式碼如何從高階語言被翻譯並執行。我實作了一個名為 **Aura** 的微型程式語言直譯器。
* **實作亮點**：
1. **語法擴充 (`while`)**：透過條件跳轉 (Conditional Jump) 標籤控制迴圈流向。
2. **函數呼叫機制 (Call Stack)**：理解了活動紀錄 (Activation Record) 中，Base Pointer (BP) 與 Stack Pointer (SP) 如何保存區域變數與返回位址。
3. **直譯器管線 (Interpreter Pipeline)**：從零手刻詞法分析 (Lexer)、遞迴下降語法分析 (Parser) 到抽象語法樹 (AST) 的走訪計算。



### 🎯 習題 3：AI 輔助底層專案開發

* **作業連結**：[填入相關專案連結或整合說明]
* **核心概念**：
利用 AI IDE 或命令列 AI 工具來構建系統程式。
* **實作亮點**：
我選擇將原先的 Aura 直譯器專案，透過 AI 的輔助，**徹底改寫為標準的 C99 底層版本**。並嚴格遵守系統開發的命名規範與指標操作，深刻體會到 AI 作為「結對編程 (Pair Programming)」夥伴的強大產能。

### 🎯 習題 4：現代系統程式設計專書撰寫

* **作業連結**：[填入書本 Markdown 檔案或 PDF 的連結]
* **核心概念**：
採用架構驅動 (Architecture-Driven) 的方式，梳理系統程式知識。
* **實作亮點**：
探討了虛擬機器設計中的「堆疊機 (Stack VM)」與「暫存器機 (Register VM)」的差異，並深入研究了記憶體邊界檢查對於防止緩衝區溢位 (Buffer Overflow) 等安全漏洞的重要性。

### 🎯 習題 5：作業系統並行控制 (Concurrency)

* **作業連結**：[填入程式碼連結，例如: `./concurrency/`]
* **核心概念**：
處理多執行緒 (Multi-threading) 下的共享資源競爭與死鎖問題。
* **實作亮點**：
1. **銀行存提款 (Race Condition)**：實作 POSIX `pthread_mutex_t` 來保護臨界區，確保 10 萬次並行存提款後餘額正確歸零。
2. **生產者消費者**：利用互斥鎖與條件變數 (`pthread_cond_t`)，解決了忙碌等待 (Busy Waiting) 的效能浪費。
3. **哲學家用餐 (Deadlock)**：透過實作「資源分級定序策略」，破除循環等待條件，完美防範死鎖發生。



### 🎯 習題 6：Linux 行程與檔案系統 API (期中專案：AuraShell)

* **作業連結**：[填入 AuraShell 專案連結]
* **核心概念**：
理解 UNIX「萬物皆檔案」的哲學，實作自己的命令列直譯器。
* **實作亮點**：
1. 利用 `fork()` 與 `execvp()` 實現「細胞分裂與大腦移植」，啟動外部指令。
2. 利用 `open()` 與 `dup2()` 實作標準輸出重導向 (`>`)，篡改檔案描述子 (File Descriptor) 表。
3. 利用非阻塞的 `waitpid` 清理背景執行 (`&`) 的子行程，防止殭屍行程 (Zombie Process) 癱瘓系統。



---

## 💡 學期總結心得

這學期的系統程式課程，讓我從「只會呼叫高階 API 的應用程式開發者」，蛻變成「能理解底層記憶體與行程運作的系統工程師」。

從手刻 AST 樹、與 Segmentation Fault 奮戰，到看見自己寫的 Shell 成功重導向輸出，這個從無到有的過程非常有成就感。這為我未來在進行伺服器架構設計、資安防護以及高效能程式開發，打下了極為堅實的基礎。
