cmake_minimum_required(VERSION 3.24)
project(co_stack C)
set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SOURCES "src/*.*")

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(
        ${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)
