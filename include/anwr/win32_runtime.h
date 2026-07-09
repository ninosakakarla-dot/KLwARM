
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

#endif // ANWR_WIN32_RUNTIME_H
