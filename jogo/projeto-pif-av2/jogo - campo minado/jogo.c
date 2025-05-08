#include <string.h>
#include <stdlib.h>
#include <time.h>
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
    screenClear();
    
    for(l = 0; l < TAM; l++) {
        for(c = 0; c < TAM; c++) {
            screenSetColor(WHITE, DARKGRAY);
            
            if(l == linha_selecionada && c == coluna_selecionada && !game_over && !vitoria) {
                screenSetColor(YELLOW, DARKGRAY);
            }
            
            if(jogo[l][c].estaAberta) {
                if(jogo[l][c].eBomba) {
                    screenSetColor(RED, BLACK);
                    screenGotoxy(c*4 + 2, l*2 + 2);
                    printw("*");
                }
                else {
                    screenSetColor(WHITE, BLACK);
                    screenGotoxy(c*4 + 2, l*2 + 2);
                    if(jogo[l][c].vizinhos > 0) {
                        printw("%d", jogo[l][c].vizinhos);
                    } else {
                        printw(" ");
                    }
                }
            }
            else {
                screenGotoxy(c*4 + 2, l*2 + 2);
                printw("#");
            }
        }
    }
    
    screenSetColor(WHITE, BLACK);
    screenDrawBorders(0, 0, TAM*4 + 3, TAM*2 + 3);
    
    if(game_over) {
        screenSetColor(RED, BLACK);
        screenGotoxy(2, TAM*2 + 4);
        printw("GAME OVER! Voce acertou uma bomba!");
        
        screenSetColor(WHITE, BLACK);
        screenGotoxy(2, TAM*2 + 5);
        printw("Aperte a tecla 1 para jogar novamente");
    }
    else if(vitoria) {
        screenSetColor(GREEN, BLACK);
        screenGotoxy(2, TAM*2 + 4);
        printw("PARABENS! VOCE GANHOU!");
        
        screenSetColor(WHITE, BLACK);
        screenGotoxy(2, TAM*2 + 5);
        printw("Aperte a tecla 1 para jogar novamente");
    }
    else {
        screenSetColor(WHITE, BLACK);
        screenGotoxy(2, TAM*2 + 4);
        printw("Use as setas para navegar. ENTER para abrir celula.");
    }
}

int main() {
    static int ch = 0;
    
    
    inicializarJogo();
    sortearBombas(20);
    contarBombas();
    
    
    screenInit(1);
    keyboardInit();
    timerInit(50);
    
    while(ch != 27) { 
        if(keyhit()) {
            ch = readch();
            
            if(!game_over && !vitoria) {
                
                if(ch == KEY_UP && linha_selecionada > 0) {
                    linha_selecionada--;
                }
                else if(ch == KEY_DOWN && linha_selecionada < TAM-1) {
                    linha_selecionada++;
                }
                else if(ch == KEY_LEFT && coluna_selecionada > 0) {
                    coluna_selecionada--;
                }
                else if(ch == KEY_RIGHT && coluna_selecionada < TAM-1) {
                    coluna_selecionada++;
                }
                else if(ch == 10) { 
                    abrirCelula(linha_selecionada, coluna_selecionada);
                    ganhou();
                }
            }
            else if((game_over || vitoria) && ch == '1') {
                inicializarJogo();
                sortearBombas(20);
                contarBombas();
            }
        }
        
        if(timerTimeOver() == 1) {
            desenharJogo();
            screenUpdate();
        }
    }
    
    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    
    return 0;
}
