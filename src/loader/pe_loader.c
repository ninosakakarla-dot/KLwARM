
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "anwr/pe_loader.h"

/**
 * anwr_get_mmap_prot: Convierte características PE a protecciones mmap.
 */
int anwr_get_mmap_prot(uint32_t characteristics) {
    int prot = 0;
    if (characteristics & IMAGE_SCN_MEM_READ) prot |= PROT_READ;
    if (characteristics & IMAGE_SCN_MEM_WRITE) prot |= PROT_WRITE;
    if (characteristics & IMAGE_SCN_MEM_EXECUTE) prot |= PROT_EXEC;
    return prot;
}

/**
 * anwr_load_pe: Carga y analiza un archivo PE, mapeando secciones en memoria.
 */
int anwr_load_pe(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "[ANWR] Error: No se pudo abrir el archivo %s\n", path);
        return -1;
    }

    IMAGE_DOS_HEADER dos_header;
    fread(&dos_header, sizeof(IMAGE_DOS_HEADER), 1, file);
    if (dos_header.e_magic != IMAGE_DOS_SIGNATURE) {
        fprintf(stderr, "[ANWR] Error: Firma DOS inválida\n");
        fclose(file);
        return -1;
    }

    fseek(file, dos_header.e_lfanew, SEEK_SET);
    IMAGE_NT_HEADERS nt_headers;
    if (fread(&nt_headers, sizeof(IMAGE_NT_HEADERS), 1, file) != 1) {
        fprintf(stderr, "[ANWR] Error: No se pudo leer el encabezado NT\n");
        fclose(file);
        return -1;
    }

    if (nt_headers.Signature != IMAGE_NT_SIGNATURE) {
        fprintf(stderr, "[ANWR] Error: Firma PE inválida\n");
        fclose(file);
        return -1;
    }

    printf("[ANWR] Analizando binario: %s\n", path);
    printf("[ANWR] ImageBase sugerida: 0x%lX, SizeOfImage: 0x%X\n", 
           nt_headers.OptionalHeader.ImageBase, nt_headers.OptionalHeader.SizeOfImage);

    // Intentar reservar el espacio total de la imagen en memoria virtual
    // Usamos MAP_PRIVATE | MAP_ANONYMOUS para reservar el rango completo primero.
    void *image_base = mmap((void*)nt_headers.OptionalHeader.ImageBase, 
                            nt_headers.OptionalHeader.SizeOfImage, 
                            PROT_READ | PROT_WRITE, 
                            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (image_base == MAP_FAILED) {
        // Si falló en la dirección sugerida, dejar que el sistema elija una
        image_base = mmap(NULL, nt_headers.OptionalHeader.SizeOfImage, 
                          PROT_READ | PROT_WRITE, 
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (image_base == MAP_FAILED) {
            fprintf(stderr, "[ANWR] Error crítico: No se pudo reservar memoria para la imagen\n");
            fclose(file);
            return -1;
        }
        printf("[ANWR] Advertencia: Cargado en dirección reubicada: %p\n", image_base);
    } else {
        printf("[ANWR] Imagen mapeada exitosamente en ImageBase: %p\n", image_base);
    }

    // Copiar encabezados a la memoria mapeada
    fseek(file, 0, SEEK_SET);
    fread(image_base, nt_headers.OptionalHeader.SizeOfHeaders, 1, file);

    // Cargar secciones
    IMAGE_SECTION_HEADER *sections = malloc(sizeof(IMAGE_SECTION_HEADER) * nt_headers.FileHeader.NumberOfSections);
    fseek(file, dos_header.e_lfanew + sizeof(IMAGE_NT_HEADERS), SEEK_SET);
    fread(sections, sizeof(IMAGE_SECTION_HEADER), nt_headers.FileHeader.NumberOfSections, file);

    for (int i = 0; i < nt_headers.FileHeader.NumberOfSections; i++) {
        void *section_dest = (char*)image_base + sections[i].VirtualAddress;
        
        if (sections[i].SizeOfRawData > 0) {
            fseek(file, sections[i].PointerToRawData, SEEK_SET);
            fread(section_dest, sections[i].SizeOfRawData, 1, file);
        }

        // Aplicar protecciones de memoria nativas de la sección
        int prot = anwr_get_mmap_prot(sections[i].Characteristics);
        mprotect(section_dest, sections[i].Misc.VirtualSize, prot);

        printf("[ANWR] Sección cargada: %-8s | RVA: 0x%08X | Tamaño: 0x%08X | Prot: 0x%X\n", 
               sections[i].Name, sections[i].VirtualAddress, sections[i].Misc.VirtualSize, prot);
    }

    free(sections);
    fclose(file);
    
    printf("[ANWR] Carga de secciones completada.\n");

    // Escanear Tabla de Importación
    uint32_t import_rva = nt_headers.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    if (import_rva != 0) {
        printf("[ANWR] Analizando Tabla de Importación (IAT)...\n");
        IMAGE_IMPORT_DESCRIPTOR *import_desc = (IMAGE_IMPORT_DESCRIPTOR*)((char*)image_base + import_rva);
        
        while (import_desc->Name != 0) {
            char *dll_name = (char*)image_base + import_desc->Name;
            printf("[ANWR] Dependencia detectada: %s\n", dll_name);
            // En la Fase 2, aquí cargaremos las DLLs nativas de ANWR
            import_desc++;
        }
    }

    printf("[ANWR] EntryPoint (RVA): 0x%X -> Dirección: %p\n", 
           nt_headers.OptionalHeader.AddressOfEntryPoint, 
           (char*)image_base + nt_headers.OptionalHeader.AddressOfEntryPoint);

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <archivo_pe>\n", argv[0]);
        return 1;
    }

    printf("[ANWR] Iniciando motor de carga nativo...\n");
    if (anwr_load_pe(argv[1]) == 0) {
        printf("[ANWR] Fase de carga de memoria exitosa.\n");
    } else {
        printf("[ANWR] Fallo en la carga del binario.\n");
        return 1;
    }
    return 0;
}
