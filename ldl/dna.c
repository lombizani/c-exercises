/***************************************************************/
/**                                                           **/
/** Carlos Filipe Lombizani                                   **/
/** Exercício-Programa 1 - Reversões em DNA                   **/
/** Professor: Eloi                                           **/
/** Turma: Matutino                                           **/
/**                                                           **/
/***************************************************************/
#include <stdlib.h>
#include <stdio.h>
#define MAXM 10000
#define MAXN 10000000
#define ERR 1

typedef struct dblNode {
    char c;
    struct dblNode *prev;
    struct dblNode *next;
} DoubleNode;

typedef DoubleNode *DoublyLList;

/* Devolve uma lista duplamente ligada (com cabeça e cauda) vazia */
DoublyLList newDblList()
{
    DoublyLList head = malloc(sizeof(DoubleNode));
    DoublyLList tail = malloc(sizeof(DoubleNode));
    
    head->next = tail;
    head->prev = NULL;
    tail->next = NULL;
    tail->prev = head;
    
    return head;
}

/* Recebe um nó de uma lista duplamente ligada e insere um nó contendo c entre
   ele o próximo nó, devolve o enderço do nó inserido */
DoublyLList dblAppendNode(DoublyLList node, char c)
{
    DoublyLList newNode = malloc(sizeof(DoubleNode));
    
    newNode->c = c;
    newNode->next = node->next;
    newNode->prev = node;
    node->next->prev = newNode;
    node->next = newNode;
    
    return newNode;
}

/* Percorre uma lista duplamente ligada e imprime o seu conteúdo como uma string
   na saída padrão */
void dblPrint(DoublyLList dblll)
{
    DoublyLList aux = dblll->next;
    
    while(aux != NULL && aux->next != NULL) {
        printf("%c", aux->c);
        if (aux->next != dblll) {
            dblll = aux;
            aux = aux->next;
        } else {
            dblll = aux;
            aux = aux->prev;
        }
    }
}

/* Libera uma lista duplamente ligada da memória */
void dblFree(DoublyLList dblll)
{
    DoublyLList aux = dblll->next;
    free(dblll);
    
    while(aux != NULL && aux->next != NULL) {
        if (aux->next != dblll) {
            dblll = aux;
            aux = aux->next;
        } else {
            dblll = aux;
            aux = aux->prev;
        }
        free(dblll);
    }
    if (aux != NULL) free(aux);
}

/* Recebe um índice i e uma matriz 2xn de reversões e devolve o índice do elemento
   que está na posição i */
int transform(int i, int **trans, int n)
{
    if (n == 0) return i;
    else {
        if (i >= trans[0][n-1] && i <= trans[1][n-1])
            i = trans[0][n-1] + trans[1][n-1] - i;
        return transform(i, trans, n - 1);
    }
}

int main()
{
    int i, /* contador de reversões */
        c, /* caracter recebido da entrada padrão */
        m, /* vamos ler aqui o número de reversões a serem feitas */
        j = 1; /* contador de caracteres */
    int *trans[2]; /* matriz de reversões */
    DoublyLList *dna; /* vetor para indexar nossa lista ligada */
    DoublyLList acid, /* nossa lista ligada de bases */
                aux; /* ponteiro auxiliar para percorrê-la */
    
    acid = newDblList();
    dna = malloc((MAXN + 2) * sizeof(DoublyLList));
    
    if (scanf("%d ", &m) != 1) return ERR;
    if (m > MAXM) return ERR;
    
    trans[0] = malloc(m * sizeof(int));
    trans[1] = malloc(m * sizeof(int));
    
    dna[0] = aux = acid;
    for (c = getchar(); c != '\n' && c != '\r' && c != EOF && j <= MAXN; c = getchar()) {
        dna[j++] = dblAppendNode(aux, c);
        aux = aux->next;
    }
    dna[j] = aux->next;
    
    for (i = 0; i < m; i++) {
        DoublyLList start, end, last; /* ponteiros para realizar a reversão */
    
        if (scanf("%d %d ", (trans[0] + i), (trans[1] + i)) != 2) return ERR;
        
        aux = dna[transform(trans[0][i] - 1, trans, i)];
        start = dna[transform(trans[0][i], trans, i)];
        end = dna[transform(trans[1][i], trans, i)];
        last = dna[transform(trans[1][i] + 1, trans, i)];
        
        if (start->prev == aux)
            start->prev = last;
        else
            start->next = last;
        
        if (aux->next == start)
            aux->next = end;
        else
            aux->prev = end;
        
        if (end->next == last)
            end->next = aux;
        else
            end->prev = aux;
        
        if (last->prev == end)
            last->prev = start;
        else
            last->next = start;
    }
    
    dblPrint(acid);
    printf("\n");
    
    free(dna);
    free(trans[0]);
    free(trans[1]);
    dblFree(acid);    
    
    return 0;
}
