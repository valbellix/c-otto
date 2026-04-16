cmake_minimum_required(VERSION 3.20)

project(c-otto LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.6.2
)
FetchContent_MakeAvailable(cli11)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

install(FILES LICENSE DESTINATION .)
install(FILES LICENSES/CLI11.txt DESTINATION LICENSES)
