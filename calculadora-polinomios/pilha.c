/*** Implementacao de uma pilha ***/
#include <stdlib.h>
#include <stdio.h>
#include "pilha.h"

struct pilha {
        Item *buffer;
        int capacidade;
        int topo;
};

Pilha cria(int n)
{
        Pilha p;
        
        p = malloc(sizeof(struct pilha));
        p->buffer = malloc(n * sizeof(Item));
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
