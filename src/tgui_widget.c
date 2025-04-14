#include "../include/tgui_widget.h"
#include "../include/tgui_constraint.h"
#include <stdlib.h>
#include <string.h>

void tgui_widget_init(tgui_widget_t* widget, uint16_t id) {
    if (widget == NULL) return;
    
    widget->id = id;
    widget->x = 0;
    widget->y = 0;
    widget->width = 0;
    widget->height = 0;
    
    // 初始化约束系统
    widget->constraint_count = 0;
    memset(widget->constraints, 0, sizeof(widget->constraints));
    
    // 设置默认绘制函数为NULL，由子类覆盖
    widget->draw = NULL;
    widget->update_layout = NULL;
    
    widget->children = NULL;
    widget->next = NULL;
}

void tgui_widget_add_child(tgui_widget_t* parent, tgui_widget_t* child) {
    if (parent == NULL || child == NULL) return;
    
    // 添加到子控件链表末尾
    tgui_widget_t** p = &parent->children;
    while (*p != NULL) {
        p = &(*p)->next;
    }
    *p = child;
    child->next = NULL;
}

void tgui_widget_add_constraint(tgui_widget_t* widget, 
                              const tgui_constraint_t* constraint) {
    if (widget == NULL || constraint == NULL || 
        widget->constraint_count >= 8) {
        return;
    }
    
    widget->constraints[widget->constraint_count++] = *constraint;
    
    // TODO: 触发布局更新
}