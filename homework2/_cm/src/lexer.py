import re

def tokenize(code):
    token_spec = [
        ('NUMBER',   r'\d+'),
        ('KEYWORD',  r'\b(let|print|if|else|while)\b'),
        ('ID',       r'[A-Za-z_][A-Za-z0-9_]*'),
        ('COMP',     r'==|!=|<=|>=|<|>'),
        ('ASSIGN',   r'='),
        ('MATH',     r'[+\-*/]'),
        ('PUNC',     r'[{}();]'),
        ('SKIP',     r'[ \t\n]+'),
        ('COMMENT',  r'//.*'), # 支援單行註解
    ]
    tokens = []
    pos = 0
    while pos < len(code):
        match = None
        for kind, pattern in token_spec:
            regex = re.compile(pattern)
            match = regex.match(code, pos)
            if match:
                if kind != 'SKIP' and kind != 'COMMENT':
                    tokens.append((kind, match.group(0)))
                pos = match.end()
                break
        if not match:
            raise Exception(f"Syntax Error: 詞法解析錯誤於 -> {code[pos:pos+10]}")
    return tokens
