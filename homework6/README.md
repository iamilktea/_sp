
# 深度解析：Linux 行程管理與檔案 I/O 重導向

## 一、 檔案描述子 (File Descriptor, FD) 的秘密

當一個行程啟動時，作業系統會自動發給它一張「檔案登記表」。這個表的前三個欄位是永遠固定且預設開啟的，這就是我們常說的標準 I/O：

* **0 (Standard Input, `stdin`)**：標準輸入，預設連結到你的**鍵盤**。
* **1 (Standard Output, `stdout`)**：標準輸出，預設連結到你的**終端機螢幕**。
* **2 (Standard Error, `stderr`)**：標準錯誤輸出，預設也是連結到**螢幕**（用來跟正常輸出分開，方便除錯）。

當你呼叫 `printf("Hello")` 時，底層其實就是對 **FD 1** 執行寫入動作。

### 檔案操作四天王

如果我們想操作非預設的檔案，就會用到以下四個系統呼叫 (System Calls)：

1. **`open(filename, flags, mode)`**：向系統請求開啟檔案。系統會從登記表裡找「**最小且未被使用的數字**」還給你。通常會得到 **3**。
2. **`write(fd, buffer, size)`**：對指定的 FD 寫入資料。
3. **`read(fd, buffer, size)`**：從指定的 FD 讀取資料。
4. **`close(fd)`**：用完了，把這個 FD 歸還給作業系統，讓出登記表的位子。

## 二、 行程的誕生與變身：Fork 與 Exec

在 Linux 中，要執行一個新程式，必須經過「細胞分裂」與「大腦移植」兩個階段。

1. **`fork()` - 細胞分裂**：
呼叫 `fork()` 會精準複製一個一模一樣的「子行程 (Child Process)」。最神奇的是，子行程會**繼承父行程所有的檔案登記表 (FD Table)**。
* 對父行程來說，`fork()` 會回傳子行程的 PID。
* 對子行程來說，`fork()` 會回傳 `0`。我們靠這個回傳值來寫 `if/else` 分道揚鑣。


2. **`execvp(file, args)` - 大腦移植**：
子行程雖然誕生了，但它的程式碼跟老爸一模一樣沒什麼用。呼叫 `execvp` 會把子行程的記憶體與程式碼完全抹除，替換成新的外部程式（例如 `ls` 或 `echo`）。**但關鍵是：它的 FD 登記表會被保留下來！**

## 三、 移花接木的魔術：`dup2`

如果我們想把 `ls` 的結果存進檔案而不是印在螢幕上，該怎麼辦？
這時就需要 **`dup2(oldfd, newfd)`** 登場。它的功能是「將登記表上的 `newfd` 欄位，強制蓋牌並指向跟 `oldfd` 同一個目標」。

如果我們執行 `dup2(3, 1)`：

* 原本指向螢幕的 `1 (stdout)`，會被無情切斷。
* `1 (stdout)` 現在會被重新接管，指向 `3` 所代表的檔案。
* 從此以後，任何呼叫 `printf` 的動作（包含 `execvp` 變身後的程式），資料都會不知不覺地流進檔案裡！

---

## 四、 終極實戰：實作 Shell 的 `ls -l > output.txt`

以下這支 C 程式碼，完美融合了你提到的 10 個關鍵字。它會透過 `fork` 產生子行程，用 `open` 開啟檔案，透過 `dup2` 將標準輸出 (1) 重導向到檔案，最後用 `execvp` 執行 `ls -l`。

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

// 為了符合嚴格的命名規範，自定義函數使用 UpperCamelCase
void ExecuteWithRedirection() {
    pid_t pid = fork(); // 關鍵字 1: fork

    if (pid < 0) {
        // fork 失敗
        perror("Fork failed");
        exit(1);
    } 
    else if (pid == 0) {
        // ==========================================
        // 這裡是子行程 (Child Process) 的領域
        // ==========================================

        // 關鍵字 2, 3: open (回傳的 fd 通常會是 3)
        // O_WRONLY: 唯寫, O_CREAT: 若無則建立, O_TRUNC: 若有則清空內容
        int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            perror("Open failed");
            exit(1);
        }

        // 關鍵字 4, 5, 6, 7: dup2, stdin(0), stdout(1), stderr(2)
        // 將標準輸出 stdout (1) 重導向到剛剛開啟的 fd (3)
        // 也就是：把往螢幕送的管線，拔起來插到 output.txt 上
        dup2(fd, STDOUT_FILENO); // STDOUT_FILENO 常數就是 1

        // 關鍵字 8: close
        // 既然 1 已經指向檔案了，原本的 3 就用不到了，關閉以節省資源
        close(fd);

        // 準備要執行的外部程式及其參數
        char *args[] = {"ls", "-l", NULL};

        // 關鍵字 9: execvp
        // 進行「大腦移植」，把當前的子行程替換成 ls 程式。
        // 因為 FD 表被繼承了，所以 ls 的輸出結果會自動流向 output.txt
        execvp("ls", args);

        // 如果 execvp 成功，這行永遠不會被執行到
        perror("Execvp failed");
        exit(1);
    } 
    else {
        // ==========================================
        // 這裡是父行程 (Parent Process) 的領域
        // ==========================================
        
        // 父行程只是在旁邊等子行程把活幹完
        int status;
        wait(&status);
        
        // 關鍵字 10: write (展示直接寫入 stdout)
        // 父行程的 stdout 依然指向螢幕，完全不受子行程瞎搞的影響
        char msg[] = "[系統報告] 子行程已完成，請查看 output.txt 檔案。\n";
        write(STDOUT_FILENO, msg, sizeof(msg) - 1); 
    }
}

int main() {
    ExecuteWithRedirection();
    return 0;
}

```
