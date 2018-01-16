/**********************************************************
 * Aluno: Carlos Filipe Lombizani De Bernardis            *
 * Numero USP: 6430536                                    *
 * Exercicio-Programa 2 -- Polinomios Esparsos: O Retorno *
 * MAC0122 -- BMAC -- 2008 -- Prof.: Francisco Reverbel   *
 * Compilador: gcc 4.2          Arquivo: pilha.c          *
 **********************************************************/

/*** Implementacao da biblioteca pilha ***/
#include <stdlib.h>
#include <stdio.h>
#include "pilha.h"
#include "mallocX.h"

struct pilha {
        Item *buffer;
        int capacidade;
        int topo;
};

Pilha cria(int n)
{
        Pilha p;
        
        p = mallocX(sizeof(struct pilha));
        p->buffer = mallocX(n * sizeof(Item));
        p->capacidade = n;
        p->topo = 0;
        
        return p;
}

int vazia(Pilha p)
{
        return(p->topo == 0);
}

void empilha(Pilha p, Item item)
{
        if(p->topo == p->capacidade) {
                fputs("Erro de estouro de pilha. Programa finalizado.\n", stderr);
                exit(EXIT_FAILURE);
        }
        p->buffer[(p->topo)++] = item;
}

Item espia_topo(Pilha p)
{
        if(p->topo == 0) {
                fputs("Erro de esvaziamento de pilha. Programa finalizado.\n", stderr);
                exit(EXIT_FAILURE);
        }
        return p->buffer[p->topo - 1];
}

Item desempilha(Pilha p)
{
        if(p->topo == 0) {
                fputs("Erro de esvaziamento de pilha. Programa finalizado.\n", stderr);
                exit(EXIT_FAILURE);
        }
        return p->buffer[--(p->topo)];
}

void destroi(Pilha p)
{
        free(p->buffer);
        free(p);
}
