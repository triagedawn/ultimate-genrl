add_subdirectory(bass)

add_library(imgui STATIC
        imgui/imconfig.h
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_internal.h
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui.cpp
        imgui/imgui.h
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h
        imgui/backends/imgui_impl_dx9.cpp
        imgui/backends/imgui_impl_dx9.h
        imgui/backends/imgui_impl_win32.cpp
        imgui/backends/imgui_impl_win32.h
        )
target_include_directories(imgui PUBLIC imgui)
target_link_libraries(imgui PUBLIC d3d9 dwmapi)

add_subdirectory(json)

set(LEMONHOOK_TESTS OFF)
add_subdirectory(lemonhook)

add_subdirectory(raknet)
target_include_directories(raknet PUBLIC raknet)

