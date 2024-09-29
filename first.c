// Imports
#include <ncurses.h>
#include <string.h>
#include <math.h>

// Defining Colors
#define DEFAULT_COLOR -1

#define MESSAGE_TEXT_COLOR 1
#define MESSAGE_BOX_COLOR 2

int row, col, ch;

// Functions for min and max
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

void message(int col, char *text) {
    attron(A_BOLD);
    attron(COLOR_PAIR(MESSAGE_BOX_COLOR));
    mvhline(0,0,ACS_HLINE,col);
    mvhline(2,0,ACS_HLINE,col);
    mvaddch(0,0,ACS_ULCORNER);
    mvaddch(0,col-1,ACS_URCORNER);
    mvaddch(1,0,ACS_VLINE);
    mvaddch(1,col-1,ACS_VLINE);
    mvaddch(2,0,ACS_LLCORNER);
    mvaddch(2,col-1,ACS_LRCORNER);
    attroff(COLOR_PAIR(MESSAGE_BOX_COLOR));
    
    attron(COLOR_PAIR(MESSAGE_TEXT_COLOR));
    mvprintw(1,1+col/2-strlen(text)/2, "%s", text);
    attroff(COLOR_PAIR(MESSAGE_TEXT_COLOR));
    attroff(A_BOLD);
}

void standing_desks() {
    bool running = true;

    while (running) {
        clear();
        message(col, "Standing Desks");

        mvprintw(13,2,"[Q] Back");

        ch = getch();

        switch(ch) {
            case 'q':
                running = false;
                break;
        }
    }
}

int main() {
    bool running = true;
    int selected = 0;
    char* products[] = {"Standing Desks", "Blackout Curtains", "Tabletops", "Table Legs"};

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    getmaxyx(stdscr,row,col);

    // Assigning colors to variables
    if (has_colors()) {
        start_color();
        use_default_colors();
        init_pair(MESSAGE_BOX_COLOR, COLOR_BLUE, DEFAULT_COLOR);
        init_pair(MESSAGE_TEXT_COLOR, COLOR_YELLOW, DEFAULT_COLOR);
    }

    while (running) {
        erase();

        // Greet the user with an engaging greeting
        message(col, "WELCOME TO YOUR LOCAL IKEA");

        // Printing Products
        for (int i = 0; i < 4; i++) {
            if (i == selected) {
                attron(A_BOLD | A_STANDOUT);
                mvprintw(3 + i * 2, 2, "%s", products[i]);
                attroff(A_BOLD | A_STANDOUT);
            } else {
                mvprintw(3 + i * 2, 2, "%s", products[i]);
            }
        }

        mvprintw(13,2,"[V] View Cart");

        attron(A_BOLD);
        attron(COLOR_PAIR(MESSAGE_TEXT_COLOR));
        mvaddch(3+selected * 2, 0, ACS_RARROW);
        attroff(COLOR_PAIR(MESSAGE_TEXT_COLOR));
        attroff(A_BOLD);

        move(row-1,0);
        ch = getch();

        // Assigning Keys
        switch(ch) {
            case KEY_RESIZE:
                getmaxyx(stdscr,row,col);
                break;
            case 'q':
                running = false;
                break;
            case KEY_DOWN:
                selected = min(3, selected+1);
                break;
            case KEY_UP:
                selected = max(0, selected-1);
                break;
            case 10:
                if (selected == 0) {
                    standing_desks();
                    break;
                }
        }
    }

    endwin();
    return 0;
}
