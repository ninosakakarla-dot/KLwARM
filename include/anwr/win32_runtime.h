
#ifndef ANWR_WIN32_RUNTIME_H
#define ANWR_WIN32_RUNTIME_H

#include <stdint.h>
#include "anwr/cpu_state.h"

/**
 * anwr_win32_dispatch: Despacha una llamada al sistema Win32 interceptada.
 * @func_name: Nombre de la función (ej: "ExitProcess")
 * @cpu: Estado actual de la CPU para acceder a argumentos
 */
void anwr_win32_dispatch(const char *func_name, CPU_STATE *cpu);

// Definiciones de funciones de kernel32 emuladas
void anwr_kernel32_ExitProcess(CPU_STATE *cpu);
void anwr_kernel32_GetModuleHandleA(CPU_STATE *cpu);

// Gestión de Heap
void anwr_kernel32_HeapAlloc(CPU_STATE *cpu);
void anwr_kernel32_HeapFree(CPU_STATE *cpu);
void anwr_kernel32_GetProcessHeap(CPU_STATE *cpu);

// Gestión de Archivos
void anwr_kernel32_CreateFileA(CPU_STATE *cpu);
void anwr_kernel32_ReadFile(CPU_STATE *cpu);
void anwr_kernel32_WriteFile(CPU_STATE *cpu);
void anwr_kernel32_CloseHandle(CPU_STATE *cpu);

// Hilos y Sincronización
void anwr_kernel32_CreateThread(CPU_STATE *cpu);
void anwr_kernel32_Sleep(CPU_STATE *cpu);
void anwr_kernel32_WaitForSingleObject(CPU_STATE *cpu);

#endif // ANWR_WIN32_RUNTIME_H
