#include "../include/tgui_widget.h"
#include "../include/tgui_constraint.h"
#include "../include/tgui_core.h"
#include <stdlib.h>
#include <string.h>

// 声明外部函数
extern void tgui_update_widget_count(void);

void tgui_widget_init(tgui_widget_t* widget, uint16_t id) {
    if (widget == NULL) return;
    
    widget->id = id;
    widget->x = 0;
    widget->y = 0;
    widget->width = 0;
    widget->height = 0;
    widget->parent = NULL;
    widget->children = NULL;
    widget->next = NULL;
    widget->draw = NULL;
    widget->update_layout = NULL;
    widget->constraint_count = 0;
    
    // 初始化约束系统
    memset(widget->constraints, 0, sizeof(widget->constraints));
}

void tgui_widget_add_child(tgui_widget_t* parent, tgui_widget_t* child) {
    if (parent == NULL || child == NULL) return;
    
    // 设置父子关系
    child->parent = parent;
    
    // 将子控件添加到链表头部
    child->next = parent->children;
    parent->children = child;
    
    // 更新控件计数
    tgui_update_widget_count();
}

void tgui_widget_add_constraint(tgui_widget_t* widget, 
                              const tgui_constraint_t* constraint) {
    if (widget == NULL || constraint == NULL || 
        widget->constraint_count >= sizeof(widget->constraints)/sizeof(widget->constraints[0])) {
        return;
    }
    
    widget->constraints[widget->constraint_count++] = *constraint;
    
    // TODO: 触发布局更新
}