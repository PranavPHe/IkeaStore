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

// Unique Values
typedef enum
{
    SIZE,
    RANGE,
    MATERIAL,
    COLOR
} ProductType;

// A struct which will hold all the needed values
typedef struct
{
    char name[50];
    int quantity;
    ProductType type;
    union
    {
        char size[10];
        int range;
        char material[10];
        char color[10];
    } info;
} Product;

// The maximum number of products(With sizes)
#define MAX_PRODUCTS 19
Product products[MAX_PRODUCTS];
int productCount = 0;

// Function to create the top banner
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

// Function that creates the menu/selectable items for each page
void menu(int y, int width, char **options, int quantity, int selected)
{
    // Print the box
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

    // Print the "Options" label
    attron(COLOR_PAIR(MESSAGE_TEXT_COLOR));
    mvprintw(y + 1, 1, "OPTIONS:");
    attroff(COLOR_PAIR(MESSAGE_TEXT_COLOR));

    // If selected, Highlight
    for (int i = 0; i < quantity; i++)
    {
        if (i == selected)
        {
            attron(A_BOLD | A_STANDOUT);
            mvprintw(i + y + 2, 1, "%-*.*s", width, width, options[i]);
            attroff(A_BOLD | A_STANDOUT);
        }
        else
        {
            mvprintw(i + y + 2, 1, "%-*.*s", width, width, options[i]);
        }
    }
    attroff(COLOR_PAIR(MESSAGE_BOX_COLOR));
}

// Function to confirm selected product
bool confirm(int row)
{
    int ch;
    mvprintw(row, 2, "Confirm? [");
    attron(COLOR_PAIR(CONFIRM_COLOR_Y));
    addch('Y');
    attroff(COLOR_PAIR(CONFIRM_COLOR_Y));
    printw("] or [");
    attron(COLOR_PAIR(MESSAGE_ERROR_COLOR));
    addch('N');
    attroff(COLOR_PAIR(MESSAGE_ERROR_COLOR));
    addch(']');

    ch = getch();
    return (ch == 'y');
}

// Initializing the products by their attributes
void initialize_products()
{
    char *sizeOptions[] = {"S", "M", "L"};
    char *materialOptions[] = {"Wood", "Marble", "Granite"};
    char *colorOptions[] = {"Almond", "Mahogany", "Drift Wood"};

    // Initialize Standing Desks
    for (int i = 0; i < 3; i++)
    {
        strcpy(products[i].name, "Standing Desks");
        products[i].type = SIZE;
        strcpy(products[i].info.size, sizeOptions[i]);
    }

    // Initialize Blackout Curtains
    for (int i = 0; i < 10; i++)
    {
        strcpy(products[3 + i].name, "Blackout Curtains");
        products[3 + i].type = RANGE;
        products[3 + i].info.range = 45 + i;
    }

    // Initialize Tabletops
    for (int i = 0; i < 3; i++)
    {
        strcpy(products[13 + i].name, "Tabletops");
        products[13 + i].type = MATERIAL;
        strcpy(products[13 + i].info.material, materialOptions[i]);
    }

    // Initialize Table Legs
    for (int i = 0; i < 3; i++)
    {
        strcpy(products[16 + i].name, "Table Legs");
        products[16 + i].type = COLOR;
        strcpy(products[16 + i].info.color, colorOptions[i]);
    }
}

void product_selection(Product *product)
{
    bool running = true;
    bool optionSelected = false;
    int selectedOption = 0;
    int quantity = 0;

    while (running)
    {
        clear();
        message(col, product->name);

        char *sizes[] = {"S", "M", "L"};
        char *ranges[] = {"45", "46", "47", "48", "49", "50", "51", "52", "53", "54"};
        char *materials[] = {"Wood", "Marble", "Granite"};
        char *colors[] = {"Almond", "Mahogany", "Drift Wood"};

        if (product->type == SIZE)
        {
            menu(3, 10, sizes, 3, selectedOption);
        }
        else if (product->type == RANGE)
        {
            menu(3, 10, ranges, 10, selectedOption);
        }
        else if (product->type == MATERIAL)
        {
            menu(3, 10, materials, 3, selectedOption);
        }
        else if (product->type == COLOR)
        {
            menu(3, 10, colors, 3, selectedOption);
        }

        if (strcmp(product->name, "Blackout Curtains") == 0)
        {
            mvprintw(20, 2, "[");
            attron(COLOR_PAIR(MESSAGE_TEXT_COLOR) | A_BOLD);
            printw("Q");
            attroff(COLOR_PAIR(MESSAGE_TEXT_COLOR) | A_BOLD);
            printw("] Back");
        }

        else
        {
            mvprintw(13, 2, "[");
            attron(COLOR_PAIR(MESSAGE_TEXT_COLOR) | A_BOLD);
            printw("Q");
            attroff(COLOR_PAIR(MESSAGE_TEXT_COLOR) | A_BOLD);
            printw("] Back");
        }

        if (optionSelected)
        {
            bool validInput = false;
            char input[10];

            while (!validInput)
            {
                if (strcmp(product->name, "Blackout Curtains") == 0)
                {
                    mvprintw(18, 2, "Enter Quantity: ");
                }
                else
                {
                    mvprintw(10, 2, "Enter Quantity: ");
                }

                clrtoeol();
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
                }
                else
                {
                    if (confirm(strcmp(product->name, "Blackout Curtains") == 0 ? 19 : 11))
                    {
                        // Find the correct product based on the selected option
                        for (int i = 0; i < MAX_PRODUCTS; i++)
                        {
                            if (strcmp(products[i].name, product->name) == 0)
                            {
                                if (product->type == SIZE && strcmp(products[i].info.size, sizes[selectedOption]) == 0)
                                {
                                    products[i].quantity += quantity;
                                    break;
                                }
                                else if (product->type == RANGE && products[i].info.range == 45 + selectedOption)
                                {
                                    products[i].quantity += quantity;
                                    break;
                                }
                                else if (product->type == MATERIAL && strcmp(products[i].info.material, materials[selectedOption]) == 0)
                                {
                                    products[i].quantity += quantity;
                                    break;
                                }
                                else if (product->type == COLOR && strcmp(products[i].info.color, colors[selectedOption]) == 0)
                                {
                                    products[i].quantity += quantity;
                                    break;
                                }
                            }
                        }
                        running = false;
                    }
                    else
                    {
                        optionSelected = false;
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
                optionSelected = true;
                break;
            case KEY_DOWN:
                selectedOption = min(product->type == RANGE ? 9 : 2, selectedOption + 1);
                break;
            case KEY_UP:
                selectedOption = max(0, selectedOption - 1);
                break;
            }
        }
    }
}

void view_cart()
{
    bool running = true;

    while (running)
    {
        clear();
        message(col, "View Cart");
        int line = 3;
        int itemIndex = 0;

        for (int i = 0; i < MAX_PRODUCTS; i++)
        {
            if (products[i].quantity > 0)
            {
                if (products[i].type == SIZE)
                {
                    mvprintw(line++, 3, "[%d] %s [%s]: %d", itemIndex++, products[i].name, products[i].info.size, products[i].quantity);
                }
                else if (products[i].type == RANGE)
                {
                    mvprintw(line++, 3, "[%d] %s [%d]: %d", itemIndex++, products[i].name, products[i].info.range, products[i].quantity);
                }
                else if (products[i].type == MATERIAL)
                {
                    mvprintw(line++, 3, "[%d] %s [%s]: %d", itemIndex++, products[i].name, products[i].info.material, products[i].quantity);
                }
                else if (products[i].type == COLOR)
                {
                    mvprintw(line++, 3, "[%d] %s [%s]: %d", itemIndex++, products[i].name, products[i].info.color, products[i].quantity);
                }
            }
        }

        // Remove or Continue
        int makeEdit = line + 1;

        // Display "Remove" option
        mvprintw(makeEdit, 2, "[");
        attron(COLOR_PAIR(MESSAGE_ERROR_COLOR) | A_BOLD);
        printw("X");
        attroff(COLOR_PAIR(MESSAGE_ERROR_COLOR) | A_BOLD);
        printw("] Remove");

        // Display "Check Out" option on the same line with enough space
        mvprintw(makeEdit, 20, "[");
        attron(COLOR_PAIR(CONFIRM_COLOR_Y) | A_BOLD);
        printw("C");
        attroff(COLOR_PAIR(CONFIRM_COLOR_Y) | A_BOLD);
        printw("] Check Out");

        // Back Button
        int backLine = line + 2;
        mvprintw(backLine, 2, "[");
        attron(COLOR_PAIR(MESSAGE_TEXT_COLOR) | A_BOLD);
        printw("Q");
        attroff(COLOR_PAIR(MESSAGE_TEXT_COLOR) | A_BOLD);
        printw("] Back");

        ch = getch();

        switch (ch)
        {
        case KEY_RESIZE:
            getmaxyx(stdscr, row, col);
            break;
        case 'q':
            running = false;
            break;
        case 'x':
            // Prompt user to enter the index of the item to remove
            mvprintw(makeEdit + 4, 2, "Enter the index of the item to remove: ");
            echo();
            char input[10];
            wgetnstr(stdscr, input, sizeof(input) - 1);
            noecho();

            int removeIndex;
            if (sscanf(input, "%d", &removeIndex) == 1 && removeIndex >= 0 && removeIndex < itemIndex)
            {
                // Confirm removal
                if (confirm(makeEdit + 6))
                {
                    // Find and remove the item
                    itemIndex = 0;
                    for (int i = 0; i < MAX_PRODUCTS; i++)
                    {
                        if (products[i].quantity > 0)
                        {
                            if (itemIndex == removeIndex)
                            {
                                products[i].quantity = 0;
                                break;
                            }
                            itemIndex++;
                        }
                    }
                }
            }
            break;
        }
    }
}

int main()
{
    bool running = true;
    int selected = 0;
    char *productNames[] = {"Standing Desks", "Blackout Curtains", "Tabletops", "Table Legs"};
    int productIndices[] = {0, 3, 13, 16};

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

    // Initialize products
    initialize_products();

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
                mvprintw(3 + i * 2, 2, "%s", productNames[i]);
                attroff(A_BOLD | A_STANDOUT);
            }
            else
            {
                mvprintw(3 + i * 2, 2, "%s", productNames[i]);
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
            product_selection(&products[productIndices[selected]]);
            break;
        case 'v':
            view_cart();
            break;
        }
    }

    endwin();
    return 0;
}
