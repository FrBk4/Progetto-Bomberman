#include <iostream>
#include <ctime>
#include "Levels.h"
#include <curses.h>

using namespace std;


map genlevels() {  //questa funzione genera i 5 livelli e ritorna un array che li contiene come matrici 23x86

    struct small_map{    //tipo di mappa iniziale con colonne dimezzate, utile per la generazione ma poco estetico
        char level[23][43][5];
    };

    small_map m;

    for (int liv=0; liv<5; liv++)  //struttura di base (muri indistruttibili = char(219) )
        for (int y=0; y<23; y++)
            for(int x=0; x<43; x++) {
                if (x%2==0 && y%2==0) m.level[y][x][liv]=char(219);
                else m.level[y][x][liv]=' ';
                if (x==0 || y==0 || x==42 || y== 22) m.level[y][x][liv]=char(219);
            }


    srand(time(nullptr));     //generazione casuale di mura distruttibili (=char(177)) che dipende dal livello
    for (int liv=0; liv<5; liv++) {
        float prob = (liv+1)*10;
        for (int y=0; y<23; y++)
            for(int x=0; x<43; x++) {
                if (m.level[y][x][liv]==' ' && (x>5 || y>5)) {
                    int p = rand()%100;
                    if (p<=prob)
                        m.level[y][x][liv]=char(177);
                }
            }

    }

    map true_map;  //vedi definizione struct in levels.h, come small_map ma con x=86, dimensioni vere mostrate
    for (int liv=0; liv<5; liv++) //conversione da small_map a map
        for (int y=0; y<23; y++)
            for(int x=0; x<86; x+=2) {
                int small_x = x/2;
                true_map.level[y][x][liv] = m.level[y][small_x][liv];
                true_map.level[y][x+1][liv] = m.level[y][small_x][liv];
            }

    return true_map;
}


WINDOW* enclose_screen(map map) {  //questa funzione mostra su schermo la mappa (inizialmente livello 1)

    int x_offset = getmaxx(stdscr) / 2 - 43;
    if (x_offset < 0) x_offset = 0;

    WINDOW * screen = newwin(23, 86, 5, x_offset);

    for (int y=0; y<23; y++)
        for(int x=0; x<86; x+=2) {
            mvwprintw(screen, y, x, "%c", map.level[y][x][0]);
        }

    wborder(screen,  ACS_VLINE, ACS_VLINE,
                 ACS_HLINE, ACS_HLINE,
                 ACS_ULCORNER, ACS_URCORNER,
                 ACS_LLCORNER, ACS_LRCORNER);

    wrefresh(screen);

    return screen;
}

int change_level(map map, WINDOW* screen, bool action, int lvl) { //Manuel ti ho lasciato questa funzione da chiamare
                                                                  //quando gestirai l'input del player e il cambio di livello
    if (action && lvl < 4) lvl++;
    else if (!action && lvl>0) lvl--; // ATTENZIONE: si suppone che il primo lvl passato sia 0 dopo la chiamata di enclose_screen

    for (int y=0; y<23; y++)
        for(int x=0; x<86; x+=2) {
            mvwprintw(screen, y, x, "%c", map.level[y][x][lvl]);
        }

    wborder(screen,  ACS_VLINE, ACS_VLINE,
                 ACS_HLINE, ACS_HLINE,
                 ACS_ULCORNER, ACS_URCORNER,
                 ACS_LLCORNER, ACS_LRCORNER);

    wrefresh(screen);

    return lvl;
}



