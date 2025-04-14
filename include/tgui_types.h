#ifndef TGUI_TYPES_H
#define TGUI_TYPES_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// 像素格式配置
#define TGUI_PIXEL_FORMAT_RGB565    0
#define TGUI_PIXEL_FORMAT_ARGB8888  1

// 选择当前使用的像素格式
#ifndef TGUI_CURRENT_PIXEL_FORMAT
#define TGUI_CURRENT_PIXEL_FORMAT   TGUI_PIXEL_FORMAT_ARGB8888
#endif

// 像素格式相关定义
#if TGUI_CURRENT_PIXEL_FORMAT == TGUI_PIXEL_FORMAT_ARGB8888
    typedef uint32_t tgui_pixel_t;
    #define TGUI_BYTES_PER_PIXEL    4
    #define TGUI_COLOR_RED          0xFFFF0000
    #define TGUI_COLOR_GREEN        0xFF00FF00
    #define TGUI_COLOR_BLUE         0xFF0000FF
    #define TGUI_COLOR_WHITE        0xFFFFFFFF
    #define TGUI_COLOR_BLACK        0xFF000000
    #define TGUI_COLOR_TRANSPARENT  0x00000000
#elif TGUI_CURRENT_PIXEL_FORMAT == TGUI_PIXEL_FORMAT_RGB565
    typedef uint16_t tgui_pixel_t;
    #define TGUI_BYTES_PER_PIXEL    2
    #define TGUI_COLOR_RED          0xF800
    #define TGUI_COLOR_GREEN        0x07E0
    #define TGUI_COLOR_BLUE         0x001F
    #define TGUI_COLOR_WHITE        0xFFFF
    #define TGUI_COLOR_BLACK        0x0000
    #define TGUI_COLOR_TRANSPARENT  0x0000
#else
    #error "Unsupported pixel format"
#endif

// 控件ID类型
typedef uint16_t tgui_id_t;

// 像素格式枚举
typedef enum {
    TGUI_PIXFMT_RGB565 = TGUI_PIXEL_FORMAT_RGB565,    // 16位RGB565
    TGUI_PIXFMT_ARGB8888 = TGUI_PIXEL_FORMAT_ARGB8888 // 32位ARGB
} tgui_pixel_format_t;

// 屏幕缓冲区描述结构体
typedef struct {
    uint16_t width;          // 逻辑宽度(像素)
    uint16_t height;         // 逻辑高度(像素) 
    uint16_t buffer_width;   // 物理宽度(可能包含padding)
    uint16_t buffer_height;  // 物理高度
    uint32_t buffer_size;    // 缓冲区大小(字节)
    tgui_pixel_format_t format; // 像素格式
    tgui_pixel_t* buffer;    // 缓冲区指针
    uint32_t stride;         // 每行字节数
} tgui_screen_t;

// 渲染结果枚举
typedef enum {
    TGUI_RENDER_OK,      // 渲染成功
    TGUI_RENDER_PARTIAL, // 部分渲染(脏矩形)
    TGUI_RENDER_ERROR    // 渲染错误
} tgui_render_result_t;

// 约束边类型
typedef enum {
    TGUI_EDGE_LEFT,
    TGUI_EDGE_RIGHT,
    TGUI_EDGE_TOP,
    TGUI_EDGE_BOTTOM,
    TGUI_EDGE_CENTER_X,
    TGUI_EDGE_CENTER_Y
} tgui_edge_t;

// 角度约束类型
typedef enum {
    TGUI_ANGLE_NONE,
    TGUI_ANGLE_PARALLEL,
    TGUI_ANGLE_PERPENDICULAR,
    TGUI_ANGLE_CUSTOM  // 自定义角度(0-360度)
} tgui_angle_constraint_t;

#ifdef __cplusplus
}
#endif

#endif // TGUI_TYPES_H