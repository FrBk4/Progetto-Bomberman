cmake_minimum_required(VERSION 4.0)
project(ascii_bomberman)

set(CMAKE_CXX_STANDARD 20)

include_directories(C:/pdcurses/PDCurses-master)

add_executable(ascii_bomberman
        src/main.cpp
        include/Menu.h
        src/Game.cpp
        src/Menu.cpp
        include/Game.h
        src/levels.cpp
        src/levels.h
)

target_include_directories(ascii_bomberman PRIVATE include)

target_link_libraries(ascii_bomberman
        C:/pdcurses/PDCurses-master/wincon/pdcurses.a
)
