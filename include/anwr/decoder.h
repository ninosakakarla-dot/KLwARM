
#ifndef ANWR_DECODER_H
#define ANWR_DECODER_H

#include <stdint.h>

// Tipos de instrucciones soportadas inicialmente
typedef enum _INST_TYPE {
    INST_UNKNOWN,
    INST_MOV,
    INST_PUSH,
    INST_POP,
    INST_CALL,
    INST_RET,
    INST_INT3,
    INST_NOP
} INST_TYPE;

// Estructura de una instrucción decodificada
typedef struct _DECODED_INST {
    INST_TYPE type;
    uint8_t size;       // Tamaño en bytes de la instrucción original
    uint8_t opcode;
    uint8_t modrm;      // Byte ModR/M si existe
    int64_t immediate;  // Valor inmediato si existe
} DECODED_INST;

/**
 * anwr_decode: Decodifica una instrucción x86_64 desde la memoria.
 */
void anwr_decode(const uint8_t *code, DECODED_INST *inst);

#endif // ANWR_DECODER_H
