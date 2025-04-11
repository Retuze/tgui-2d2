#ifndef TGUI_CORE_H
#define TGUI_CORE_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// 前向声明
typedef struct tgui_widget tgui_widget_t;

// 像素格式枚举
typedef enum {
    TGUI_PIXFMT_RGB565,  // 16位RGB565
    TGUI_PIXFMT_RGBA8888 // 32位RGBA
} tgui_pixel_format_t;

// 屏幕缓冲区描述结构体
typedef struct {
    uint16_t width;          // 逻辑宽度(像素)
    uint16_t height;         // 逻辑高度(像素) 
    uint16_t buffer_width;   // 物理宽度(可能包含padding)
    uint16_t buffer_height;  // 物理高度
    uint32_t buffer_size;    // 缓冲区大小(字节)
    tgui_pixel_format_t format; // 像素格式
    void* buffer;           // 缓冲区指针
    uint32_t stride;        // 每行字节数
} tgui_screen_t;

// 渲染结果枚举
typedef enum {
    TGUI_RENDER_OK,      // 渲染成功
    TGUI_RENDER_PARTIAL, // 部分渲染(脏矩形)
    TGUI_RENDER_ERROR    // 渲染错误
} tgui_render_result_t;

// 平台抽象接口
typedef struct {
    // 初始化平台
    int (*init)(tgui_screen_t* screen); // 传入期望的屏幕参数
    // 渲染屏幕
    tgui_render_result_t (*render)(const tgui_screen_t* screen);
    // 获取输入事件
    int (*get_event)(void* event);
} tgui_platform_t;

// 初始化GUI系统
int tgui_init(const tgui_platform_t* platform, 
             uint16_t width, uint16_t height,
             tgui_pixel_format_t format,
             void* buffer);

// 渲染GUI
tgui_render_result_t tgui_render(void);

// 获取当前屏幕缓冲区
const tgui_screen_t* tgui_get_screen(void);

// 获取根控件
tgui_widget_t* tgui_get_root_widget(void);

#ifdef __cplusplus
}
#endif

#endif // TGUI_CORE_H