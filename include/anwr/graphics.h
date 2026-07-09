
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

// Vulkan Bridge
void anwr_vulkan_create_device();
void anwr_vulkan_submit_command(void *cmd_buffer);
void anwr_vulkan_present();

// Gestión de Recursos GPU
void* anwr_graphics_create_texture(uint32_t width, uint32_t height, uint32_t format);
void* anwr_graphics_compile_shader(const char *source, uint32_t type);

#endif // ANWR_GRAPHICS_H
