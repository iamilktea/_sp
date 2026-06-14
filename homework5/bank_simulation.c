#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ITERATIONS 100000

long g_bankBalance = 0;          // 全域共享帳戶餘額
pthread_mutex_t g_accountMutex;  // 互斥鎖

// 存款執行緒常式 (大駝峰命名)
void* DepositRoutine(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&g_accountMutex);    // 【加鎖】進入臨界區
        g_bankBalance += 1;                      // 存款 1 元 (非原子操作)
        pthread_mutex_unlock(&g_accountMutex);  // 【解鎖】離開臨界區
    }
    return NULL;
}

// 提款執行緒常式 (大駝峰命名)
void* WithdrawRoutine(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&g_accountMutex);    // 【加鎖】進入臨界區
        g_bankBalance -= 1;                      // 提款 1 元 (非原子操作)
        pthread_mutex_unlock(&g_accountMutex);  // 【解鎖】離開臨界區
    }
    return NULL;
}

int main() {
    pthread_t depositorThread, withdrawerThread;

    // 初始化互斥鎖
    if (pthread_mutex_init(&g_accountMutex, NULL) != 0) {
        printf("互斥鎖初始化失敗\n");
        return 1;
    }

    // 建立並行執行緒
    pthread_create(&depositorThread, NULL, DepositRoutine, NULL);
    pthread_create(&withdrawerThread, NULL, WithdrawRoutine, NULL);

    // 等待執行緒結束歸隊
    pthread_join(depositorThread, NULL);
    pthread_join(withdrawerThread, NULL);

    // 銷毀互斥鎖資源
    pthread_mutex_destroy(&g_accountMutex);

    printf("[C 語言銀行帳戶模擬結果]\n");
    printf("預期最終餘額: 0 元\n");
    printf("實際最終餘額: %ld 元\n", g_bankBalance);

    return 0;
}
