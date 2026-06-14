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
