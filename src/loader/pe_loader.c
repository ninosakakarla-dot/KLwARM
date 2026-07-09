
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "anwr/pe_loader.h"

/**
 * anwr_load_pe: Carga y analiza un archivo PE básico.
 * @path: Ruta al archivo .exe o .dll
 * 
 * Esta función es el punto de entrada inicial para el cargador.
 * En esta fase, solo realiza el análisis de las cabeceras.
 */
int anwr_load_pe(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "[ANWR] Error: No se pudo abrir el archivo %s\n", path);
        return -1;
    }

    IMAGE_DOS_HEADER dos_header;
    if (fread(&dos_header, sizeof(IMAGE_DOS_HEADER), 1, file) != 1) {
        fprintf(stderr, "[ANWR] Error: No se pudo leer el encabezado DOS\n");
        fclose(file);
        return -1;
    }

    // Verificar firma MZ
    if (dos_header.e_magic != IMAGE_DOS_SIGNATURE) {
        fprintf(stderr, "[ANWR] Error: Firma DOS inválida (no es un archivo ejecutable de Windows)\n");
        fclose(file);
        return -1;
    }

    printf("[ANWR] Encabezado DOS válido detectado.\n");
    printf("[ANWR] Offset a encabezado NT: 0x%X\n", dos_header.e_lfanew);

    // Moverse al encabezado NT
    if (fseek(file, dos_header.e_lfanew, SEEK_SET) != 0) {
        fprintf(stderr, "[ANWR] Error: No se pudo encontrar el encabezado NT\n");
        fclose(file);
        return -1;
    }

    uint32_t nt_signature;
    if (fread(&nt_signature, sizeof(uint32_t), 1, file) != 1) {
        fprintf(stderr, "[ANWR] Error: No se pudo leer la firma NT\n");
        fclose(file);
        return -1;
    }

    // Verificar firma PE
    if (nt_signature != IMAGE_NT_SIGNATURE) {
        fprintf(stderr, "[ANWR] Error: Firma PE inválida\n");
        fclose(file);
        return -1;
    }

    printf("[ANWR] Firma PE válida detectada.\n");

    IMAGE_FILE_HEADER file_header;
    if (fread(&file_header, sizeof(IMAGE_FILE_HEADER), 1, file) != 1) {
        fprintf(stderr, "[ANWR] Error: No se pudo leer el File Header\n");
        fclose(file);
        return -1;
    }

    printf("[ANWR] Arquitectura detectada: 0x%X\n", file_header.Machine);
    printf("[ANWR] Número de secciones: %d\n", file_header.NumberOfSections);

    // En futuras fases, aquí se cargarán las secciones y se realizará la reubicación.
    
    fclose(file);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <archivo_pe>\n", argv[0]);
        return 1;
    }

    printf("[ANWR] Iniciando motor de carga nativo...\n");
    return anwr_load_pe(argv[1]);
}
