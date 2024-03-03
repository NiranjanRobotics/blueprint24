#include "vm.h"

int main(void)
{
    VM vm;
    initVM(&vm);
    addInstruction(&vm, OP_LDC, 2, R0, 2);
    addInstruction(&vm, OP_LDC, 2, R1, 10);
    addInstruction(&vm, OP_MUL, 3, R0, R0, R1);
    addInstruction(&vm, OP_RET, 1, 0);
    for (int i = 0; i < vm.size; i += 4) printf("[%d %d %d %d]", vm.code[i], vm.code[i + 1], vm.code[i + 2], vm.code[i + 3]);
    uint16_t result = run(&vm);
    printf("%d\n", result);

    return 0;
}