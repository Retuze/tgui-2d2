#ifndef TGUI_WIDGET_H
#define TGUI_WIDGET_H

#include "tgui_types.h"
#include "tgui_constraint.h"

#ifdef __cplusplus
extern "C" {
#endif

// 前向声明
struct tgui_widget;
typedef struct tgui_widget tgui_widget_t;

// 控件基类
struct tgui_widget {
    tgui_id_t id;           // 控件ID
    int16_t x;             // X坐标
    int16_t y;             // Y坐标
    uint16_t width;        // 宽度
    uint16_t height;       // 高度
    struct tgui_widget* parent;  // 父控件
    struct tgui_widget* children;  // 子控件链表
    struct tgui_widget* next;      // 兄弟控件链表
    void (*draw)(struct tgui_widget*, tgui_screen_t*);  // 绘制函数
    void (*update_layout)(struct tgui_widget*);         // 布局更新函数
    tgui_constraint_t constraints[8];  // 约束数组
    uint8_t constraint_count;          // 约束数量
};

// 初始化控件
void tgui_widget_init(tgui_widget_t* widget, tgui_id_t id);

// 添加子控件
void tgui_widget_add_child(tgui_widget_t* parent, tgui_widget_t* child);

// 添加约束
void tgui_widget_add_constraint(tgui_widget_t* widget, const tgui_constraint_t* constraint);

#ifdef __cplusplus
}
#endif

#endif // TGUI_WIDGET_H