
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "anwr/win32_runtime.h"

void anwr_kernel32_ExitProcess(CPU_STATE *cpu) {
    uint32_t exit_code = (uint32_t)cpu->regs.rcx; // En x64, el primer argumento va en RCX
    printf("[ANWR-WIN32] ExitProcess llamado con código: %d\n", exit_code);
    printf("[ANWR] Finalizando ejecución del programa Windows.\n");
    exit(exit_code);
}

void anwr_kernel32_GetModuleHandleA(CPU_STATE *cpu) {
    printf("[ANWR-WIN32] GetModuleHandleA llamado.\n");
    cpu->regs.rax = 0; 
}

void anwr_kernel32_GetProcessHeap(CPU_STATE *cpu) {
    printf("[ANWR-WIN32] GetProcessHeap llamado.\n");
    cpu->regs.rax = 0x12345678; // Handle de heap ficticio
}

void anwr_kernel32_HeapAlloc(CPU_STATE *cpu) {
    uint32_t size = (uint32_t)cpu->regs.r8; // 3er arg en x64
    void *ptr = malloc(size);
    printf("[ANWR-WIN32] HeapAlloc: Asignados %d bytes en %p\n", size, ptr);
    cpu->regs.rax = (uint64_t)ptr;
}

void anwr_kernel32_HeapFree(CPU_STATE *cpu) {
    void *ptr = (void *)cpu->regs.r8;
    printf("[ANWR-WIN32] HeapFree: Liberando memoria en %p\n", ptr);
    free(ptr);
    cpu->regs.rax = 1; // TRUE
}

void anwr_win32_dispatch(const char *func_name, CPU_STATE *cpu) {
    if (strcmp(func_name, "ExitProcess") == 0) {
        anwr_kernel32_ExitProcess(cpu);
    } else if (strcmp(func_name, "GetModuleHandleA") == 0) {
        anwr_kernel32_GetModuleHandleA(cpu);
    } else if (strcmp(func_name, "GetProcessHeap") == 0) {
        anwr_kernel32_GetProcessHeap(cpu);
    } else if (strcmp(func_name, "HeapAlloc") == 0) {
        anwr_kernel32_HeapAlloc(cpu);
    } else if (strcmp(func_name, "HeapFree") == 0) {
        anwr_kernel32_HeapFree(cpu);
    } else {
        printf("[ANWR-WIN32] Advertencia: Función no implementada: %s\n", func_name);
    }
}
