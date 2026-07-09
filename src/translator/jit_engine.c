
#include <stdio.h>
#include <stdint.h>
#include "anwr/cpu_state.h"
#include "anwr/decoder.h"

/**
 * anwr_emit_arm64: Escribe una instrucción ARM64 en el búfer JIT.
 */
void anwr_emit_arm64(CPU_STATE *cpu, uint32_t arm_inst) {
    if (cpu->jit_ptr + 4 <= cpu->jit_size) {
        uint32_t *dest = (uint32_t *)((char *)cpu->jit_buffer + cpu->jit_ptr);
        *dest = arm_inst;
        cpu->jit_ptr += 4;
    }
}

/**
 * anwr_translate_instruction: Traduce una instrucción x86_64 decodificada a ARM64.
 */
void anwr_translate_instruction(CPU_STATE *cpu, DECODED_INST *x86_inst) {
    switch (x86_inst->type) {
        case INST_NOP:
            // ARM64 NOP: 0xD503201F
            anwr_emit_arm64(cpu, 0xD503201F);
            break;
        case INST_RET:
            // ARM64 RET: 0xD65F03C0
            anwr_emit_arm64(cpu, 0xD65F03C0);
            break;
        default:
            // Por ahora, otras instrucciones se manejarán mediante interpretación
            // o se añadirán aquí en el futuro.
            break;
    }
}
