// Imports
#include <ncurses.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// Defining Colors
#define DEFAULT_COLOR -1

#define MESSAGE_TEXT_COLOR 1
#define MESSAGE_BOX_COLOR 2
#define MESSAGE_ERROR_COLOR 3
#define CONFIRM_COLOR_Y 4

int row, col, ch;

// Functions for min and max
#define max(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

void message(int col, char *text)
{
    attron(A_BOLD);
    attron(COLOR_PAIR(MESSAGE_BOX_COLOR));
    mvhline(0, 0, ACS_HLINE, col);
    mvhline(2, 0, ACS_HLINE, col);
    mvaddch(0, 0, ACS_ULCORNER);
    mvaddch(0, col - 1, ACS_URCORNER);
    mvaddch(1, 0, ACS_VLINE);
    mvaddch(1, col - 1, ACS_VLINE);
    mvaddch(2, 0, ACS_LLCORNER);
    mvaddch(2, col - 1, ACS_LRCORNER);
    attroff(COLOR_PAIR(MESSAGE_BOX_COLOR));

    attron(COLOR_PAIR(MESSAGE_TEXT_COLOR));
    mvprintw(1, 1 + col / 2 - strlen(text) / 2, "%s", text);
    attroff(COLOR_PAIR(MESSAGE_TEXT_COLOR));
    attroff(A_BOLD);
}

void menu(int y, int width, char **sizes, int quantity, int selected)
{
    attron(A_BOLD);
    attron(COLOR_PAIR(MESSAGE_BOX_COLOR));
    mvhline(y, 1, ACS_HLINE, width);
    mvhline(y + quantity + 2, 1, ACS_HLINE, width);
    mvaddch(y, 0, ACS_ULCORNER);
    mvaddch(y, width + 1, ACS_URCORNER);
    mvvline(y + 1, 0, ACS_VLINE, quantity + 1);
    mvvline(y + 1, width + 1, ACS_VLINE, quantity + 1);
    mvaddch(y + quantity + 2, 0, ACS_LLCORNER);
    mvaddch(y + quantity + 2, width + 1, ACS_LRCORNER);

    // Print the "Sizes" label
    attron(COLOR_PAIR(MESSAGE_TEXT_COLOR));
    mvprintw(y + 1, 1, "SIZES:");
    attroff(COLOR_PAIR(MESSAGE_TEXT_COLOR));

    for (int i = 0; i < quantity; i++)
    {
        if (i == selected)
        {
            attron(A_BOLD | A_STANDOUT);
            mvprintw(i + y + 2, 1, "%-*.*s", width, width, sizes[i]);
            attroff(A_BOLD | A_STANDOUT);
        }
        else
        {
            mvprintw(i + y + 2, 1, "%-*.*s", width, width, sizes[i]);
        }
    }
    attroff(COLOR_PAIR(MESSAGE_BOX_COLOR));
}

bool confirm()
{
    int ch;
    mvprintw(11, 2, "Confirm? [");
    attron(COLOR_PAIR(CONFIRM_COLOR_Y));
    printw("Y");
    attroff(COLOR_PAIR(CONFIRM_COLOR_Y));
    printw("] or [");
    attron(COLOR_PAIR(MESSAGE_ERROR_COLOR));
    printw("N");
    attroff(COLOR_PAIR(MESSAGE_ERROR_COLOR));
    printw("]");

    ch = getch();
    return (ch == 'y');
}

void standing_desks()
{
    bool running = true;
    bool sizeSelected = false;
    int selectedSize = 0;
    int quantity = 0;

    while (running)
    {
        clear();
        message(col, "Standing Desks");

        char *sizes[] = {"S", "M", "L"};
        menu(3, 10, sizes, 3, selectedSize);

        mvprintw(13, 2, "[");
        attron(COLOR_PAIR(MESSAGE_TEXT_COLOR) | A_BOLD);
        printw("Q");
        attroff(COLOR_PAIR(MESSAGE_TEXT_COLOR) | A_BOLD);
        printw("] Back");

        if (sizeSelected)
        {
            bool validInput = false;
            char input[10];

            while (!validInput)
            {
                mvprintw(10, 2, "Enter Quantity: ");
                echo();
                wgetnstr(stdscr, input, sizeof(input) - 1);
                noecho();

                // Validate the input string
                validInput = true;
                for (int i = 0; input[i] != '\0'; i++)
                {
                    if (!isdigit(input[i]))
                    {
                        validInput = false;
                        break;
                    }
                }

                if (!validInput || sscanf(input, "%d", &quantity) != 1 || quantity <= 0)
                {
                    attron(COLOR_PAIR(MESSAGE_ERROR_COLOR) | A_BOLD);
                    mvprintw(16, 2, "Invalid input. Please enter a whole number.");
                    attroff(COLOR_PAIR(MESSAGE_ERROR_COLOR) | A_BOLD);
                    clrtoeol();
                }
                else
                {
                    if (confirm())
                    {
                        running = false;
                    }
                    else
                    {
                        sizeSelected = false;
                        quantity = 0;
                    }
                }
            }
        }
        else
        {
            ch = getch();

            switch (ch)
            {
            case 'q':
                running = false;
                break;
            case 10:
                sizeSelected = true;
                break;
            case KEY_DOWN:
                selectedSize = min(2, selectedSize + 1);
                break;
            case KEY_UP:
                selectedSize = max(0, selectedSize - 1);
                break;
            }
        }
    }
}

int main()
{
    bool running = true;
    int selected = 0;
    char *products[] = {"Standing Desks", "Blackout Curtains", "Tabletops", "Table Legs"};

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    getmaxyx(stdscr, row, col);

    // Assigning colors to variables
    if (has_colors())
    {
        start_color();
        use_default_colors();
        init_pair(MESSAGE_BOX_COLOR, COLOR_BLUE, DEFAULT_COLOR);
        init_pair(MESSAGE_TEXT_COLOR, COLOR_YELLOW, DEFAULT_COLOR);
        init_pair(MESSAGE_ERROR_COLOR, COLOR_RED, DEFAULT_COLOR);
        init_pair(CONFIRM_COLOR_Y, COLOR_GREEN, DEFAULT_COLOR);
    }

    while (running)
    {
        erase();

        // Greet the user with an engaging greeting
        message(col, "WELCOME TO YOUR LOCAL IKEA");

        // Printing Products
        for (int i = 0; i < 4; i++)
        {
            if (i == selected)
            {
                attron(A_BOLD | A_STANDOUT);
                mvprintw(3 + i * 2, 2, "%s", products[i]);
                attroff(A_BOLD | A_STANDOUT);
            }
            else
            {
                mvprintw(3 + i * 2, 2, "%s", products[i]);
            }
        }

        mvprintw(13, 2, "[");
        attron(COLOR_PAIR(MESSAGE_TEXT_COLOR) | A_BOLD);
        printw("V");
        attroff(COLOR_PAIR(MESSAGE_TEXT_COLOR) | A_BOLD);
        printw("] View Cart");

        attron(COLOR_PAIR(MESSAGE_TEXT_COLOR) | A_BOLD);
        mvaddch(3 + selected * 2, 0, ACS_RARROW);
        attroff(COLOR_PAIR(MESSAGE_TEXT_COLOR) | A_BOLD);

        move(row - 1, 0);
        ch = getch();

        // Assigning Keys
        switch (ch)
        {
        case KEY_RESIZE:
            getmaxyx(stdscr, row, col);
            break;
        case 'q':
            running = false;
            break;
        case KEY_DOWN:
            selected = min(3, selected + 1);
            break;
        case KEY_UP:
            selected = max(0, selected - 1);
            break;
        case 10:
            if (selected == 0)
            {
                standing_desks();
                break;
            }
        }
    }

    endwin();
    return 0;
}
