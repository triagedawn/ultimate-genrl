cmake_minimum_required(VERSION 3.13)

project(lemonhook LANGUAGES CXX)

option(LEMONHOOK_TESTS "Enable testing" ON)

add_subdirectory(lib/xbyak)
option(ZYDIS_BUILD_TOOLS "" OFF)
option(ZYDIS_BUILD_EXAMPLES "" OFF)
add_subdirectory(lib/zydis)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

add_library(${PROJECT_NAME} STATIC
        include/lemon/details/details.hpp
        include/lemon/details/detour_base.hpp
        include/lemon/details/detour_callback.hpp
        include/lemon/details/hook_base.hpp
        include/lemon/details/hook_callback.hpp
        include/lemon/detour.hpp
        include/lemon/hook.hpp
        include/lemon/mem.hpp

        src/details/details.cpp
        src/details/detour_base.cpp
        src/mem.cpp
        )

target_include_directories(${PROJECT_NAME} PRIVATE include/lemon)
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PUBLIC xbyak Zydis)

if (LEMONHOOK_TESTS)
    enable_testing()

    add_executable(main_test tests/main.cpp)
    target_link_libraries(main_test lemonhook)

    add_test(main main_test)
endif ()
