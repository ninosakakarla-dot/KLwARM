
#ifndef ANWR_PE_LOADER_H
#define ANWR_PE_LOADER_H

#include <stdint.h>

// Definiciones para el PE Loader

// Estructura del encabezado DOS
typedef struct _IMAGE_DOS_HEADER {
    uint16_t e_magic;    // Magic number
    uint16_t e_cblp;     // Bytes on last page of file
    uint16_t e_cp;       // Pages in file
    uint16_t e_crlc;     // Relocations
    uint16_t e_cparhdr;  // Size of header in paragraphs
    uint16_t e_minalloc; // Minimum extra paragraphs needed
    uint16_t e_maxalloc; // Maximum extra paragraphs needed
    uint16_t e_ss;       // Initial (relative) SS value
    uint16_t e_sp;       // Initial SP value
    uint16_t e_csum;     // Checksum
    uint16_t e_ip;       // Initial IP value
    uint16_t e_cs;       // Initial (relative) CS value
    uint16_t e_lfarlc;   // File address of relocation table
    uint16_t e_ovno;     // Overlay number
    uint16_t e_oemid;    // OEM identifier (for e_oeminfo)
    uint16_t e_oeminfo;  // OEM information; e_oemid specific
    uint16_t e_res2[10]; // Reserved words
    uint32_t e_lfanew;   // File address of new exe header
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

// Estructura del encabezado de archivo PE (parte de IMAGE_NT_HEADERS)
typedef struct _IMAGE_FILE_HEADER {
    uint16_t Machine;
    uint16_t NumberOfSections;
    uint32_t TimeDateStamp;
    uint32_t PointerToSymbolTable;
    uint32_t NumberOfSymbols;
    uint16_t SizeOfOptionalHeader;
    uint16_t Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

// Estructura del encabezado opcional PE (parte de IMAGE_NT_HEADERS)
// Esta es una versión simplificada para 32 bits (PE32) o 64 bits (PE32+)
// Solo se incluyen los campos más relevantes para la carga inicial.
typedef struct _IMAGE_OPTIONAL_HEADER {
    uint16_t Magic;
    uint8_t  MajorLinkerVersion;
    uint8_t  MinorLinkerVersion;
    uint32_t SizeOfCode;
    uint32_t SizeOfInitializedData;
    uint32_t SizeOfUninitializedData;
    uint32_t AddressOfEntryPoint;
    uint32_t BaseOfCode;
    // PE32+ tiene BaseOfData, pero no es universalmente requerido para la carga inicial
    // y puede variar. Para simplificar, nos enfocamos en los campos comunes.
    uint64_t ImageBase; // uint32_t para PE32, uint64_t para PE32+
    uint32_t SectionAlignment;
    uint32_t FileAlignment;
    uint16_t MajorOperatingSystemVersion;
    uint16_t MinorOperatingSystemVersion;
    uint16_t MajorImageVersion;
    uint16_t MinorImageVersion;
    uint16_t MajorSubsystemVersion;
    uint16_t MinorSubsystemVersion;
    uint32_t Win32VersionValue;
    uint32_t SizeOfImage;
    uint32_t SizeOfHeaders;
    uint32_t CheckSum;
    uint16_t Subsystem;
    uint16_t DllCharacteristics;
    uint64_t SizeOfStackReserve; // uint32_t para PE32, uint64_t para PE32+
    uint64_t SizeOfStackCommit;  // uint32_t para PE32, uint64_t para PE32+
    uint64_t SizeOfHeapReserve;  // uint32_t para PE32, uint64_t para PE32+
    uint64_t SizeOfHeapCommit;   // uint32_t para PE32, uint64_t para PE32+
    uint32_t LoaderFlags;
    uint32_t NumberOfRvaAndSizes;
    // IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER, *PIMAGE_OPTIONAL_HEADER;

// Estructura del encabezado NT (PE)
typedef struct _IMAGE_NT_HEADERS {
    uint32_t Signature; // "PE\0\0"
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER OptionalHeader;
} IMAGE_NT_HEADERS, *PIMAGE_NT_HEADERS;

// Estructura de la cabecera de sección
typedef struct _IMAGE_SECTION_HEADER {
    uint8_t  Name[8];
    union {
        uint32_t PhysicalAddress;
        uint32_t VirtualSize;
    } Misc;
    uint32_t VirtualAddress;
    uint32_t SizeOfRawData;
    uint32_t PointerToRawData;
    uint32_t PointerToLinenumbers;
    uint16_t NumberOfRelocations;
    uint16_t NumberOfLinenumbers;
    uint32_t Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

// Constantes para los magic numbers
#define IMAGE_DOS_SIGNATURE  0x5A4D     // MZ
#define IMAGE_NT_SIGNATURE   0x00004550 // PE\0\0

#endif // ANWR_PE_LOADER_H
