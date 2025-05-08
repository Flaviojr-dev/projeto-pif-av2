#ifndef SCREEN_H
#define SCREEN_H

#include <curses.h>

// Cores
#define BLACK   0
#define RED     1
#define GREEN   2
#define YELLOW  3
#define WHITE   7
#define DARKGRAY 8

void screenInit(int useColor);
void screenDestroy();
void screenClear();
void screenUpdate();
void screenGotoxy(int x, int y);
void screenSetColor(int fg, int bg);
void screenDrawBorders(int x1, int y1, int x2, int y2);

#endif