
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "anwr/cpu_state.h"
#include "anwr/decoder.h"

// Prototipo del motor JIT
void anwr_translate_instruction(CPU_STATE *cpu, DECODED_INST *x86_inst);

#define JIT_BUFFER_SIZE (4 * 1024 * 1024) // 4MB para pruebas iniciales

void anwr_cpu_init(CPU_STATE *cpu, uint64_t entry_point, uint64_t stack_base) {
    memset(cpu, 0, sizeof(CPU_STATE));
    cpu->regs.rip = entry_point;
    cpu->regs.rsp = stack_base;

    // Reservar memoria RWX para el búfer JIT
    cpu->jit_buffer = mmap(NULL, JIT_BUFFER_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, 
                           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    cpu->jit_size = JIT_BUFFER_SIZE;
    cpu->jit_ptr = 0;

    printf("[ANWR-CPU] Inicializada en RIP: 0x%lX | RSP: 0x%lX\n", entry_point, stack_base);
    printf("[ANWR-CPU] Búfer JIT (RWX) asignado en %p\n", cpu->jit_buffer);
}

/**
 * anwr_fetch_instruction: Obtiene el siguiente opcode de la memoria.
 */
uint8_t anwr_fetch_instruction(CPU_STATE *cpu) {
    uint8_t *code_ptr = (uint8_t *)cpu->regs.rip;
    return *code_ptr;
}

/**
 * anwr_cpu_execute: Ciclo principal de ejecución (Fetch -> Decode -> Execute/Translate)
 */
void anwr_cpu_execute(CPU_STATE *cpu) {
    printf("[ANWR-CPU] Iniciando ciclo de ejecución con Decodificador...\n");

    int limit = 20; 
    while (limit--) {
        DECODED_INST inst;
        anwr_decode((uint8_t *)cpu->regs.rip, &inst);
        
        printf("[ANWR-CPU] RIP: 0x%016lX | Opcode: 0x%02X | Tipo: %d | Tamaño: %d\n", 
               cpu->regs.rip, inst.opcode, inst.type, inst.size);

        if (inst.type == INST_INT3) {
            printf("[ANWR-CPU] Breakpoint detectado. Deteniendo.\n");
            break;
        }

        if (inst.type == INST_UNKNOWN) {
            printf("[ANWR-CPU] Instrucción desconocida (0x%02X). Abortando.\n", inst.opcode);
            break;
        }

        // Intentar traducir la instrucción al búfer JIT
        anwr_translate_instruction(cpu, &inst);

        // Avanzar RIP según el tamaño real de la instrucción decodificada
        cpu->regs.rip += inst.size; 
    }

    printf("[ANWR-CPU] Ejecución pausada.\n");
}
