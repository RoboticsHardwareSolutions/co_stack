if (MSVC OR MSYS OR MINGW)
    add_definitions(-DCO_STACK_FOR_WINDOWS)
endif ()

if (APPLE)
    add_definitions(-DCO_STACK_FOR_APPLE)
endif ()

if (UNIX AND NOT APPLE)
    add_definitions(-DCO_STACK_FOR_UNIX)
endif ()


set(CO_STACK_DIRECTORIES  co_stack/drivers co_stack/include co_stack/src co_stack/ )
file(GLOB_RECURSE CO_STACK_EXECUTABLE_FILES  "co_stack/include/*.*" "co_stack/src/*.*" )









