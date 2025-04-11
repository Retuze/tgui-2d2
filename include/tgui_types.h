#ifndef TGUI_TYPES_H
#define TGUI_TYPES_H

#include <stdint.h>

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