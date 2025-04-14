#ifndef TGUI_TYPES_H
#define TGUI_TYPES_H

#include <stdint.h>

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

#endif // TGUI_TYPES_H