#ifndef TGUI_CONSTRAINT_H
#define TGUI_CONSTRAINT_H

#include "tgui_types.h"

// 前向声明
typedef struct tgui_widget tgui_widget_t;

// 约束类型
typedef enum {
    TGUI_CONSTRAINT_FIXED,      // 固定约束
    TGUI_CONSTRAINT_FLEXIBLE,   // 弹性约束
    TGUI_CONSTRAINT_PERCENT,    // 百分比约束
    TGUI_CONSTRAINT_ASPECT      // 宽高比约束
} tgui_constraint_type_t;

// 约束条件
typedef struct {
    tgui_id_t target_id;          // 目标控件ID
    tgui_edge_t target_edge;      // 目标边
    tgui_edge_t self_edge;        // 自身边
    tgui_constraint_type_t type;  // 约束类型
    union {
        int16_t fixed_value;      // 固定值
        struct {
            int16_t min_value;    // 最小值(弹性约束)
            int16_t max_value;    // 最大值(弹性约束)
            float weight;         // 权重(0-1)
        } flex;
        float percent;            // 百分比(0-1)
        float aspect_ratio;       // 宽高比
    } value;
    tgui_angle_constraint_t angle_type;  // 角度约束
    uint16_t custom_angle;       // 自定义角度(0-360)
} tgui_constraint_t;

// 约束求解器
typedef struct {
    tgui_constraint_t* constraints;
    uint16_t count;
} tgui_constraint_solver_t;

// 初始化约束求解器
void tgui_constraint_solver_init(tgui_constraint_solver_t* solver);

// 添加约束条件
void tgui_constraint_solver_add(tgui_constraint_solver_t* solver, 
                              const tgui_constraint_t* constraint);

// 求解布局
void tgui_constraint_solver_solve(tgui_constraint_solver_t* solver,
                                tgui_widget_t* widgets,
                                uint16_t widget_count);

#endif // TGUI_CONSTRAINT_H