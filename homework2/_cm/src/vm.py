class VM:
    def __init__(self):
        self.stack = []
        self.env = {}

    def run(self, bytecode):
        pc = 0  
        while pc < len(bytecode):
            op, arg = bytecode[pc]
            if op == 'PUSH':
                self.stack.append(arg)
            elif op == 'LOAD':
                if arg not in self.env:
                    raise Exception(f"Runtime Error: 變數未定義 '{arg}'")
                self.stack.append(self.env[arg])
            elif op == 'STORE':
                self.env[arg] = self.stack.pop()
            elif op == 'PRINT':
                print(f">> {self.stack.pop()}")
            elif op == 'OP':
                r = self.stack.pop()
                l = self.stack.pop()
                if arg == '+': self.stack.append(l + r)
                elif arg == '-': self.stack.append(l - r)
                elif arg == '*': self.stack.append(l * r)
                elif arg == '/': self.stack.append(l // r)
                elif arg == '==': self.stack.append(1 if l == r else 0)
                elif arg == '!=': self.stack.append(1 if l != r else 0)
                elif arg == '<': self.stack.append(1 if l < r else 0)
                elif arg == '>': self.stack.append(1 if l > r else 0)
                elif arg == '<=': self.stack.append(1 if l <= r else 0)
                elif arg == '>=': self.stack.append(1 if l >= r else 0)
            elif op == 'JMP':
                pc = arg
                continue
            elif op == 'JMP_IF_FALSE':
                cond = self.stack.pop()
                if cond == 0:
                    pc = arg
                    continue
            elif op == 'HALT':
                break
            pc += 1
