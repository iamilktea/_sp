#include "aura.h"

char* ReadSourceFile(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filepath);
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* buffer = (char*)malloc(length + 1);
    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: ./aura <filename.au>\n");
        return 1;
    }

    char* source = ReadSourceFile(argv[1]);
    
    TokenizeSource(source);
    AstNode* ast = ParseProgram();
    
    printf("--- Executing %s ---\n", argv[1]);
    InterpretAst(ast);
    printf("--- Execution Complete ---\n");

    free(source);
    return 0;
}
