cmake_minimum_required(VERSION 3.20)

project(c-otto LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.6.2
)
FetchContent_MakeAvailable(cli11)

find_package(SDL2 REQUIRED)

add_subdirectory(src)

include(GNUInstallDirs)

install(FILES LICENSE DESTINATION .)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/LICENSES/CLI11.txt")
    install(FILES LICENSES/CLI11.txt DESTINATION LICENSES)
endif()
