
cmake_minimum_required(VERSION 3.10)
project(ANWR VERSION 0.1.0 LANGUAGES C)

# Configuración de estándares de C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Directorios de inclusión
include_directories(include)

# Definición del ejecutable principal (el motor ELF nativo)
add_executable(anwr_core 
    src/loader/pe_loader.c
    src/translator/cpu_translator.c
    src/translator/decoder.c
    src/translator/jit_engine.c
)

# Opciones de compilación para máxima optimización y depuración
target_compile_options(anwr_core PRIVATE -Wall -Wextra -O3 -g)

# Instalación (opcional por ahora)
install(TARGETS anwr_core DESTINATION bin)
