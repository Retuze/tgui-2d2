#ifndef TGUI_BUTTON_H
#define TGUI_BUTTON_H

#include "tgui_widget.h"

#ifdef __cplusplus
extern "C" {
#endif

// 前向声明
typedef struct tgui_button tgui_button_t;

// 按钮结构体
struct tgui_button {
    tgui_widget_t base;
    char* text;
    uint16_t bg_color;
    uint16_t text_color;
    void (*on_click)(tgui_button_t*);
};

// 初始化按钮
void tgui_button_init(tgui_button_t* button, uint16_t id, const char* text);

// 设置按钮颜色
void tgui_button_set_colors(tgui_button_t* button, uint16_t bg_color, uint16_t text_color);

#ifdef __cplusplus
}
#endif

#endif // TGUI_BUTTON_H