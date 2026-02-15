#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TB_HEIGHT 3
#define TB_WIDTH 18
#define INNER_COLOR_PAIR 1
#define RAIN_COLOR_PAIR 2
#define MAX_RAIN 300

#define VX 0
#define VY 1
#define RAIN_CHARACTER "|"

typedef struct {
    int x,y, vx, vy;
    int color;
} Raindrop;

Raindrop rain[MAX_RAIN];

void initRain()
{   
    int y_max, x_max;
    getmaxyx(stdscr, y_max, x_max);

    for (int i=0; i < MAX_RAIN; i++){
        rain[i].x = (rand() % x_max) + 1;
        rain[i].y = (rand() % y_max);
        rain[i].vy = VY;
        rain[i].vx = VX;
    }
}

void drawRain()
{   
    int y_max, x_max;
    getmaxyx(stdscr, y_max, x_max);
    char* drop = RAIN_CHARACTER;

    attron(COLOR_PAIR(RAIN_COLOR_PAIR));

    for (int i=0; i < MAX_RAIN; i++){
        Raindrop *curr = &(rain[i]);

        curr->y += curr->vy;
        curr->x += curr->vx;

        if (curr->y >= y_max - 1){
            curr->y = 1;
            curr->x = rand() % (x_max - 2) + 1;
            curr->vy = (rand() % 2) + 1;
        }

        if (curr->x >= x_max - 1){
            curr->x = rand() % (x_max) + 1;
            curr->y = 0;
        }

        if (curr->y >= 0 && curr->y < y_max && curr->x >= 0 && curr->x < x_max){
            mvwaddstr(stdscr, curr->y, curr->x, drop);
        }
    }

    attroff(COLOR_PAIR(RAIN_COLOR_PAIR));
}

void initColorPairs()
{
    init_pair(INNER_COLOR_PAIR, COLOR_BLACK, COLOR_CYAN);
    init_pair(RAIN_COLOR_PAIR, COLOR_BLUE, COLOR_BLACK);
}


void drawOuterBox()
{
    box(stdscr, 0, 0);
    wnoutrefresh(stdscr);
}

void drawInnerBox()
{   
    int y_max, x_max;

    getmaxyx(stdscr, y_max, x_max);
    WINDOW *innerWin = newwin(TB_HEIGHT, TB_WIDTH, (y_max-TB_HEIGHT)/2, (x_max-TB_WIDTH)/2);

    wattron(innerWin, COLOR_PAIR(INNER_COLOR_PAIR));
    box(innerWin, 0, 0);
    char* innerWinStr = "////Raindrop\\\\\\\\";
    mvwaddstr(innerWin, (TB_HEIGHT/2), (TB_WIDTH-strlen(innerWinStr))/2, innerWinStr);

    wattroff(innerWin, COLOR_PAIR(INNER_COLOR_PAIR));

    wnoutrefresh(innerWin);
    delwin(innerWin);
}

int main()
{
    initscr();
    if (start_color() == ERR){
        endwin();
        return 1;
    }

    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);

    initRain();

    initColorPairs();

    while (getch() != 'q'){
        erase();
        drawOuterBox();
        drawRain();
        wnoutrefresh(stdscr);
        drawInnerBox();
        doupdate();
        usleep(75000);
    }

    endwin();

    return 0;
}