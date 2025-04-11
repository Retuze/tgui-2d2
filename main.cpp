#include "include/tgui_core.h"
#include "include/tgui_button.h"
#include "include/tgui_constraint.h"
#include <stdlib.h>

// 声明Windows平台实现
extern tgui_platform_t windows_platform;

int main() {
    // 创建屏幕缓冲区
    const int width = 800;
    const int height = 600;
    const int buffer_size = width * height * 2; // RGB565 每个像素2字节
    uint16_t* screen_buffer = (uint16_t*)malloc(buffer_size);
    if (screen_buffer == NULL) {
        return -1;
    }

    // 初始化GUI系统(800x600, RGB565格式)
    if (tgui_init(&windows_platform, width, height, TGUI_PIXFMT_RGB565, screen_buffer) != 0) {
        free(screen_buffer);
        return -1;
    }

    // 创建按钮
    tgui_button_t button1, button2;
    tgui_button_init(&button1, 1, "Button 1");
    tgui_button_init(&button2, 2, "Button 2");

    // 设置按钮颜色
    tgui_button_set_colors(&button1, 0xF800, 0xFFFF); // 红色背景，白色文字
    tgui_button_set_colors(&button2, 0x07E0, 0x0000); // 绿色背景，黑色文字

    // 设置约束
    tgui_constraint_t constraints[4];
    
    // button1居中显示
    constraints[0] = {0, TGUI_EDGE_CENTER_X, TGUI_EDGE_CENTER_X, TGUI_CONSTRAINT_FIXED};
    constraints[0].value.fixed_value = 0;
    
    constraints[1] = {0, TGUI_EDGE_CENTER_Y, TGUI_EDGE_CENTER_Y, TGUI_CONSTRAINT_FIXED};
    constraints[1].value.fixed_value = -50;
    
    // button2在button1下方50像素
    constraints[2] = {1, TGUI_EDGE_TOP, TGUI_EDGE_BOTTOM, TGUI_CONSTRAINT_FIXED};
    constraints[2].value.fixed_value = 50;
    
    constraints[3] = {1, TGUI_EDGE_CENTER_X, TGUI_EDGE_CENTER_X, TGUI_CONSTRAINT_FIXED};
    constraints[3].value.fixed_value = 0;

    // 添加约束
    for (int i = 0; i < 4; i++) {
        tgui_widget_add_constraint(constraints[i].self_edge == 0 ? &button1.base : &button2.base, &constraints[i]);
    }

    // 添加按钮到根控件
    tgui_widget_add_child(tgui_get_root_widget(), &button1.base);
    tgui_widget_add_child(tgui_get_root_widget(), &button2.base);

    // 主循环
    while (true) {
        tgui_render();
        
        // 简单退出机制
        if (windows_platform.get_event(NULL) != 0) {
            break;
        }
    }

    free(screen_buffer);
    return 0;
}
