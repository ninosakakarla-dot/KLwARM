
#ifndef ANWR_CPU_STATE_H
#define ANWR_CPU_STATE_H

#include <stdint.h>
#include <sys/mman.h>

// Estado de los registros x86_64
typedef struct _X86_64_REGS {
    uint64_t rax, rbx, rcx, rdx;
    uint64_t rsi, rdi, rbp, rsp;
    uint64_t r8,  r9,  r10, r11;
    uint64_t r12, r13, r14, r15;
    uint64_t rip;    // Instruction Pointer
    uint64_t rflags; // Flags
} X86_64_REGS;

// Estado completo de la CPU emulada
typedef struct _CPU_STATE {
    X86_64_REGS regs;
    void *jit_buffer;     // Búfer para código ARM64 traducido
    uint32_t jit_size;
    uint32_t jit_ptr;     // Puntero actual en el búfer JIT
} CPU_STATE;

/**
 * anwr_cpu_init: Inicializa el estado de la CPU.
 */
void anwr_cpu_init(CPU_STATE *cpu, uint64_t entry_point, uint64_t stack_base);

/**
 * anwr_cpu_execute: Inicia el ciclo de ejecución/traducción.
 */
void anwr_cpu_execute(CPU_STATE *cpu);

#endif // ANWR_CPU_STATE_H
