#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

// 函式宣告 (採用 UpperCamelCase)
void ParseCommand(char* cmd, char** args, int* background, char** redirectFile);
void ExecuteCommand(char** args, int background, char* redirectFile);

int main() {
    char cmd[MAX_CMD_LEN];
    char* args[MAX_ARGS];
    int background;
    char* redirectFile;

    printf("=========================================\n");
    printf("  🚀 歡迎使用 AuraShell (期中專案版本)  \n");
    printf("=========================================\n");

    while (1) {
        // 清理已結束的背景子行程 (防止 Zombie Process)
        while (waitpid(-1, NULL, WNOHANG) > 0);

        printf("AuraShell> ");
        if (fgets(cmd, MAX_CMD_LEN, stdin) == NULL) {
            printf("\n登出 AuraShell。\n");
            break;
        }

        // 移除換行符號
        cmd[strcspn(cmd, "\n")] = '\0';
        if (strlen(cmd) == 0) continue;

        if (strcmp(cmd, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }

        // 解析指令
        ParseCommand(cmd, args, &background, &redirectFile);

        if (args[0] != NULL) {
            ExecuteCommand(args, background, redirectFile);
        }
    }
    return 0;
}

// 解析使用者輸入，處理背景 '&' 與重導向 '>'
void ParseCommand(char* cmd, char** args, int* background, char** redirectFile) {
    *background = 0;
    *redirectFile = NULL;
    int i = 0;

    char* token = strtok(cmd, " ");
    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            *background = 1; // 標記為背景執行
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " "); // 取得 '>' 後面的檔名
            if (token != NULL) *redirectFile = token;
        } else {
            args[i++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[i] = NULL; // execvp 需要以 NULL 結尾的陣列
}

// 核心：Fork, Exec, 與 Dup2
void ExecuteCommand(char** args, int background, char* redirectFile) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork 失敗");
    } 
    else if (pid == 0) {
        // --- 子行程領域 ---
        
        // 處理輸出重導向
        if (redirectFile != NULL) {
            int fd = open(redirectFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("無法開啟重導向檔案");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO); // 將螢幕輸出 (1) 轉向到檔案
            close(fd);
        }

        // 執行外部程式
        if (execvp(args[0], args) == -1) {
            printf("AuraShell: 找不到指令 '%s'\n", args[0]);
            exit(1);
        }
    } 
    else {
        // --- 父行程領域 ---
        if (!background) {
            // 前景執行：父行程必須等待子行程結束
            int status;
            waitpid(pid, &status, 0);
        } else {
            // 背景執行：印出 PID 並立刻返回提示字元
            printf("[背景執行] PID: %d\n", pid);
        }
    }
}
