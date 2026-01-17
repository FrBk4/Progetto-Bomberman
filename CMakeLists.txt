cmake_minimum_required(VERSION 3.20)
project(ascii_bomberman)

set(CMAKE_CXX_STANDARD 17)

include_directories(C:/pdcurses/PDCurses-master)

add_executable(ascii_bomberman
        main.cpp
)

target_link_libraries(ascii_bomberman
        C:/pdcurses/PDCurses-master/wincon/pdcurses.a
)
