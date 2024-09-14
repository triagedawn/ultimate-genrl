cmake_minimum_required(VERSION 3.0)

project(bass LANGUAGES C)

add_library(${PROJECT_NAME} INTERFACE include/bass.h)
target_link_libraries(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/bass.lib")
target_include_directories(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")