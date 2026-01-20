#include "Score.h"
#include <fstream>

using namespace std;

bool Score::saveScore(const string& filename, const string& name, int score) {
    ofstream out(filename, ios::app);
    if (!out.is_open()) return false;

    out << name << " " << score << "\n";
    return true;
}
