cmake_minimum_required(VERSION 3.15)

project("ultimate-genrl" LANGUAGES CXX)

set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".asi")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_subdirectory(lib EXCLUDE_FROM_ALL)

add_library(${PROJECT_NAME} SHARED
        src/main.cpp
        src/config.cpp
        src/plugin.cpp
        src/ultimate_genrl.cpp
        src/ui/dx9.cpp
        src/ui/menu.cpp
        src/misc/misc.cpp
        )

target_compile_definitions(${PROJECT_NAME} PRIVATE
        "_MBCS"
        )
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_STANDARD_EXTENSIONS NO
        )
target_link_libraries(${PROJECT_NAME} PRIVATE
        bass
        imgui
        lemonhook
        nlohmann_json
        raknet
        )

find_program(UPX_BIN upx)
if (UPX_BIN)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${UPX_BIN} --lzma $<TARGET_FILE:${PROJECT_NAME}>)
endif ()

install(TARGETS ${PROJECT_NAME} DESTINATION .)