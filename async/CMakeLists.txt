cmake_minimum_required (VERSION 3.8)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project (libasync VERSION ${PROJECT_VESRION})

configure_file(version.h.in  ${CMAKE_CURRENT_BINARY_DIR}/version.h)

add_library (${PROJECT_NAME} STATIC
    src/libasync.cpp
    src/CommandController.cpp
    src/Printers.cpp
    src/MassageQueue.cpp
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR}
)
target_include_directories(
    ${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE 
        -g
        -Wall
        -Wextra
        -Werror
        -pedantic
    )

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE
        /EHsc
        /W4
        /WX
    )

endif()
