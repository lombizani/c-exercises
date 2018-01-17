/*** Definicao da interface da biblioteca pilha ***/
#include "item.h"

/* Este e o nosso tipo de dados abstrato */
typedef struct pilha *Pilha;

/* Devolve uma pilha recém-criada, com capacidade para no máximo n Itens. */
Pilha cria(int n);

/* Devolve 1 se a pilha está vazia, 0 em caso contrário. */
int vazia(Pilha p);

/* Empilha o item passado como parâmtro. */
void empilha(Pilha p, Item item);

/* Devolve o Item que se estiver no topo da pilha, sem desempilhá-lo. */
Item espia_topo(Pilha p);

/* Desempilha um Item e o devolve como valor da função. */
Item desempilha(Pilha p);

/* Libera a memoria utilizada pela pilha p */
void destroi(Pilha p);
