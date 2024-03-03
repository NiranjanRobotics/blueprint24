#ifndef vm_h
#define vm_h

#include <stdarg.h>
#include "util.h"

enum {
    R0 = 0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
    R9,
    R10,
    R11,
    R12,
    RPC,
    RCOMP,
    NUM_REGISTERS,
};

typedef enum {
    OP_LDC,
    OP_MOV,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_CMP,
    OP_BNP,
    OP_BNN,
    OP_JMP,
    OP_NONE,
    OP_RET,
} OpCode;

enum {
    CMPNONE,
    CMPPOS,
    CMPNEG
};

typedef struct {
    uint16_t reg[NUM_REGISTERS];
    uint8_t code[16384];
    size_t size;
} VM;

void initVM(VM *vm);
void addInstruction(VM *vm, OpCode op_code, int args, ...);

#endif