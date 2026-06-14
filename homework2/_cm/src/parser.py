# AST 節點定義
class AST: pass
class NumberNode(AST):
    def __init__(self, value): self.value = value
class VarNode(AST):
    def __init__(self, name): self.name = name
class BinOpNode(AST):
    def __init__(self, left, op, right):
        self.left = left; self.op = op; self.right = right
class LetNode(AST):
    def __init__(self, name, expr):
        self.name = name; self.expr = expr
class AssignNode(AST):
    def __init__(self, name, expr):
        self.name = name; self.expr = expr
class PrintNode(AST):
    def __init__(self, expr): self.expr = expr
class BlockNode(AST):
    def __init__(self, statements): self.statements = statements
class IfNode(AST):
    def __init__(self, condition, true_block, false_block=None):
        self.condition = condition; self.true_block = true_block; self.false_block = false_block
class WhileNode(AST):
    def __init__(self, condition, body):
        self.condition = condition; self.body = body

# Parser 實作
class Parser:
    def __init__(self, tokens):
        self.tokens = tokens
        self.pos = 0

    def current(self):
        return self.tokens[self.pos] if self.pos < len(self.tokens) else ('EOF', '')

    def consume(self, expected_kind, expected_val=None):
        kind, val = self.current()
        if kind == expected_kind and (expected_val is None or val == expected_val):
            self.pos += 1
            return val
        raise Exception(f"Parser Error: 預期 {expected_kind}({expected_val}), 卻得到 {kind}({val})")

    def parse(self):
        stmts = []
        while self.current()[0] != 'EOF':
            stmts.append(self.parse_statement())
        return BlockNode(stmts)

    def parse_block(self):
        self.consume('PUNC', '{')
        stmts = []
        while self.current()[1] != '}':
            stmts.append(self.parse_statement())
        self.consume('PUNC', '}')
        return BlockNode(stmts)

    def parse_statement(self):
        kind, val = self.current()
        if kind == 'KEYWORD' and val == 'let':
            self.consume('KEYWORD', 'let')
            name = self.consume('ID')
            self.consume('ASSIGN', '=')
            expr = self.parse_expr()
            self.consume('PUNC', ';')
            return LetNode(name, expr)
        elif kind == 'KEYWORD' and val == 'print':
            self.consume('KEYWORD', 'print')
            self.consume('PUNC', '(')
            expr = self.parse_expr()
            self.consume('PUNC', ')')
            self.consume('PUNC', ';')
            return PrintNode(expr)
        elif kind == 'KEYWORD' and val == 'if':
            self.consume('KEYWORD', 'if')
            self.consume('PUNC', '(')
            cond = self.parse_condition()
            self.consume('PUNC', ')')
            true_block = self.parse_block()
            false_block = None
            if self.current() == ('KEYWORD', 'else'):
                self.consume('KEYWORD', 'else')
                false_block = self.parse_block()
            return IfNode(cond, true_block, false_block)
        elif kind == 'KEYWORD' and val == 'while':
            self.consume('KEYWORD', 'while')
            self.consume('PUNC', '(')
            cond = self.parse_condition()
            self.consume('PUNC', ')')
            body = self.parse_block()
            return WhileNode(cond, body)
        elif kind == 'ID':
            name = self.consume('ID')
            self.consume('ASSIGN', '=')
            expr = self.parse_expr()
            self.consume('PUNC', ';')
            return AssignNode(name, expr)
        else:
            raise Exception(f"Parser Error: 無法識別的敘述 -> {val}")

    def parse_condition(self):
        left = self.parse_expr()
        op = self.consume('COMP')
        right = self.parse_expr()
        return BinOpNode(left, op, right)

    def parse_expr(self):
        node = self.parse_term()
        while self.current()[0] == 'MATH' and self.current()[1] in ('+', '-'):
            op = self.consume('MATH')
            right = self.parse_term()
            node = BinOpNode(node, op, right)
        return node

    def parse_term(self):
        node = self.parse_factor()
        while self.current()[0] == 'MATH' and self.current()[1] in ('*', '/'):
            op = self.consume('MATH')
            right = self.parse_factor()
            node = BinOpNode(node, op, right)
        return node

    def parse_factor(self):
        kind, val = self.current()
        if kind == 'NUMBER':
            return NumberNode(int(self.consume('NUMBER')))
        elif kind == 'ID':
            return VarNode(self.consume('ID'))
        elif val == '(':
            self.consume('PUNC', '(')
            node = self.parse_expr()
            self.consume('PUNC', ')')
            return node
        raise Exception(f"Parser Error: 運算式中出現非預期 Token -> {val}")
