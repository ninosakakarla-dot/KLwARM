
#include <stdio.h>
#include "anwr/decoder.h"

void anwr_decode(const uint8_t *code, DECODED_INST *inst) {
    uint8_t opcode = code[0];
    inst->opcode = opcode;
    inst->size = 1;
    inst->type = INST_UNKNOWN;

    switch (opcode) {
        case 0x90: // NOP
            inst->type = INST_NOP;
            break;
        case 0xCC: // INT3
            inst->type = INST_INT3;
            break;
        case 0xC3: // RET
            inst->type = INST_RET;
            break;
        case 0xE8: // CALL rel32
            inst->type = INST_CALL;
            inst->size = 5;
            // El desplazamiento está en los siguientes 4 bytes
            break;
        case 0x50: case 0x51: case 0x52: case 0x53:
        case 0x54: case 0x55: case 0x56: case 0x57: // PUSH reg
            inst->type = INST_PUSH;
            break;
        case 0x58: case 0x59: case 0x5A: case 0x5B:
        case 0x5C: case 0x5D: case 0x5E: case 0x5F: // POP reg
            inst->type = INST_POP;
            break;
        case 0xB8: case 0xB9: case 0xBA: case 0xBB:
        case 0xBC: case 0xBD: case 0xBE: case 0xBF: // MOV reg, imm32
            inst->type = INST_MOV;
            inst->size = 5;
            break;
    }
}
