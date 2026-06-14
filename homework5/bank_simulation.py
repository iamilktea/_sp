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
