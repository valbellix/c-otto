file(GLOB_RECURSE C8_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/c8/*.cpp
)

add_library(c8 ${C8_SOURCES})
target_include_directories(c8
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
add_executable(chip-otto Main.cpp)

target_link_libraries(chip-otto
    PRIVATE
        c8
)