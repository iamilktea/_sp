from parser import *

class Compiler:
    def __init__(self):
        self.code = []
        self.label_idx = 0

    def new_label(self):
        self.label_idx += 1
        return f"L{self.label_idx}"

    def emit(self, op, arg=None):
        self.code.append((op, arg))

    def compile(self, ast):
        self.visit(ast)
        self.emit('HALT')
        return self.resolve_labels()

    def resolve_labels(self):
        resolved = []
        labels = {}
        for i, (op, arg) in enumerate(self.code):
            if op == 'LABEL':
                labels[arg] = len(resolved)
            else:
                resolved.append((op, arg))
        
        final_code = []
        for op, arg in resolved:
            if op in ('JMP', 'JMP_IF_FALSE'):
                final_code.append((op, labels[arg]))
            else:
                final_code.append((op, arg))
        return final_code

    def visit(self, node):
        if isinstance(node, BlockNode):
            for stmt in node.statements: self.visit(stmt)
        elif isinstance(node, NumberNode):
            self.emit('PUSH', node.value)
        elif isinstance(node, VarNode):
            self.emit('LOAD', node.name)
        elif isinstance(node, LetNode) or isinstance(node, AssignNode):
            self.visit(node.expr)
            self.emit('STORE', node.name)
        elif isinstance(node, PrintNode):
            self.visit(node.expr)
            self.emit('PRINT')
        elif isinstance(node, BinOpNode):
            self.visit(node.left)
            self.visit(node.right)
            self.emit('OP', node.op)
        elif isinstance(node, IfNode):
            l_else = self.new_label()
            l_end = self.new_label()
            self.visit(node.condition)
            self.emit('JMP_IF_FALSE', l_else)
            self.visit(node.true_block)
            self.emit('JMP', l_end)
            self.emit('LABEL', l_else)
            if node.false_block: self.visit(node.false_block)
            self.emit('LABEL', l_end)
        elif isinstance(node, WhileNode):
            l_start = self.new_label()
            l_end = self.new_label()
            self.emit('LABEL', l_start)
            self.visit(node.condition)
            self.emit('JMP_IF_FALSE', l_end)
            self.visit(node.body)
            self.emit('JMP', l_start)
            self.emit('LABEL', l_end)
