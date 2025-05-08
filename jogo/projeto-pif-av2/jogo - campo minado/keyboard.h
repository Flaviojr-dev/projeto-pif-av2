#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <curses.h>

#define KEY_ESC 27
#define KEY_ENTER 10

void keyboardInit();
void keyboardDestroy();
int keyhit();
int readch();

#endif