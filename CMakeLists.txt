cmake_minimum_required(VERSION 3.10)
project(tgui-2d)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

# 定义 UNICODE 宏
add_definitions(-DUNICODE -D_UNICODE)

# 包含所有源文件
add_executable(tgui-2d
    src/tgui_core.c
    src/tgui_widget.c
    src/tgui_button.c
    src/tgui_constraint.c
    src/tgui_win32.c
    main.cpp
)

# 设置 C 源文件的编译选项
set_source_files_properties(
    src/tgui_core.c
    src/tgui_widget.c
    src/tgui_button.c
    src/tgui_constraint.c
    src/tgui_win32.c
    PROPERTIES LANGUAGE C
)

# Windows平台链接库
if(WIN32)
    target_link_libraries(tgui-2d gdi32)
endif()