#include "../include/Enemy.hpp"
#include "../include/Levels.hpp"

int countEnemies(map* level) {
    int count = 0;
    for (int y = 1; y < 22; y++) {
        for (int x = 1; x < 42; x++) {
            if (level->level[y][x] == 'N' ||
                level->level[y][x] == 'S') {
                count++;
            }
        }
    }
    return count;
}