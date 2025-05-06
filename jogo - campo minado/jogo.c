#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*
    CELULA
    eBomba 0 ou 1
    estaAberta 0 ou 1
    vizinhos 0 a 4
*/
typedef struct{
    int eBomba;
    int estaAberta;
    int vizinhos;
}Celula;

Celula jogo[10][10];
int l, c, tam = 10;

void inicializarJogo(){
    for(l = 0; l < tam; l++){
        for(c = 0; c < tam; c++){
            jogo[l][c].eBomba = 0;
            jogo[l][c].estaAberta = 0;
            jogo[l][c].vizinhos = 0;
        }
    }
}

void sortearBombas(int n){
    int i, bombas = 0;
    srand(time(NULL));
    for(i = 1; i <= n; i++){
        l = rand() % tam;
        c = rand() % tam;
        if(jogo[l][c].eBomba == 0){
            jogo[l][c].eBomba = 1;
            bombas++;
        }
        else
            i--;
    }
}

int coordenadaEhValida(int l, int c){
    if(l >= 0 && l < tam && c >= 0 && c < tam)
        return 1;
    else
        return 0;
}

int quantBombasVizinhas(int l, int c){
    /*  l - 1 e c
        l + 1 e c
        l e c + 1
        l e c - 1
    */
    int quantidade = 0;
    if(coordenadaEhValida(l - 1, c) && jogo[l-1][c].eBomba)
        quantidade++;
    if(coordenadaEhValida(l + 1, c) && jogo[l+1][c].eBomba)
        quantidade++;
    if(coordenadaEhValida(l, c + 1) && jogo[l][c+1].eBomba)
        quantidade++;
    if(coordenadaEhValida(l, c - 1) && jogo[l][c-1].eBomba)
        quantidade++;
    return quantidade;
}

void contarBombas(){
    for(l = 0; l < tam; l++){
        for(c = 0; c < tam; c++)
            jogo[l][c].vizinhos = quantBombasVizinhas(l, c);
    }
}