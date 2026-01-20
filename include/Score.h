#ifndef ASCII_BOMBERMAN_SCORE_H
#define ASCII_BOMBERMAN_SCORE_H

#include <string>

class Score {
public:
    static bool saveScore(const std::string& filename,
                          const std::string& name,
                          int score);
};

#endif //ASCII_BOMBERMAN_SCORE_H