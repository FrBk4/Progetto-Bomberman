cmake_minimum_required(VERSION 4.0)
project(ascii_bomberman)

set(CMAKE_CXX_STANDARD 20)

include_directories(C:/pdcurses/PDCurses-master)

add_executable(ascii_bomberman
        main.cpp
)

target_link_libraries(ascii_bomberman
        C:/pdcurses/PDCurses-master/wincon/pdcurses.a
)
