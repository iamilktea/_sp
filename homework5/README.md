

# 核心多執行緒並行控制原理解析

## 一、 核心概念說明

### 1. 執行緒 (Thread)

* **定義**：執行緒是作業系統能夠進行運算排程的最小單位。它被包含在行程（Process）之中，是行程中的實際運作單位。
* **特性**：同一個行程內的多個執行緒會**共享該行程的記憶體空間**（如全域變數、堆積空間 Heap），但每個執行緒都擁有自己獨立的**程式計數器 (PC)**、**暫存器**與**堆疊空間 (Stack)**。

### 2. 競態條件 (Race Condition)

* **定義**：當兩個或多個執行緒同時存取相同的共享資料，且至少有一個執行緒在進行寫入操作時，最終的資料正確性便取決於執行緒之間微妙的**執行順序與作業系統的排程時機**。
* **成因**：高階語言的許多操作（例如 `count++` 或 `balance += 1`）在底層 CPU 執行時並非原子操作（Atomic Operation），而是被拆解為「讀取 $\rightarrow$ 修改 $\rightarrow$ 寫回」三個步驟。當這些步驟交錯執行時，就會發生數值被互相覆寫的錯誤。

### 3. 互斥鎖 (Mutex / Lock)

* **定義**：互斥鎖（Mutual Exclusion）是一種用來維護並行安全鎖定機制的同步原語。
* **原理**：它用來保護**臨界區 (Critical Section)**（即存取共享資源的程式碼區塊）。在任一給定時間內，**只有一個執行緒能夠獲得互斥鎖並進入臨界區**。其他企圖獲取該鎖的執行緒將會被掛起等待，直到鎖被釋放，藉此確保操作的原子性。

### 4. 死鎖 (Deadlock)

* **定義**：指兩個或多個執行緒在執行過程中，因爭奪資源而造成的一種**互相等待、互不相讓的僵持狀態**。
* **Coffman 必要條件**：死鎖必須同時滿足以下四個條件才會發生：
1. **互斥 (Mutual Exclusion)**：資源一次只能被一個執行緒佔用。
2. **持有並等待 (Hold and Wait)**：執行緒已持有至少一個資源，又在等待新資源。
3. **不可剝奪 (No Preemption)**：已被持有的資源不能被強行奪走，只能自願釋放。
4. **循環等待 (Circular Wait)**：存在一個封閉的執行緒等待環（A 等 B，B 等 C，C 等 A）。



---

## 二、 銀行存提款模擬範例（解決競態條件）

以下模擬同一個帳戶同時被兩個執行緒（一個存款、一個提款）頻繁操作 100,000 次的情境。透過互斥鎖的保護，確保最終金額絕對正確（回歸 0 元）。

### 1. C 語言版本 (`bank_simulation.c`)

C 語言中使用 POSIX 執行緒（`pthread`）與互斥鎖（`pthread_mutex_t`）。

```c
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

```

### 2. Python 版本 (`bank_simulation.py`)

Python 中使用 `threading.Thread` 與 `threading.Lock`。

```python
import threading

ITERATIONS = 100000
g_bankBalance = 0               # 全域共享帳戶餘額
g_accountLock = threading.Lock() # 互斥鎖

# 存款執行緒常式 (大駝峰命名)
def DepositRoutine():
    global g_bankBalance
    for _ in range(ITERATIONS):
        # 使用 with 敘述句會自動在進入時加鎖 (acquire)，離開時解鎖 (release)
        with g_accountLock:
            g_bankBalance += 1

# 提款執行緒常式 (大駝峰命名)
def WithdrawRoutine():
    global g_bankBalance
    for _ in range(ITERATIONS):
        with g_accountLock:
            g_bankBalance -= 1

def Main():
    # 建立執行緒
    depositor = threading.Thread(target=DepositRoutine)
    withdrawer = threading.Thread(target=WithdrawRoutine)

    # 啟動執行緒
    depositor.start()
    withdrawer.start()

    # 等待執行緒結束
    depositor.join()
    withdrawer.join()

    print("[Python 銀行帳戶模擬結果]")
    print(f"預期最終餘額: 0 元")
    print(f"實際最終餘額: {g_bankBalance} 元")

if __name__ == "__main__":
    Main()

```

---

## 三、 死鎖 (Deadlock) 錯誤示範與防範說明

### 1. 死鎖是如何發生的？（錯誤程式碼概念）

假設有兩個互斥鎖 `LockA` 與 `LockB`：

* **執行緒 1** 的邏輯：先鎖住 `LockA` $\rightarrow$ 進入睡眠 $\rightarrow$ 試圖鎖住 `LockB`。
* **執行緒 2** 的邏輯：先鎖住 `LockB` $\rightarrow$ 進入睡眠 $\rightarrow$ 試圖鎖住 `LockA`。

當兩個執行緒並行時，執行緒 1 拿到了 A，執行緒 2 拿到了 B。接著執行緒 1 永遠在等 B 釋放，執行緒 2 永遠在等 A 釋放，這就陷入了**循環等待**，程式會徹底卡死。

### 2. 如何防範與解決死鎖？

本專案採用的解決策略是 **資源分級分配定序法 (Resource Ordering)**：

* **原理**：我們為系統內所有的鎖規定一個固定的嚴格存取順序。
* **實作**：不論是哪一個執行緒，只要同時需要這兩把鎖，**一律規定必須先獲取編號/地址較小的鎖，才能獲取較大的鎖**。
* **效果**：這破壞了 Coffman 定理中的「循環等待」條件，從根本上杜絕了死鎖的發生。
