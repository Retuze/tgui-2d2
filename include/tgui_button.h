#ifndef TGUI_BUTTON_H
#define TGUI_BUTTON_H

#include "tgui_widget.h"
#include "tgui_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// 前向声明
typedef struct tgui_button tgui_button_t;

// 按钮控件结构体
struct tgui_button {
    tgui_widget_t base;      // 继承自基础控件
    char* text;              // 按钮文本（动态分配）
    tgui_pixel_t bg_color;   // 背景色
    tgui_pixel_t text_color; // 文字颜色
    void (*on_click)(struct tgui_button*);
};

// 初始化按钮
void tgui_button_init(tgui_button_t* button, tgui_id_t id, const char* text);

// 设置按钮颜色
void tgui_button_set_colors(tgui_button_t* button, tgui_pixel_t bg_color, tgui_pixel_t text_color);

// 释放按钮资源
void tgui_button_destroy(tgui_button_t* button);

#ifdef __cplusplus
}
#endif

#endif // TGUI_BUTTON_H