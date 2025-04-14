#ifndef TGUI_CORE_H
#define TGUI_CORE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "tgui_types.h"
#include "tgui_result.h"

#ifdef __cplusplus
extern "C" {
#endif

// Platform abstraction interface
typedef struct {
    // Initialize platform
    tgui_result_t (*init)(tgui_screen_t* screen); // Pass in desired screen parameters
    // Render screen
    tgui_render_result_t (*render)(const tgui_screen_t* screen);
    // Get input events
    tgui_result_t (*get_event)(void* event);
} tgui_platform_t;

// Forward declaration
struct tgui_widget;
typedef struct tgui_widget tgui_widget_t;

// Initialize GUI system
tgui_result_t tgui_init(const tgui_platform_t* platform, 
                      uint16_t width, uint16_t height,
                      tgui_pixel_format_t format,
                      void* buffer);

// Render GUI
tgui_render_result_t tgui_render(void);

// Get current screen buffer
const tgui_screen_t* tgui_get_screen(void);

// Get root widget
tgui_widget_t* tgui_get_root_widget(void);

// Get total widget count
uint16_t tgui_get_widget_count(void);

// Update widget count (internal use)
void tgui_update_widget_count(void);

// Trigger re-layout
void tgui_trigger_layout(void);

// Cleanup resources
void tgui_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif // TGUI_CORE_H