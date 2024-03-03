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
        }; break;
        case 2:
        {
            uint8_t dest = va_arg(va_args, int);
            addByte(vm, dest);
            int arg = va_arg(va_args, int);
            uint8_t bytes[2] = {(arg & 0xFF00) >> 8, arg & 0xFF};
            addByte(vm, bytes[0]);
            addByte(vm, bytes[1]);
        }; break;
        case 3:
        {
            uint8_t dest = va_arg(va_args, uint8_t);
            uint8_t arg1 = va_arg(va_args, uint8_t);
            uint8_t arg2 = va_arg(va_args, uint8_t);
            addByte(vm, dest);
            addByte(vm, arg1);
            addByte(vm, arg2);
        }; break;
        default: {
            ERROR("Too many arguments for instruction.");
        }; break;
    }
}

inline static uint8_t nextByte(VM *vm) {
    vm->reg[RPC]++;
    return vm->code[vm->reg[RPC - 1]];
}