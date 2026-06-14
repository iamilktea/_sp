# src/main.py
import sys
import os

from lexer import tokenize
from parser import Parser
from interpreter import Interpreter

def main():
    if len(sys.argv) < 2:
        print("用法: python src/main.py <filename.au>")
        sys.exit(1)

    filepath = sys.argv[1]
    
    if not os.path.exists(filepath):
        print(f"錯誤: 找不到檔案 '{filepath}'")
        sys.exit(1)

    with open(filepath, 'r', encoding='utf-8') as f:
        source_code = f.read()

    try:
        # 1. 詞法分析 (Lexer)
        tokens = tokenize(source_code)
        
        # 2. 語法分析 (建構 AST)
        parser = Parser(tokens)
        ast = parser.parse()
        
        # 3. 直譯器直接執行 AST
        print(f"--- 執行 {filepath} (Interpreter 模式) ---")
        interpreter = Interpreter()
        interpreter.interpret(ast)
        print("--- 執行結束 ---")

    except Exception as e:
        print(e)

if __name__ == "__main__":
    main()
