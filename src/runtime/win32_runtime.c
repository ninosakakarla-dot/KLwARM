
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
    // Simulación simple: devolver 0 si el nombre es NULL (módulo actual)
    printf("[ANWR-WIN32] GetModuleHandleA llamado.\n");
    cpu->regs.rax = 0; // Resultado en RAX
}

void anwr_win32_dispatch(const char *func_name, CPU_STATE *cpu) {
    if (strcmp(func_name, "ExitProcess") == 0) {
        anwr_kernel32_ExitProcess(cpu);
    } else if (strcmp(func_name, "GetModuleHandleA") == 0) {
        anwr_kernel32_GetModuleHandleA(cpu);
    } else {
        printf("[ANWR-WIN32] Advertencia: Función no implementada: %s\n", func_name);
    }
}
