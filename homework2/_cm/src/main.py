import sys
import os

from lexer import tokenize
from parser import Parser
from compiler import Compiler
from vm import VM

def main():
    # 檢查是否有傳入檔案參數
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
        # 1. 詞法分析
        tokens = tokenize(source_code)
        
        # 2. 語法分析 (建構 AST)
        parser = Parser(tokens)
        ast = parser.parse()
        
        # 3. 程式碼生成 (建構 Bytecode)
        compiler = Compiler()
        bytecode = compiler.compile(ast)
        
        # 4. 虛擬機執行
        print(f"--- 執行 {filepath} ---")
        vm = VM()
        vm.run(bytecode)
        print("--- 執行結束 ---")

    except Exception as e:
        print(e)

if __name__ == "__main__":
    main()
