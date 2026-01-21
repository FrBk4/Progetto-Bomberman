cmake_minimum_required(VERSION 4.0)
project(ascii_bomberman)

set(CMAKE_CXX_STANDARD 20)

include_directories(C:/pdcurses/PDCurses-master)

add_executable(ascii_bomberman
        src/main.cpp
        src/Game.cpp
        src/Menu.cpp
        include/Game.h
        include/Menu.h
        include/Leaderboard.h
        src/Leaderboard.cpp
        include/Score.h
        src/Score.cpp
        cmake-build-debug/Leaderboard.txt
        include/Levels.h
        src/Levels.cpp
        include/Player.hpp
        include/Enemy.hpp
        src/Enemy.cpp
        src/Player.cpp
)

target_include_directories(ascii_bomberman PRIVATE include)

target_link_libraries(ascii_bomberman
        C:/pdcurses/PDCurses-master/wincon/pdcurses.a
)
