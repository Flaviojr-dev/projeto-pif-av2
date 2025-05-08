#include <ncurses.h>
#include <sys/time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

static struct timeval timer_start;
static int timer_interval;

void screenInit(int useColor) {
    initscr();
    if(useColor) {
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_WHITE, COLOR_BLACK);
        init_pair(5, COLOR_WHITE, COLOR_BLUE);
    }
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void screenDestroy() {
    endwin();
}

void screenClear() {
    clear();
}

void screenUpdate() {
    refresh();
}

void screenGotoxy(int x, int y) {
    move(y, x);
}

void screenSetColor(int fg, int bg) {
    if(fg == WHITE && bg == DARKGRAY) {
        attron(COLOR_PAIR(5));
    } else if(fg == RED) {
        attron(COLOR_PAIR(1));
    } else if(fg == GREEN) {
        attron(COLOR_PAIR(2));
    } else if(fg == YELLOW) {
        attron(COLOR_PAIR(3));
    } else {
        attron(COLOR_PAIR(4));
    }
}

void screenDrawBorders(int x1, int y1, int x2, int y2) {
    int i;
    for(i = x1; i <= x2; i++) {
        mvaddch(y1, i, '-');
        mvaddch(y2, i, '-');
    }
    for(i = y1; i <= y2; i++) {
        mvaddch(i, x1, '|');
        mvaddch(i, x2, '|');
    }
    mvaddch(y1, x1, '+');
    mvaddch(y1, x2, '+');
    mvaddch(y2, x1, '+');
    mvaddch(y2, x2, '+');
}

void keyboardInit() {
    // Already initialized by screenInit
}

void keyboardDestroy() {
    // Will be destroyed by screenDestroy
}

int keyhit() {
    nodelay(stdscr, TRUE);
    int ch = getch();
    nodelay(stdscr, FALSE);
    return ch != ERR;
}

int readch() {
    return getch();
}

void timerInit(int ms) {
    timer_interval = ms * 1000; // Convert to microseconds
    gettimeofday(&timer_start, NULL);
}

void timerDestroy() {
    // Nothing to destroy
}

int timerTimeOver() {
    struct timeval now;
    gettimeofday(&now, NULL);
    long elapsed = (now.tv_sec - timer_start.tv_sec) * 1000000 + (now.tv_usec - timer_start.tv_usec);
    if(elapsed >= timer_interval) {
        gettimeofday(&timer_start, NULL);
        return 1;
    }
    return 0;
}