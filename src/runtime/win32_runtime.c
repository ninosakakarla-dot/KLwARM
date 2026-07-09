
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

// Auxiliar para traducir rutas C:\... a /sdcard/... o similar
void anwr_translate_path(const char *win_path, char *android_path) {
    // Simulación simple: mapear C:\ a /home/ubuntu/anwr_root/
    if (strncmp(win_path, "C:\\", 3) == 0) {
        sprintf(android_path, "/home/ubuntu/anwr_root/%s", win_path + 3);
    } else {
        strcpy(android_path, win_path);
    }
    // Cambiar \ por /
    for (int i = 0; android_path[i]; i++) {
        if (android_path[i] == '\\') android_path[i] = '/';
    }
}

void anwr_kernel32_CreateFileA(CPU_STATE *cpu) {
    const char *win_path = (const char *)cpu->regs.rcx;
    char android_path[512];
    anwr_translate_path(win_path, android_path);
    
    printf("[ANWR-FS] CreateFileA: %s -> %s\n", win_path, android_path);
    
    FILE *f = fopen(android_path, "rb+");
    if (!f) f = fopen(android_path, "wb+"); // Crear si no existe
    
    cpu->regs.rax = (uint64_t)f;
}

void anwr_kernel32_ReadFile(CPU_STATE *cpu) {
    FILE *f = (FILE *)cpu->regs.rcx;
    void *buffer = (void *)cpu->regs.rdx;
    uint32_t size = (uint32_t)cpu->regs.r8;
    
    size_t read = fread(buffer, 1, size, f);
    printf("[ANWR-FS] ReadFile: Leídos %zu bytes\n", read);
    cpu->regs.rax = 1; // Success
}

void anwr_kernel32_WriteFile(CPU_STATE *cpu) {
    FILE *f = (FILE *)cpu->regs.rcx;
    const void *buffer = (const void *)cpu->regs.rdx;
    uint32_t size = (uint32_t)cpu->regs.r8;
    
    size_t written = fwrite(buffer, 1, size, f);
    printf("[ANWR-FS] WriteFile: Escritos %zu bytes\n", written);
    cpu->regs.rax = 1; // Success
}

void anwr_kernel32_CloseHandle(CPU_STATE *cpu) {
    FILE *f = (FILE *)cpu->regs.rcx;
    if (f) fclose(f);
    cpu->regs.rax = 1;
}

void anwr_kernel32_Sleep(CPU_STATE *cpu) {
    uint32_t ms = (uint32_t)cpu->regs.rcx;
    printf("[ANWR-SYS] Sleep: %d ms\n", ms);
    usleep(ms * 1000);
}

void anwr_kernel32_CreateThread(CPU_STATE *cpu) {
    uint64_t start_addr = cpu->regs.r8;
    printf("[ANWR-SYS] CreateThread: Nueva ejecución en 0x%lX\n", start_addr);
    // En el futuro, aquí lanzaremos un nuevo hilo nativo de Linux
    cpu->regs.rax = 0x5555; // Handle de hilo ficticio
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
    } else if (strcmp(func_name, "CreateFileA") == 0) {
        anwr_kernel32_CreateFileA(cpu);
    } else if (strcmp(func_name, "ReadFile") == 0) {
        anwr_kernel32_ReadFile(cpu);
    } else if (strcmp(func_name, "WriteFile") == 0) {
        anwr_kernel32_WriteFile(cpu);
    } else if (strcmp(func_name, "CloseHandle") == 0) {
        anwr_kernel32_CloseHandle(cpu);
    } else if (strcmp(func_name, "Sleep") == 0) {
        anwr_kernel32_Sleep(cpu);
    } else if (strcmp(func_name, "CreateThread") == 0) {
        anwr_kernel32_CreateThread(cpu);
    } else {
        printf("[ANWR-WIN32] Advertencia: Función no implementada: %s\n", func_name);
    }
}
