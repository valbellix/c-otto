file(GLOB_RECURSE C8_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/c8/*.cpp
)

add_library(c8 STATIC ${C8_SOURCES})

target_link_libraries(c8
    PUBLIC
        SDL2::SDL2
        CLI11::CLI11
)

target_include_directories(c8
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
add_executable(c-otto Main.cpp)

target_link_libraries(c-otto
    PRIVATE
        c8
)

include(GNUInstallDirs)
install(TARGETS c-otto
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)