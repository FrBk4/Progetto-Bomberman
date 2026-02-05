#include "../include/Enemy.hpp"
#include "../include/Levels.hpp"

int countEnemies(map* level) {
    int count = 0;
    for (int y = 1; y < 22; y++) {
        for (int x = 1; x < 42; x++) {
            char c = level->level[y][x];

            if (c == 'N' || c == 'U' || c == 'S' ||
                c == 'T') {
                count++;
            }
        }
    }
    return count;
}