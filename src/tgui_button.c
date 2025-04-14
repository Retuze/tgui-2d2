#include "../include/tgui_button.h"
#include "../include/tgui_core.h"
#include <string.h>
#include <stdlib.h>

// 按钮绘制函数
static void button_draw(tgui_widget_t* base, tgui_screen_t* screen) {
    tgui_button_t* button = (tgui_button_t*)base;
    tgui_pixel_t* buffer = screen->buffer;
    uint32_t pitch = screen->buffer_width;
    
    // 绘制背景
    for (uint16_t y = base->y; y < base->y + base->height; y++) {
        for (uint16_t x = base->x; x < base->x + base->width; x++) {
            if (y < screen->height && x < screen->width) {
                buffer[y * pitch + x] = button->bg_color;
            }
        }
    }
    
    // 绘制边框
    tgui_pixel_t border_color = TGUI_COLOR_BLACK;
    for (uint16_t x = base->x; x < base->x + base->width; x++) {
        if (base->y < screen->height && x < screen->width) {
            buffer[base->y * pitch + x] = border_color; // 上边框
        }
        if (base->y + base->height - 1 < screen->height && x < screen->width) {
            buffer[(base->y + base->height - 1) * pitch + x] = border_color; // 下边框
        }
    }
    
    for (uint16_t y = base->y; y < base->y + base->height; y++) {
        if (y < screen->height && base->x < screen->width) {
            buffer[y * pitch + base->x] = border_color; // 左边框
        }
        if (y < screen->height && base->x + base->width - 1 < screen->width) {
            buffer[y * pitch + base->x + base->width - 1] = border_color; // 右边框
        }
    }
    
    // 绘制文本(简单实现)
    if (button->text != NULL) {
        uint16_t text_x = base->x + (base->width - strlen(button->text) * 8) / 2;
        uint16_t text_y = base->y + (base->height - 8) / 2;
        
        // 简单文本绘制(每个字符8x8像素)
        for (uint16_t i = 0; i < strlen(button->text); i++) {
            for (uint16_t dy = 0; dy < 8; dy++) {
                for (uint16_t dx = 0; dx < 8; dx++) {
                    uint16_t px = text_x + i * 8 + dx;
                    uint16_t py = text_y + dy;
                    if (py < screen->height && px < screen->width) {
                        buffer[py * pitch + px] = button->text_color;
                    }
                }
            }
        }
    }
}

// 按钮初始化函数
void tgui_button_init(tgui_button_t* button, uint16_t id, const char* text) {
    // 初始化基类
    tgui_widget_init(&button->base, id);
    
    // 设置按钮大小
    button->base.width = 100;
    button->base.height = 30;
    
    // 设置绘制函数
    button->base.draw = button_draw;
    
    // 设置文本
    if (text != NULL) {
        button->text = strdup(text);
    } else {
        button->text = NULL;
    }
    
    // 设置默认颜色
    button->bg_color = TGUI_COLOR_BLACK;    // 黑色背景
    button->text_color = TGUI_COLOR_WHITE;  // 白色文字
}

// 设置按钮颜色
void tgui_button_set_colors(tgui_button_t* button, tgui_pixel_t bg_color, tgui_pixel_t text_color) {
    button->bg_color = bg_color;
    button->text_color = text_color;
}

// ... 其余函数保持不变 ...