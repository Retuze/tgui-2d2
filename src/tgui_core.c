#include "../include/tgui_core.h"
#include "../include/tgui_widget.h"
#include <stdlib.h>
#include <string.h>

static const tgui_platform_t* platform = NULL;
static tgui_widget_t* root_widget = NULL;
static tgui_screen_t current_screen;

// 递归绘制控件及其子控件
static void draw_widget_tree(tgui_widget_t* widget, tgui_screen_t* screen) {
    if (!widget) return;
    
    // 绘制当前控件
    if (widget->draw) {
        widget->draw(widget, screen);
    }
    
    // 绘制子控件
    for (tgui_widget_t* child = widget->children; child != NULL; child = child->next) {
        draw_widget_tree(child, screen);
    }
}

// 获取当前屏幕缓冲区
const tgui_screen_t* tgui_get_screen(void) {
    return &current_screen;
}

// 获取根控件
tgui_widget_t* tgui_get_root_widget(void) {
    return root_widget;
}

// 初始化GUI系统
int tgui_init(const tgui_platform_t* platform_ptr, 
             uint16_t width, uint16_t height,
             tgui_pixel_format_t format,
             void* buffer) {
    if (platform_ptr == NULL || platform_ptr->init == NULL || 
        platform_ptr->render == NULL || platform_ptr->get_event == NULL ||
        buffer == NULL) {
        return -1; // 无效参数
    }
    
    // 检查像素格式是否匹配编译时配置
    #if TGUI_CURRENT_PIXEL_FORMAT == TGUI_PIXEL_FORMAT_ARGB8888
    if (format != TGUI_PIXFMT_ARGB8888) {
        return -4; // 像素格式不匹配
    }
    #elif TGUI_CURRENT_PIXEL_FORMAT == TGUI_PIXEL_FORMAT_RGB565
    if (format != TGUI_PIXFMT_RGB565) {
        return -4; // 像素格式不匹配
    }
    #endif
    
    // 初始化屏幕缓冲区
    current_screen.width = width;
    current_screen.height = height;
    current_screen.buffer_width = width; // 无padding
    current_screen.buffer_height = height;
    current_screen.format = format;
    current_screen.stride = width * TGUI_BYTES_PER_PIXEL;
    current_screen.buffer_size = current_screen.stride * height;
    current_screen.buffer = (tgui_pixel_t*)buffer;
    
    // 清空屏幕缓冲区为透明色
    for (size_t i = 0; i < (width * height); i++) {
        current_screen.buffer[i] = TGUI_COLOR_TRANSPARENT;
    }
    
    platform = platform_ptr;
    if (platform->init(&current_screen) != 0) {
        return -2; // 平台初始化失败
    }
    
    root_widget = (tgui_widget_t*)malloc(sizeof(tgui_widget_t));
    if (root_widget == NULL) {
        return -3; // 内存分配失败
    }
    
    tgui_widget_init(root_widget, 0); // 根控件ID为0
    return 0;
}

tgui_render_result_t tgui_render(void) {
    if (platform == NULL || root_widget == NULL) {
        return TGUI_RENDER_ERROR;
    }
    
    // 清屏为透明色
    for (size_t i = 0; i < (current_screen.width * current_screen.height); i++) {
        current_screen.buffer[i] = TGUI_COLOR_TRANSPARENT;
    }
    
    // 从根控件开始递归绘制整个控件树
    draw_widget_tree(root_widget, &current_screen);
    
    // 调用平台渲染
    return platform->render(&current_screen);
}