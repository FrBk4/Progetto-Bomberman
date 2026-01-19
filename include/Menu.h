#ifndef ASCII_BOMBERMAN_MENU_H
#define ASCII_BOMBERMAN_MENU_H

#include <string>

class Menu {
public:
    // items = array di stringhe
    // count = numero di voci
    // ritorna indice selezionato
    // ritorna -1 se ESC
    int run(const std::string items[],
            int count);

private:
    void draw(const std::string items[],
              int count,
              int selected,
              int key);
};

#endif // ASCII_BOMBERMAN_MENU_H
