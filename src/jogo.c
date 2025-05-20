#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ncursesw/ncurses.h>
#include <time.h>
#include <locale.h>
#include <wchar.h>
#include <unistd.h>
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
int pontuacao = 1;

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
    pontuacao = 0;
}

void sortearBombas(int n) {
    int i;
    srand(time(NULL));
    for(i = 1; i <= n; i++) {
        l = rand() % TAM;
        c = rand() % TAM;
        if(jogo[l][c].eBomba == 0) {
            jogo[l][c].eBomba = 1;
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

void animarExplosao() {
    int frames = 30;
    int offsetX = 0, offsetY = 0;
    
    const char* explosion[] = {
        "⣿⣿⣿⣿⣿⣿⣿⣿⠿⢛⣛⡭⠙⣉⣉⣉⠙⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿",
        "⣿⣿⣿⣿⣿⡿⢋⣴⣀⣿⣿⣷⣴⣿⣿⣿⣾⣦⣈⠙⠛⠿⣿⣿⣿⣿⣿",
        "⣿⡿⢋⣉⠋⣄⣿⣿⣯⠉⣡⣾⣿⡿⣿⣿⣿⠿⣿⣦⣿⣆⡉⠿⣿⣿⣿",
        "⡟⢀⣿⣿⣨⣿⣿⣿⣿⣼⣿⣿⡏⠰⣾⣿⣿⣷⢰⣶⣿⣿⣇⢰⡜⣿⣿",
        "⠅⣽⣿⠩⣿⣿⣿⣿⡿⢿⡿⠆⠙⢂⠙⠋⠛⠿⡟⢿⣿⡿⣿⣮⣃⠸⣿",
        "⣆⢠⢻⡷⠖⣿⣿⣍⣁⣤⢤⣴⣶⣿⢿⡿⠀⣦⣁⠙⢯⡀⢾⣿⢿⡇⢿",
        "⣿⡀⠈⢿⣿⡥⢿⣷⡶⢌⡊⢻⡟⣿⣸⠀⠀⣭⣥⣲⣾⡿⠛⢏⣤⣤⣾",
        "⣿⣿⣷⣤⣤⣴⣷⣶⣶⣦⣬⠈⣇⠃⠇⡄⠤⣬⣉⣡⣬⣤⣶⣿⣿⣿⣿",
        "⣿⣿⣿⣿⣿⡿⠟⢋⠡⠒⠀⢻⣴⢸⡇⡚⢂⡌⠛⢿⣿⣿⣿⣿⣿⣿⣿",
        "⣿⣿⣿⣿⣿⣯⠐⢾⠶⣤⡰⠇⠸⠈⠶⠀⢋⡘⣷⣵⣆⠘⢿⣿⣿⣿⣿",
        "⣿⣿⣿⣿⣿⣿⣦⣉⠈⠹⠿⠳⣾⣧⠾⠗⢿⡷⠿⠏⠟⠀⣸⣿⣿⣿⣿",
        "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣤⠀⠈⣤⢴⠰⣦⣀⣤⣶⣶⣾⣿⣿⣿⣿⣿",
        "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⡄⣿⢸⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿",
        "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠀⡇⣼⢸⡄⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿",
        "⣿⣿⣿⣿⣿⣿⣿⣿⢿⠋⠀⣠⣿⣿⠈⣷⡀⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿",
        "⣿⣿⣿⣿⣿⣿⣿⣿⣯⣴⣾⣟⣼⣿⣷⣿⣷⣦⣽⣿⣿⣿⣿⣿⣿⣿⣿"
    };
    
    int art_height = 16;
    int art_width = 60;
    
    int term_height, term_width;
    getmaxyx(stdscr, term_height, term_width);
    
    int centerX = (term_width - art_width) / 2;
    int centerY = (term_height - art_height) / 2;

    for(int frame = 0; frame < frames; frame++) {
        clear();
        
        offsetX = (rand() % 3) - 1;
        offsetY = (rand() % 3) - 1;

        box(stdscr, 0, 0);
        
        attron(COLOR_PAIR(2) | A_BOLD);
        for(int i = 0; i < art_height; i++) {
            int yPos = centerY + i + offsetY;
            int xPos = centerX + offsetX;
            
            if(yPos >= 1 && yPos < term_height - 1) {
                move(yPos, xPos);
                addstr(explosion[i]);
            }
        }
        attroff(COLOR_PAIR(2) | A_BOLD);
        
        refresh();
        usleep(50000);
    }
}

void abrirCelula(int l, int c) {
    if(!coordenadaEhValida(l, c) || jogo[l][c].estaAberta)
        return;

    jogo[l][c].estaAberta = 1;

    if(jogo[l][c].eBomba) {
        game_over = 1;
        pontuacao -= 100;
        animarExplosao();
        for(int i = 0; i < TAM; i++) {
            for(int j = 0; j < TAM; j++) {
                if(jogo[i][j].eBomba)
                    jogo[i][j].estaAberta = 1;
            }
        }
        return;
    }

    pontuacao += 10;

    if(jogo[l][c].vizinhos == 0) {
        pontuacao += 50;
        abrirCelula(l-1, c);
        abrirCelula(l+1, c);
        abrirCelula(l, c-1);
        abrirCelula(l, c+1);
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
                if(l == linha_selecionada && c == coluna_selecionada && !game_over && !vitoria) {
                    attron(COLOR_PAIR(3) | A_BOLD);
                    if(jogo[l][c].eBomba) {
                        addstr(" * ");
                    } else if(jogo[l][c].vizinhos > 0) {
                        printw(" %d ", jogo[l][c].vizinhos);
                    } else {
                        addstr("   ");
                    }
                    attroff(COLOR_PAIR(3) | A_BOLD);
                } else {
                    if(jogo[l][c].eBomba) {
                        attron(COLOR_PAIR(5) | A_BOLD);
                        addstr(" * ");
                        attroff(COLOR_PAIR(5) | A_BOLD);
                    } else if(jogo[l][c].vizinhos > 0) {
                        attron(COLOR_PAIR(1));
                        printw(" %d ", jogo[l][c].vizinhos);
                        attroff(COLOR_PAIR(1));
                    } else {
                        addstr("   ");
                    }
                }
            } else {
                if(l == linha_selecionada && c == coluna_selecionada && !game_over && !vitoria) {
                    attron(COLOR_PAIR(3) | A_BOLD);
                    addstr(" ◆ ");
                    attroff(COLOR_PAIR(3) | A_BOLD);
                } else {
                    attron(COLOR_PAIR(4) | A_BOLD);
                    addstr(" ◆ ");
                    attroff(COLOR_PAIR(4) | A_BOLD);
                }
            }
        }
    }

    move(TAM*2 + 3, 2);
    printw("Pontuacao: %d", pontuacao);

    move(TAM*2 + 4, 2);
    if(game_over) {
        attron(COLOR_PAIR(5) | A_BOLD);
        printw("GAME OVER! Voce acertou uma bomba!");
        attroff(COLOR_PAIR(5) | A_BOLD);
        move(TAM*2 + 6, 2);
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

void desenharMenu() {
    clear();
    attron(A_BOLD);
    box(stdscr, 0, 0);
    attroff(A_BOLD);

    int term_height, term_width;
    getmaxyx(stdscr, term_height, term_width);

    const char *title[] = {
        "███▄ ▄███▓ ██▓ ███▄    █  ▄▄▄      ▓█████▄  ▒█████   ██▀███  ",
        "▓██▒▀█▀ ██▒▓██▒ ██ ▀█   █ ▒████▄    ▒██▀ ██▌▒██▒  ██▒▓██ ▒ ██▒",
        "▓██    ▓██░▒██▒▓██  ▀█ ██▒▒██  ▀█▄  ░██   █▌▒██░  ██▒▓██ ░▄█ ▒",
        "▒██    ▒██ ░██░▓██▒  ▐▌██▒░██▄▄▄▄██ ░▓█▄   ▌▒██   ██░▒██▀▀█▄  ",
        "▒██▒   ░██▒░██░▒██░   ▓██░ ▓█   ▓██▒░▒████▓ ░ ████▓▒░░██▓ ▒██▒",
        "░ ▒░   ░  ░░▓  ░ ▒░   ▒ ▒  ▒▒   ▓▒█░ ▒▒▓  ▒ ░ ▒░▒░▒░ ░ ▒▓ ░▒▓░",
        "░  ░      ░ ▒ ░░ ░░   ░ ▒░  ▒   ▒▒ ░ ░ ▒  ▒   ░ ▒ ▒░   ░▒ ░ ▒░",
        "░      ░    ▒ ░   ░   ░ ░   ░   ▒    ░ ░  ░ ░ ░ ░ ▒    ░░   ░ ",
        "       ░    ░           ░       ░  ░   ░        ░ ░     ░     ",
        "                                      ░                        "
    };

    int art_height = 9;
    int art_width = 65;
    int start_y = (term_height / 4) - (art_height / 2);
    int start_x = (term_width - art_width) / 2;

    if (start_x < 1) start_x = 1;
    if (start_y < 1) start_y = 1;

    attron(COLOR_PAIR(4) | A_BOLD);
    for(int i = 0; i < art_height; i++) {
        mvprintw(start_y + i, start_x, "%s", title[i]);
    }
    attroff(COLOR_PAIR(4) | A_BOLD);

    attron(COLOR_PAIR(1));
    mvprintw(term_height/2 + 4, (term_width-15)/2, "1. Novo Jogo");
    mvprintw(term_height/2 + 6, (term_width-15)/2, "2. Como Jogar");
    mvprintw(term_height/2 + 8, (term_width-15)/2, "3. Sair");
    attroff(COLOR_PAIR(1));

    refresh();
}

void mostrarInstrucoes() {
    clear();
    box(stdscr, 0, 0);

    attron(COLOR_PAIR(4) | A_BOLD);
    mvprintw(2, 2, "Como Jogar:");
    attroff(COLOR_PAIR(4) | A_BOLD);

    attron(COLOR_PAIR(1));
    mvprintw(4, 2, "- Use as setas do teclado para mover o cursor");
    mvprintw(5, 2, "- Pressione ENTER para revelar uma celula");
    mvprintw(6, 2, "- Evite as bombas!");
    mvprintw(7, 2, "- Revele todas as celulas sem bombas para vencer");
    mvprintw(9, 2, "Pressione qualquer tecla para voltar ao menu...");
    attroff(COLOR_PAIR(1));

    refresh();
    getch();
}

void desenharSelecaoDificuldade() {
    clear();
    attron(A_BOLD);
    box(stdscr, 0, 0);
    attroff(A_BOLD);

    int term_height, term_width;
    getmaxyx(stdscr, term_height, term_width);

    attron(COLOR_PAIR(4) | A_BOLD);
    mvprintw(term_height/4, (term_width-25)/2, "SELECIONE A DIFICULDADE");
    attroff(COLOR_PAIR(4) | A_BOLD);

    attron(COLOR_PAIR(1));
    mvprintw(term_height/2 - 2, (term_width-35)/2, "1. Fácil    (15 bombas)");
    mvprintw(term_height/2, (term_width-35)/2, "2. Médio    (20 bombas)");
    mvprintw(term_height/2 + 2, (term_width-35)/2, "3. Difícil  (30 bombas)");
    mvprintw(term_height/2 + 4, (term_width-35)/2, "ESC - Voltar ao menu");
    attroff(COLOR_PAIR(1));

    refresh();
}

void executarJogo() {
    static int ch = 0;
    int estado = 0;
    int num_bombas = 20;
    
    setlocale(LC_ALL, "");
    initscr();

    if(has_colors()) {
        start_color();
        init_pair(1, COLOR_BLUE, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_BLACK, COLOR_WHITE);
        init_pair(4, COLOR_GREEN, COLOR_BLACK);
        init_pair(5, COLOR_RED, COLOR_BLACK);
    }

    use_default_colors();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    keyboardInit();
    resize_term(TAM*2 + 30, TAM*4 + 10);

    while(ch != 27) {
        if(estado == 0) {
            desenharMenu();
            ch = getch();
            
            switch(ch) {
                case '1':
                    estado = 1;
                    break;
                case '2':
                    mostrarInstrucoes();
                    break;
                case '3':
                    ch = 27;
                    break;
            }
        }
        else if(estado == 1) {
            desenharSelecaoDificuldade();
            ch = getch();
            
            switch(ch) {
                case '1':
                    num_bombas = 15;
                    estado = 2;
                    inicializarJogo();
                    sortearBombas(num_bombas);
                    contarBombas();
                    desenharJogo();
                    break;
                case '2':
                    num_bombas = 20;
                    estado = 2;
                    inicializarJogo();
                    sortearBombas(num_bombas);
                    contarBombas();
                    desenharJogo();
                    break;
                case '3':
                    num_bombas = 30;
                    estado = 2;
                    inicializarJogo();
                    sortearBombas(num_bombas);
                    contarBombas();
                    desenharJogo();
                    break;
                case 27:
                    estado = 0;
                    break;
            }
        }
        else if(estado == 2) {
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
                    else if(ch == 27) {
                        estado = 0;
                        continue;
                    }
                }
                else if((game_over || vitoria) && ch == '1') {
                    estado = 0;
                    continue;
                }

                if(houve_mudanca) {
                    desenharJogo();
                }
            }
            napms(50);
        }
    }

    keyboardDestroy();
    endwin();
}



