#include "vm.h"

void initVM(VM *vm)
{
    vm->size = 0;
    vm->reg[RCOMP] = CMPNONE;
    vm->reg[RPC] = 0;
}

static void addByte(VM *vm, uint8_t byte)
{
    vm->size++;
    if (vm->size > 16384) ERROR("VM program is full.");
    vm->code[vm->size - 1] = byte;
}

void addInstruction(VM *vm, OpCode op_code, int args, ...)
{
    addByte(vm, op_code);
    va_list va_args;
    va_start(va_args, args);

    switch (args)
    {
        case 1:
        {
            int arg = va_arg(va_args, int);
            uint8_t bytes[3] = {(arg & 0xFF0000) >> 16, (arg & 0xFF00) >> 8, arg & 0xFF};
            addByte(vm, bytes[0]);
            addByte(vm, bytes[1]);
            addByte(vm, bytes[2]);
            break;
        }
        case 2:
        {
            uint8_t dest = va_arg(va_args, int);
            addByte(vm, dest);
            int arg = va_arg(va_args, int);
            uint8_t bytes[2] = {(arg & 0xFF00) >> 8, arg & 0xFF};
            addByte(vm, bytes[0]);
            addByte(vm, bytes[1]);
            break;
        }
        case 3:
        {
            uint8_t dest = va_arg(va_args, int);
            uint8_t arg1 = va_arg(va_args, int);
            uint8_t arg2 = va_arg(va_args, int);
            addByte(vm, dest);
            addByte(vm, arg1);
            addByte(vm, arg2);
            break;
        }
        default:
        {
            ERROR("Too many arguments for instruction.");
            break;
        }
    }
}

static inline uint8_t nextByte(VM *vm)
{
    uint8_t val = vm->code[vm->reg[RPC]];
    vm->reg[RPC]++;
    return val;
}

static inline void printRegisters(VM *vm)
{
    printf("[ ");
    for (int i = 0; i < NUM_REGISTERS; i++) printf("%d ", vm->reg[i]);
    printf(" ]\n");
}

uint16_t run(VM *vm)
{
    #define DEBUG true
    #define BINARY(op, arg1, arg2, arg3, name) \
        do \
        { \
            if (DEBUG) printf("OP_%s [%d] %d %d\n", name, arg1, arg2, arg3); \
            vm->reg[arg1] = vm->reg[arg2] op vm->reg[arg3]; \
        } \
        while (false); \

    if (DEBUG) printRegisters(vm);

    while (true)
    {
        OpCode op = nextByte(vm);
        uint8_t arg1 = nextByte(vm);
        uint8_t arg2 = nextByte(vm);
        uint8_t arg3 = nextByte(vm);

        switch (op)
        {
            case OP_LDC:
            {
                vm->reg[arg1] = (arg2 << 8) | arg3;
                if (DEBUG) printf("OP_LDC [%d] %d\n", arg1, ((arg2 << 8) | arg3));
                break;
            }
            case OP_MOV:
            {
                vm->reg[arg1] = vm->reg[(arg2 << 8) | arg3];
                if (DEBUG) printf("OP_MOV [%d] %d\n", arg1, ((arg2 << 8) | arg3));
                break;
            }
            case OP_ADD: BINARY(+, arg1, arg2, arg3, "ADD"); break;
            case OP_SUB: BINARY(-, arg1, arg2, arg3, "SUB"); break;
            case OP_MUL: BINARY(*, arg1, arg2, arg3, "MUL"); break;
            case OP_DIV: BINARY(/, arg1, arg2, arg3, "DIV"); break;
            case OP_AND: BINARY(&, arg1, arg2, arg3, "AND"); break;
            case OP_NOT:
            {
                vm->reg[arg1] = ~(vm->reg[(arg2 << 8) | arg3]);
                if (DEBUG) printf("OP_NOT [%d] %d\n", arg1, ((arg2 << 8) | arg3));
                break;
            }
            case OP_CMP:
            {
                vm->reg[RCOMP] = (vm->reg[arg1] == vm->reg[(arg2 << 8) | arg3])? CMPPOS : CMPNEG;
                if (DEBUG) printf("OP_CMP %d %d\n", arg1, (arg2 << 8 | arg3));
                break;
            }
            case OP_JMP:
            {
                int steps = (arg1 << 16) | (arg2 << 8) | arg3;
                vm->reg[RPC] += steps;
                if (DEBUG) printf("OP_JMP %d\n", steps);
                break;
            }
            case OP_BNP:
            {
                int pos = (arg1 << 16) | (arg2 << 8) | arg3;
                if (vm->reg[RCOMP] == CMPPOS) vm->reg[RPC] = pos;
                if (DEBUG) printf("OP_BNP %d\n", pos);
                break;
            }
            case OP_BNN:
            {
                int pos = (arg1 << 16) | (arg2 << 8) | arg3;
                if (vm->reg[RCOMP] == CMPNEG) vm->reg[RPC] = pos;
                if (DEBUG) printf("OP_BNN %d\n", pos);
                break;
            }
            case OP_NONE: if (DEBUG) printf("OP_NONE\n"); break;
            case OP_RET:
            {
                int exit_val = (arg1 << 16) | (arg2 << 8) | arg3;
                if (DEBUG) printf("OP_RET %d\n", exit_val);
                if (exit_val != 0) ERROR("VM exited with error.");
                return vm->reg[R0];
            }
        }

        if (DEBUG) printRegisters(vm);
    }

    #undef DEBUG
    #undef BINARY
}