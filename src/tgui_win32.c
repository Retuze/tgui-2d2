#include "../include/tgui_core.h"
#include <windows.h>
#include <stdint.h>
#include <tchar.h>

// 函数声明
static int windows_init(tgui_screen_t* screen);
static tgui_render_result_t windows_render(const tgui_screen_t* screen);
static int windows_get_event(void* event);
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Windows平台实现
tgui_platform_t windows_platform = {
    .init = windows_init,
    .render = windows_render,
    .get_event = windows_get_event
};

static HDC screen_dc = NULL;
static HBITMAP screen_bitmap = NULL;
static uint32_t* screen_buffer = NULL;
static HWND hwnd = NULL;
static HINSTANCE hInstance = NULL;
static RECT client_rect = {0};

// 窗口过程函数
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE:
            GetClientRect(hwnd, &client_rect);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            if (screen_dc && screen_bitmap) {
                // 计算居中显示的位置
                int x = (client_rect.right - client_rect.left - ps.rcPaint.right + ps.rcPaint.left) / 2;
                int y = (client_rect.bottom - client_rect.top - ps.rcPaint.bottom + ps.rcPaint.top) / 2;
                BitBlt(hdc, x, y, ps.rcPaint.right - ps.rcPaint.left, 
                      ps.rcPaint.bottom - ps.rcPaint.top,
                      screen_dc, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
            }
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// 初始化Windows平台
static int windows_init(tgui_screen_t* screen) {
    if (screen == NULL || screen->width == 0 || screen->height == 0 || screen->buffer == NULL) {
        return -1;
    }

    hInstance = GetModuleHandle(NULL);

    // 注册窗口类
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = TEXT("TGUI2D");
    
    if (!RegisterClassEx(&wc)) {
        return -1;
    }

    // 计算窗口大小（包含边框和标题栏）
    RECT window_rect = {0, 0, screen->width, screen->height};
    AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);

    // 创建窗口
    hwnd = CreateWindowEx(
        0,
        TEXT("TGUI2D"),
        TEXT("TGUI 2D"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        window_rect.right - window_rect.left,
        window_rect.bottom - window_rect.top,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) {
        return -1;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    // 获取客户区大小
    GetClientRect(hwnd, &client_rect);

    // 创建内存DC
    HDC hdc = GetDC(hwnd);
    if (hdc == NULL) return -1;
    
    screen_dc = CreateCompatibleDC(hdc);
    ReleaseDC(hwnd, hdc);
    if (screen_dc == NULL) return -1;
    
    // 创建32位RGBA屏幕缓冲区
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = screen->width;
    bmi.bmiHeader.biHeight = -screen->height; // 从上到下的位图
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    
    screen_bitmap = CreateDIBSection(screen_dc, &bmi, DIB_RGB_COLORS, (void**)&screen_buffer, NULL, 0);
    if (screen_bitmap == NULL || screen_buffer == NULL) return -1;
    
    SelectObject(screen_dc, screen_bitmap);
    
    return 0;
}

// 渲染函数
static tgui_render_result_t windows_render(const tgui_screen_t* screen) {
    if (screen == NULL || screen->buffer == NULL || screen_buffer == NULL) {
        return TGUI_RENDER_ERROR;
    }
    
    // 根据源格式转换像素
    if (screen->format == TGUI_PIXFMT_RGB565) {
        uint16_t* src = (uint16_t*)screen->buffer;
        for (int y = 0; y < screen->height; y++) {
            for (int x = 0; x < screen->width; x++) {
                uint16_t pixel = src[y * screen->buffer_width + x];
                // 提取 RGB565 颜色分量
                uint8_t r = (pixel >> 11) & 0x1F;  // 5 bits
                uint8_t g = (pixel >> 5) & 0x3F;   // 6 bits
                uint8_t b = pixel & 0x1F;          // 5 bits
                
                // 扩展颜色范围到 8 位
                // 使用移位和或运算来扩展颜色范围
                r = (r << 3) | (r >> 2);  // 5 bits -> 8 bits
                g = (g << 2) | (g >> 4);  // 6 bits -> 8 bits
                b = (b << 3) | (b >> 2);  // 5 bits -> 8 bits
                
                // 组合成 RGBA8888 格式 (0xAARRGGBB)
                screen_buffer[y * screen->width + x] = (0xFF << 24) | (r << 16) | (g << 8) | b;
            }
        }
    } else if (screen->format == TGUI_PIXFMT_RGBA8888) {
        // 直接拷贝RGBA数据
        uint32_t* src = (uint32_t*)screen->buffer;
        for (int y = 0; y < screen->height; y++) {
            memcpy(&screen_buffer[y * screen->width], 
                  &src[y * screen->buffer_width],
                  screen->width * 4);
        }
    }
    
    // 显示到窗口
    HDC hdc = GetDC(hwnd);
    if (hdc == NULL) return TGUI_RENDER_ERROR;
    
    // 计算居中显示的位置
    int x = (client_rect.right - client_rect.left - screen->width) / 2;
    int y = (client_rect.bottom - client_rect.top - screen->height) / 2;
    
    BitBlt(hdc, x, y, screen->width, screen->height, screen_dc, 0, 0, SRCCOPY);
    ReleaseDC(hwnd, hdc);
    
    return TGUI_RENDER_OK;
}

// 获取事件
static int windows_get_event(void* event) {
    MSG msg;
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return -1; // 退出信号
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}