#include "vm.h"

void printByte(uint8_t byte)
{
    printf("0x");
    #define printNibble(n) \
        do \
        { \
            switch (n) \
            { \
                case 15: printf("F"); break; \
                case 14: printf("E"); break; \
                case 13: printf("D"); break; \
                case 12: printf("C"); break; \
                case 11: printf("B"); break; \
                case 10: printf("A"); break; \
                default: printf("%d", n); break; \
            } \
        } \
        while (false);

    int first_nibble = byte >> 4;
    int second_nibble = byte & 0xF;
    printNibble(first_nibble);
    printNibble(second_nibble);
    printf(" ");
}

int main(void)
{
    VM vm;
    initVM(&vm);
    addInstruction(&vm, OP_LDC, 2, R0, 2);
    addInstruction(&vm, OP_LDC, 2, R1, 10);
    addInstruction(&vm, OP_MUL, 3, R0, R0, R1);
    addInstruction(&vm, OP_LDC, 2, R1, 2);
    addInstruction(&vm, OP_DIV, 3, R0, R0, R1);
    addInstruction(&vm, OP_RET, 1, 0);
    printf("Instructions:\n");
    for (int i = 0; i < vm.size; ++i) printByte(vm.code[i]);
    printf("\n\nProcess:\n");
    uint16_t result = run(&vm);
    printf("\nResult:\n");
    printf("%d\n", result);

    return 0;
}