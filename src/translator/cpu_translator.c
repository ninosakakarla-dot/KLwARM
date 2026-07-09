
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "anwr/cpu_state.h"

void anwr_cpu_init(CPU_STATE *cpu, uint64_t entry_point, uint64_t stack_base) {
    memset(cpu, 0, sizeof(CPU_STATE));
    cpu->regs.rip = entry_point;
    cpu->regs.rsp = stack_base;
    printf("[ANWR-CPU] Inicializada en RIP: 0x%lX | RSP: 0x%lX\n", entry_point, stack_base);
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
    printf("[ANWR-CPU] Iniciando ciclo de ejecución...\n");

    // Por ahora, implementamos un bucle de interpretación muy básico
    // para demostrar la capacidad de "recorrer" el código.
    int limit = 10; // Límite de instrucciones para la prueba inicial
    while (limit--) {
        uint8_t opcode = anwr_fetch_instruction(cpu);
        
        printf("[ANWR-CPU] RIP: 0x%lX | Opcode: 0x%02X\n", cpu->regs.rip, opcode);

        // Aquí vendría la lógica de decodificación y traducción a ARM64.
        // Como ejemplo, simplemente avanzamos el RIP.
        // En una implementación real, el avance depende del tamaño de la instrucción x86.
        cpu->regs.rip += 1; 

        if (opcode == 0xCC) { // INT3 (Breakpoint)
            printf("[ANWR-CPU] Breakpoint detectado. Deteniendo.\n");
            break;
        }
    }

    printf("[ANWR-CPU] Ejecución pausada (Límite de instrucciones alcanzado).\n");
}
