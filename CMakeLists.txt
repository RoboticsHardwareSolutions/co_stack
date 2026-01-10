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


#################### FORMAT SECTION #######################
include(FetchContent)
FetchContent_Declare(
        rlibhelper
        GIT_REPOSITORY https://github.com/RoboticsHardwareSolutions/rlibhelper.git
        GIT_TAG main
)
FetchContent_MakeAvailable(rlibhelper)

file(GLOB_RECURSE SRC_FILES "src/*.*")
file(GLOB_RECURSE INCLUDE_FILES "include/*.*")

set(ALL_PROJECT_FILES ${SRC_FILES} ${INCLUDE_FILES})




set(FILES_FOR_FORMATTING ${SRC_FILES})
if (EXISTS "${rlibhelper_SOURCE_DIR}/format.cmake")
    include(${rlibhelper_SOURCE_DIR}/format.cmake)
else ()
    message(WARNING "format.cmake file not found")
endif ()
format_files(${PROJECT_NAME} SOURCES ${FILES_FOR_FORMATTING})