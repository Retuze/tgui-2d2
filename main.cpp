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
    const int buffer_size = width * height * TGUI_BYTES_PER_PIXEL;
    tgui_pixel_t* screen_buffer = (tgui_pixel_t*)malloc(buffer_size);
    if (screen_buffer == NULL) {
        return -1;
    }

    // 初始化GUI系统
    #if TGUI_CURRENT_PIXEL_FORMAT == TGUI_PIXEL_FORMAT_ARGB8888
    if (tgui_init(&windows_platform, width, height, TGUI_PIXFMT_ARGB8888, screen_buffer) != 0) {
    #else
    if (tgui_init(&windows_platform, width, height, TGUI_PIXFMT_RGB565, screen_buffer) != 0) {
    #endif
        free(screen_buffer);
        return -1;
    }

    // 创建按钮
    tgui_button_t button1, button2, button3, button4, button5;
    tgui_button_init(&button1, 1, "Center");
    tgui_button_init(&button2, 2, "Below");
    tgui_button_init(&button3, 3, "Left");
    tgui_button_init(&button4, 4, "Right");
    tgui_button_init(&button5, 5, "Flexible");

    // 设置按钮颜色
    tgui_button_set_colors(&button1, TGUI_COLOR_RED, TGUI_COLOR_WHITE);      // 红色
    tgui_button_set_colors(&button2, TGUI_COLOR_GREEN, TGUI_COLOR_BLACK);    // 绿色
    tgui_button_set_colors(&button3, TGUI_COLOR_BLUE, TGUI_COLOR_WHITE);     // 蓝色
    tgui_button_set_colors(&button4, 0xFFFFA500, TGUI_COLOR_BLACK);          // 橙色
    tgui_button_set_colors(&button5, 0xFF800080, TGUI_COLOR_WHITE);          // 紫色

    // 设置按钮大小
    button1.base.width = 100;  
    button1.base.height = 40;
    button2.base.width = 100;
    button2.base.height = 40;
    button3.base.width = 80;
    button3.base.height = 40;
    button4.base.width = 80;
    button4.base.height = 40;
    button5.base.width = 120;
    button5.base.height = 40;

    // 添加按钮到根控件
    tgui_widget_t* root = tgui_get_root_widget();
    tgui_widget_add_child(root, &button1.base);
    tgui_widget_add_child(root, &button2.base);
    tgui_widget_add_child(root, &button3.base);
    tgui_widget_add_child(root, &button4.base);
    tgui_widget_add_child(root, &button5.base);

    // 中心按钮约束（固定约束）
    tgui_constraint_t center_x = {
        .target_id = 0,  // 根控件ID
        .target_edge = TGUI_EDGE_CENTER_X,
        .self_edge = TGUI_EDGE_CENTER_X,
        .type = TGUI_CONSTRAINT_FIXED,
        .value = {.fixed_value = 0}
    };

    tgui_constraint_t center_y = {
        .target_id = 0,  // 根控件ID
        .target_edge = TGUI_EDGE_CENTER_Y,
        .self_edge = TGUI_EDGE_CENTER_Y,
        .type = TGUI_CONSTRAINT_FIXED,
        .value = {.fixed_value = 0}
    };

    // 下方按钮约束（相对约束）
    tgui_constraint_t below_x = {
        .target_id = 1,  // button1的ID
        .target_edge = TGUI_EDGE_CENTER_X,
        .self_edge = TGUI_EDGE_CENTER_X,
        .type = TGUI_CONSTRAINT_FIXED,
        .value = {.fixed_value = 0}
    };

    tgui_constraint_t below_y = {
        .target_id = 1,  // button1的ID
        .target_edge = TGUI_EDGE_BOTTOM,
        .self_edge = TGUI_EDGE_TOP,
        .type = TGUI_CONSTRAINT_FIXED,
        .value = {.fixed_value = 20}
    };

    // 左侧按钮约束（百分比约束）
    tgui_constraint_t left_x = {
        .target_id = 0,  // 根控件ID
        .target_edge = TGUI_EDGE_LEFT,
        .self_edge = TGUI_EDGE_LEFT,
        .type = TGUI_CONSTRAINT_PERCENT,
        .value = {.percent = 0.1f}  // 距离左边10%
    };

    tgui_constraint_t left_y = {
        .target_id = 1,  // button1的ID
        .target_edge = TGUI_EDGE_CENTER_Y,
        .self_edge = TGUI_EDGE_CENTER_Y,
        .type = TGUI_CONSTRAINT_FIXED,
        .value = {.fixed_value = 0}
    };

    // 右侧按钮约束（百分比约束）
    tgui_constraint_t right_x = {
        .target_id = 0,  // 根控件ID
        .target_edge = TGUI_EDGE_RIGHT,
        .self_edge = TGUI_EDGE_RIGHT,
        .type = TGUI_CONSTRAINT_PERCENT,
        .value = {.percent = 0.1f}  // 距离右边10%
    };

    tgui_constraint_t right_y = {
        .target_id = 1,  // button1的ID
        .target_edge = TGUI_EDGE_CENTER_Y,
        .self_edge = TGUI_EDGE_CENTER_Y,
        .type = TGUI_CONSTRAINT_FIXED,
        .value = {.fixed_value = 0}
    };

    // 弹性按钮约束（弹性约束）
    tgui_constraint_t flex_x = {
        .target_id = 0,  // 根控件ID
        .target_edge = TGUI_EDGE_CENTER_X,
        .self_edge = TGUI_EDGE_CENTER_X,
        .type = TGUI_CONSTRAINT_FLEXIBLE,
        .value = {
            .flex = {
                .min_value = -100,  // 最小偏移
                .max_value = 100,   // 最大偏移
                .weight = 0.7f      // 偏向右侧
            }
        }
    };

    tgui_constraint_t flex_y = {
        .target_id = 2,  // button2的ID
        .target_edge = TGUI_EDGE_BOTTOM,
        .self_edge = TGUI_EDGE_TOP,
        .type = TGUI_CONSTRAINT_FIXED,
        .value = {.fixed_value = 40}
    };

    // 添加约束
    // 中心按钮
    tgui_widget_add_constraint(&button1.base, &center_x);
    tgui_widget_add_constraint(&button1.base, &center_y);

    // 下方按钮
    tgui_widget_add_constraint(&button2.base, &below_x);
    tgui_widget_add_constraint(&button2.base, &below_y);

    // 左侧按钮
    tgui_widget_add_constraint(&button3.base, &left_x);
    tgui_widget_add_constraint(&button3.base, &left_y);

    // 右侧按钮
    tgui_widget_add_constraint(&button4.base, &right_x);
    tgui_widget_add_constraint(&button4.base, &right_y);

    // 弹性按钮
    tgui_widget_add_constraint(&button5.base, &flex_x);
    tgui_widget_add_constraint(&button5.base, &flex_y);

    // 触发布局更新
    tgui_trigger_layout();

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
