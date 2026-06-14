
#!/bin/bash
echo "開始清理專案..."
make clean
echo "開始編譯 Aura 專案..."
make
echo "編譯完成！執行檔已產出：./aura"
