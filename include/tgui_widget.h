#ifndef TGUI_WIDGET_H
#define TGUI_WIDGET_H

#include "tgui_core.h"
#include "tgui_constraint.h"

#ifdef __cplusplus
extern "C" {
#endif

// 控件基类
typedef struct tgui_widget {
    uint16_t id;         // 控件唯一ID
    uint16_t x;          // X坐标
    uint16_t y;          // Y坐标
    uint16_t width;      // 宽度
    uint16_t height;     // 高度
    
    // 约束系统
    tgui_constraint_t constraints[8]; // 最大8个约束
    uint8_t constraint_count;
    
    // 虚函数表
    void (*draw)(struct tgui_widget* self, tgui_screen_t* screen);  // 绘制函数
    void (*update_layout)(struct tgui_widget* self);  // 更新布局函数
    
    // 子控件链表
    struct tgui_widget* children;
    struct tgui_widget* next;
} tgui_widget_t;

// 初始化控件
void tgui_widget_init(tgui_widget_t* widget, uint16_t id);

// 添加子控件
void tgui_widget_add_child(tgui_widget_t* parent, tgui_widget_t* child);

// 添加约束条件
void tgui_widget_add_constraint(tgui_widget_t* widget, 
                              const tgui_constraint_t* constraint);

#ifdef __cplusplus
}
#endif

#endif // TGUI_WIDGET_H