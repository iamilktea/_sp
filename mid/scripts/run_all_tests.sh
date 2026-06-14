#!/bin/bash
# 確保已經編譯
make

echo "執行 Lexer 單元測試..."
gcc -Wall -std=c99 src/lexer.c tests/test_lexer.c -o tests/run_lexer_test
./tests/run_lexer_test

echo "執行 Aura 腳本整合測試..."
./aura tests/test_scripts/math_test.au
./aura tests/test_scripts/loop_test.au
