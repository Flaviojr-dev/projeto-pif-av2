#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <stdio.h>

#define ESC            "\033"
#define NORMALTEXT     "[0m"
#define BOLDTEXT       "[1m"
#define ITALICTEXT     "[3m"
#define BLINKTEXT      "[5m"
#define REVERSETEXT    "[7m"
#define HOMECURSOR     "[f"
#define SHOWCURSOR     "[?25h"
#define HIDECURSOR     "[?25l"
#define CLEARSCREEN    "[2J"
#define BOX_ENABLE     "(0"
#define BOX_DISABLE    "(B"
#define BOX_VLINE      0x78
#define BOX_HLINE      0x71
#define BOX_UPLEFT     0x6C
#define BOX_UPRIGHT    0x6B
#define BOX_DWNLEFT    0x6D
#define BOX_DWNRIGHT   0x6A
#define BOX_CROSS      0x6E
#define BOX_TLEFT      0X74
#define BOX_TRIGHT     0X75
#define BOX_TUP        0X77
#define BOX_TDOWN      0X76
#define BOX_DIAMOND    0x60
#define BOX_BLOCK      0x61
#define BOX_DOT        0x7E
#define SCRSTARTX      3
#define SCRENDX        75
#define SCRSTARTY      1
#define SCRENDY        23
#define MINX           1
#define MINY           1
#define MAXX           80
#define MAXY           24

typedef enum {BLACK, RED, GREEN, BROWN, BLUE, MAGENTA, CYAN, LIGHTGRAY,
        DARKGRAY, LIGHTRED, LIGHTGREEN, YELLOW, LIGHTBLUE, 
        LIGHTMAGENTA, LIGHTCYAN, WHITE} screenColor;

void screenDrawBorders(int x1, int y1, int x2, int y2);

static inline void screenHomeCursor() {
    printf("%s%s", ESC, HOMECURSOR);
}

static inline void screenShowCursor() {
    printf("%s%s", ESC, SHOWCURSOR);
}

static inline void screenHideCursor() {
    printf("%s%s", ESC, HIDECURSOR);
}

void screenClear(void);
void screenUpdate(void);

static inline void screenSetNormal() {
    printf("%s%s", ESC, NORMALTEXT);
}

static inline void screenSetBold() {
    printf("%s%s", ESC, BOLDTEXT);
}

static inline void screenSetBlink() {
    printf("%s%s", ESC, BLINKTEXT);
}

static inline void screenSetReverse() {
    printf("%s%s", ESC, REVERSETEXT);
}

static inline void screenBoxEnable() {
    printf("%s%s", ESC, BOX_ENABLE);
}

static inline void screenBoxDisable() {
    printf("%s%s", ESC, BOX_DISABLE);
}

void screenInit(int drawBorders);
void screenDestroy(void);
void screenGotoxy(int x, int y);
void screenSetColor(screenColor fg, screenColor bg);

#endif