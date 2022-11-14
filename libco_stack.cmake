message("Hello from co_stack!")
message("Full path to module: ${CMAKE_CURRENT_LIST_FILE}")
message("Module located in directory: ${CMAKE_CURRENT_LIST_DIR}")

if (MSVC OR MSYS OR MINGW)
    add_definitions(-DCO_STACK_FOR_WINDOWS)
endif ()

if (APPLE)
    add_definitions(-DCO_STACK_FOR_APPLE)
endif ()

if (UNIX AND NOT APPLE)
    add_definitions(-DCO_STACK_FOR_UNIX)
endif ()


set(CO_STACK_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/src ${CMAKE_CURRENT_LIST_DIR}/ )
file(GLOB_RECURSE CO_STACK_EXECUTABLE_FILES  "${CMAKE_CURRENT_LIST_DIR}/include/*.*" "${CMAKE_CURRENT_LIST_DIR}/src/*.*" )










