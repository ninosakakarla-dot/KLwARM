
#ifndef ANWR_GRAPHICS_H
#define ANWR_GRAPHICS_H

#include <stdint.h>

/**
 * anwr_graphics_init: Inicializa el subsistema gráfico.
 * Prepara la integración con Vulkan y ANativeWindow.
 */
void anwr_graphics_init();

/**
 * anwr_graphics_create_window: Crea una ventana nativa de Android para la app Windows.
 */
void* anwr_graphics_create_window(uint32_t width, uint32_t height);

/**
 * anwr_graphics_swap_buffers: Realiza el intercambio de búferes (V-Sync).
 */
void anwr_graphics_swap_buffers();

#endif // ANWR_GRAPHICS_H
