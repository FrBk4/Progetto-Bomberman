cmake_minimum_required(VERSION 4.0)
project(ascii_bomberman)

set(CMAKE_CXX_STANDARD 20)

include_directories(C:/pdcurses/PDCurses-master)

add_executable(ascii_bomberman
        src/main.cpp
        src/Game.cpp
        src/Menu.cpp
        include/Game.hpp
        include/Menu.hpp
        include/Leaderboard.hpp
        src/Leaderboard.cpp
        include/Score.hpp
        src/Score.cpp
        cmake-build-debug/Leaderboard.txt
        include/Levels.hpp
        src/Levels.cpp
        include/Player.hpp
        src/Player.cpp
        include/items.hpp
        src/items.cpp
)

target_include_directories(ascii_bomberman PRIVATE include)

target_link_libraries(ascii_bomberman
        C:/pdcurses/PDCurses-master/wincon/pdcurses.a)
