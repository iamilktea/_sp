# src/interpreter.py
from parser import *

class Interpreter:
    def __init__(self):
        self.env = {}  # 儲存變數的環境表 (Symbol Table)

    def interpret(self, ast):
        self.visit(ast)

    def visit(self, node):
        if isinstance(node, BlockNode):
            for stmt in node.statements:
                self.visit(stmt)
        elif isinstance(node, LetNode) or isinstance(node, AssignNode):
            value = self.visit(node.expr)
            self.env[node.name] = value
        elif isinstance(node, PrintNode):
            value = self.visit(node.expr)
            print(f">> {value}")
        elif isinstance(node, IfNode):
            cond_value = self.visit(node.condition)
            # 布林值判斷：非 0 即為真
            if cond_value != 0:
                self.visit(node.true_block)
            elif node.false_block:
                self.visit(node.false_block)
        elif isinstance(node, WhileNode):
            while self.visit(node.condition) != 0:
                self.visit(node.body)
        elif isinstance(node, BinOpNode):
            left = self.visit(node.left)
            right = self.visit(node.right)
            if node.op == '+': return left + right
            elif node.op == '-': return left - right
            elif node.op == '*': return left * right
            elif node.op == '/': return left // right
            elif node.op == '==': return 1 if left == right else 0
            elif node.op == '!=': return 1 if left != right else 0
            elif node.op == '<': return 1 if left < right else 0
            elif node.op == '>': return 1 if left > right else 0
            elif node.op == '<=': return 1 if left <= right else 0
            elif node.op == '>=': return 1 if left >= right else 0
        elif isinstance(node, NumberNode):
            return node.value
        elif isinstance(node, VarNode):
            if node.name not in self.env:
                raise Exception(f"Runtime Error: 變數 '{node.name}' 未定義")
            return self.env[node.name]
