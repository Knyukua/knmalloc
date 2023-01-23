cmake_minimum_required(VERSION 3.8)

project(knmalloc)

add_executable(knmalloc
    src/main.c
    src/knmalloc.c
)

target_include_directories(knmalloc PRIVATE ${CMAKE_SOURCE_DIR}/include)