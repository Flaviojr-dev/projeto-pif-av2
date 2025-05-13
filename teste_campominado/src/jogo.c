#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ncursesw/ncurses.h>
#include <time.h>
#include <locale.h>
#include <wchar.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define TAM 15

typedef struct {
    int eBomba;
    int estaAberta;
    int vizinhos;
} Celula;

Celula jogo[TAM][TAM];
int l, c;
int linha_selecionada = 0, coluna_selecionada = 0;
int game_over = 0;
int vitoria = 0;

void inicializarJogo() {
    for(l = 0; l < TAM; l++) {
        for(c = 0; c < TAM; c++) {
            jogo[l][c].eBomba = 0;
            jogo[l][c].estaAberta = 0;
            jogo[l][c].vizinhos = 0;
        }
    }
    linha_selecionada = coluna_selecionada = 0;
    game_over = vitoria = 0;
}

void sortearBombas(int n) {
    int i, bombas = 0;
    srand(time(NULL));
    for(i = 1; i <= n; i++) {
        l = rand() % TAM;
        c = rand() % TAM;
        if(jogo[l][c].eBomba == 0) {
            jogo[l][c].eBomba = 1;
            bombas++;
        }
        else
            i--;
    }
}

int coordenadaEhValida(int l, int c) {
    return (l >= 0 && l < TAM && c >= 0 && c < TAM);
}

int quantBombasVizinhas(int l, int c) {
    int quantidade = 0;
    if(coordenadaEhValida(l-1, c) && jogo[l-1][c].eBomba) quantidade++;
    if(coordenadaEhValida(l+1, c) && jogo[l+1][c].eBomba) quantidade++;
    if(coordenadaEhValida(l, c+1) && jogo[l][c+1].eBomba) quantidade++;
    if(coordenadaEhValida(l, c-1) && jogo[l][c-1].eBomba) quantidade++;
    return quantidade;
}

void contarBombas() {
    for(l = 0; l < TAM; l++) {
        for(c = 0; c < TAM; c++) {
            jogo[l][c].vizinhos = quantBombasVizinhas(l, c);
        }
    }
}

void abrirCelula(int l, int c) {
    if(coordenadaEhValida(l, c) && !jogo[l][c].estaAberta) {
        jogo[l][c].estaAberta = 1;
        if(jogo[l][c].eBomba) {
            game_over = 1;
            for(int i = 0; i < TAM; i++) {
                for(int j = 0; j < TAM; j++) {
                    if(jogo[i][j].eBomba) jogo[i][j].estaAberta = 1;
                }
            }
        }
        else if(jogo[l][c].vizinhos == 0) {
            abrirCelula(l-1, c);
            abrirCelula(l+1, c);
            abrirCelula(l, c+1);
            abrirCelula(l, c-1);
        }
    }
}

int ganhou() {
    for(l = 0; l < TAM; l++) {
        for(c = 0; c < TAM; c++) {
            if(!jogo[l][c].estaAberta && !jogo[l][c].eBomba)
                return 0;
        }
    }
    vitoria = 1;
    return 1;
}

void desenharJogo() {
    clear();
    
    attron(A_BOLD);
    box(stdscr, 0, 0);
    attroff(A_BOLD);
    
    for(l = 0; l < TAM; l++) {
        for(c = 0; c < TAM; c++) {
            move(l*2 + 2, c*4 + 2);
            
            if(jogo[l][c].estaAberta) {
                if(jogo[l][c].eBomba) {
                    attron(COLOR_PAIR(2) | A_BOLD);
                    addstr("*");
                    attroff(COLOR_PAIR(2) | A_BOLD);
                } else {
                    if(jogo[l][c].vizinhos > 0) {
                        attron(COLOR_PAIR(1));
                        printw("%d", jogo[l][c].vizinhos);
                        attroff(COLOR_PAIR(1));
                    } else {
                        addstr(" ");
                    }
                }
            } else {
                if(l == linha_selecionada && c == coluna_selecionada && !game_over && !vitoria) {
                    attron(COLOR_PAIR(3) | A_BOLD);
                    addstr("◇");
                    attroff(COLOR_PAIR(3) | A_BOLD);
                } else {
                    attron(COLOR_PAIR(4) | A_BOLD);
                    addstr("◆");
                    attroff(COLOR_PAIR(4) | A_BOLD);
                }
            }
        }
    }
    
    move(TAM*2 + 4, 2);
    if(game_over) {
        attron(COLOR_PAIR(2) | A_BOLD);
        printw("GAME OVER! Voce acertou uma bomba!");
        attroff(COLOR_PAIR(2) | A_BOLD);
        move(TAM*2 + 5, 2);
        printw("Aperte a tecla 1 para jogar novamente");
    }
    else if(vitoria) {
        attron(COLOR_PAIR(4) | A_BOLD);
        printw("PARABENS! VOCE GANHOU!");
        attroff(COLOR_PAIR(4) | A_BOLD);
        move(TAM*2 + 5, 2);
        printw("Aperte a tecla 1 para jogar novamente");
    }
    else {
        printw("Use as setas para navegar. ENTER para abrir celula.");
    }
    
    refresh();
}

void executarJogo() {
    static int ch = 0;
    
    setlocale(LC_ALL, "");
    
    initscr();
    
    if(has_colors()) {
        start_color();
        init_pair(1, COLOR_BLUE, COLOR_BLACK);
        init_pair(2, COLOR_RED, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_BLUE);
        init_pair(4, COLOR_GREEN, COLOR_BLACK);
    }
    
    use_default_colors();
    
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    
    keyboardInit();
    
    resize_term(TAM*2 + 6, TAM*4 + 4);
    
    inicializarJogo();
    sortearBombas(20);
    contarBombas();
    
    box(stdscr, 0, 0);
    
    desenharJogo();
    
    while(ch != 27) {
        if(keyhit()) {
            ch = getch();
            int houve_mudanca = 0;
            
            if(!game_over && !vitoria) {
                if(ch == KEY_UP && linha_selecionada > 0) {
                    linha_selecionada--;
                    houve_mudanca = 1;
                }
                else if(ch == KEY_DOWN && linha_selecionada < TAM-1) {
                    linha_selecionada++;
                    houve_mudanca = 1;
                }
                else if(ch == KEY_LEFT && coluna_selecionada > 0) {
                    coluna_selecionada--;
                    houve_mudanca = 1;
                }
                else if(ch == KEY_RIGHT && coluna_selecionada < TAM-1) {
                    coluna_selecionada++;
                    houve_mudanca = 1;
                }
                else if(ch == 10) {
                    abrirCelula(linha_selecionada, coluna_selecionada);
                    ganhou();
                    houve_mudanca = 1;
                }
            }
            else if((game_over || vitoria) && ch == '1') {
                inicializarJogo();
                sortearBombas(20);
                contarBombas();
                houve_mudanca = 1;
            }
            
            if(houve_mudanca) {
                desenharJogo();
            }
        }
        
        napms(50);
    }
    
    keyboardDestroy();
    endwin();
    return;
}
