/**********************************************************
 * Aluno: Carlos Filipe Lombizani De Bernardis            *
 * Numero USP: 6430536                                    *
 * Exercicio-Programa 3 -- Ordenacao de Arquivos de texto *
 * MAC0122 -- BMAC -- 2008 -- Prof.: Francisco Reverbel   *
 * Compilador: gcc 4.2          Arquivo: sort.c           *
 **********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "mallocX.h"
#define MAX_LENGTH 40000000     /* tamanho maximo da entrada             */
#define MAX_LINES 10            /* numero inicial de linhas              */
#define FACTOR 2                /* coeficiente de crescimento das linhas */
#define FALSE 0
#define TRUE 1

/* vamos denominar nossas funcoes de comparacao com o tipo cmpfunc */
typedef int(*cmpfunc)(const char *, const  char *);

/* comparacao case-insensitive -f */
int strcasecmp(const char *s, const  char *t)
{
        for (; toupper(*s) == toupper(*t); s++, t++)
                if (*s == '\0')
                        return 0;
                        
        return toupper(*s) - toupper(*t);
}

/* comparacao numerica -n */
int numcmp(const char *s, const  char *t)
{
        int vs, vt;
        
        vs = atoi(s);
        vt = atoi(t);
        if (vs < vt)
                return -1;
        else if (vs > vt)
                return 1;
        else
                return 0;
}

/* implementacao do algoritmo de ordenacao por insercao */
void insertionsort(char **strs, int s, int e, cmpfunc cmp)
{
        int i, j;
        char *x;
        
        for (j = s + 1; j < e; j++) {
                x = strs[j];
                for (i = j - 1; i >= s && ((*cmp)(strs[i], x) > 0); i--)
                        strs[i + 1] = strs[i];
                strs[i + 1] = x;
        }
}

/* implementacao do algoritmo de ordenacao por selecao */
void selectionsort(char **strs, int s, int e, cmpfunc cmp)
{
        int i, j, minj;
        char *x;
        
        for (i = s; i < e - 1; i++) {
                minj = i;
                for (j = i + 1; j < e; j++)
                        if ((*cmp)(strs[j], strs[minj]) < 0)
                                minj = j;
                x = strs[i];
                strs[i] = strs[minj];
                strs[minj] = x;
        }
}

/* implementacao do algoritmo de intercalacao */
void merge(int p, int q, int r, char **strs, cmpfunc cmp)
{
        int i, j, k;
        char **w;
        
        w = mallocX((r - p) * sizeof(char *));
        i = p;
        j = q;
        k = 0;
        
        while (i < q && j < r) {
                if((*cmp)(strs[i], strs[j]) <= 0)
                        w[k++] = strs[i++];
                else
                        w[k++] = strs[j++];
        }
        while (i < q)
                w[k++] = strs[i++];
        while (j < r)
                w[k++] = strs[j++];
        
        for(i = p; i < r; i++)
                strs[i] = w[i - p];
        
        free(w);
}

/* implementacao do algoritmo de ordenacao "mergesort" */
void mergesort(char **strs, int s, int e, cmpfunc cmp)
{
        if(s < e - 1) {
                int q = (s + e)/2;
                mergesort(strs, s, q, cmp);
                mergesort(strs, q, e, cmp);
                merge(s, q, e, strs, cmp);
        }
}

/* implementacao do algoritmo de peneirar */
void heapify(int p, int m, char **strs, cmpfunc cmp)
{
        char *x = strs[p];
        int f = 2*p;
        
        while (f <= m) {
                if (f < m && ((*cmp)(strs[f], strs[f + 1]) < 0))
                        f++;
                if ((*cmp)(x, strs[f]) >= 0)
                        break;
                strs[p] = strs[f];
                p = f;
                f = 2 * p;
        }
        
        strs[p] = x;
}

/* implementacao do algoritmo de ordenacao "heapsort" */
void heapsort(char **strs, int s, int e, cmpfunc cmp)
{
        int p, m;
        char *x;
        
        for(p = (e - s)/2; p >= s; p--)
                heapify(p, e - 1, strs, cmp);
        for(m = e - 1; m >= s + 1; m--) {
                x = strs[s];
                strs[s] = strs[m];
                strs[m] = x;
                heapify(s, m - 1, strs, cmp);
        }
}

/* implementacao do algoritmo de separacao */
int partition(int p, int r, char **strs,  cmpfunc cmp)
{
        char *c = strs[p], *t;
        int i = p + 1, j = r;
        
        while (i <= j) {
                if((*cmp)(strs[i], c) <= 0)
                        i++;
                else if ((*cmp)(strs[j], c) > 0)
                        j--;
                else {
                        t = strs[i];
                        strs[i] = strs[j];
                        strs[j] = t;
                        i++;
                        j--;
                }
        }
        strs[p] = strs[j];
        strs[j] = c;
        
        return j;
}

/* implementacao do algoritmo de ordenacao "quicksort" */
void quicksort(char **strs, int s, int e,  cmpfunc cmp)
{
        int j;
        
        while (s < (e - 1)) {
                j = partition(s, (e - 1), strs, cmp);
                quicksort(strs, s, j, cmp);
                s = j + 1;
        }
}

/* esta funcao vai nos dizer o tamanho da entrada quando ela for um arquivo */
unsigned long fsize(FILE *file)
{
        unsigned long size;
        
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        rewind(file);
        
        return size;
}

/* realocar a memoria para o vetor de linhas */
void realloc_lines(int *maxlines, char ***lines)
{
        char **tmpptr;

        *maxlines *= FACTOR;
        tmpptr = realloc(*lines, (*maxlines) * sizeof(char *));
        if (tmpptr == NULL) {
                fputs("Erro de alocação de memória. Programa finalizado.\n", stderr);
                exit(EXIT_FAILURE);
        } else {
                *lines = tmpptr;
        }
}

int main(int argc, char *argv[])
{
        cmpfunc cmp = strcmp;     /* funcao de comparacao que vamos usar      */
        char *text;               /* string de texto da entrada               */
        char **lines;             /* o vetor de linhas                        */
        char alg = 'Q';           /* algoritmo, por padrao "quicksort"        */
        unsigned long size;       /* tamanho da entrada                       */
        unsigned long i;          /* contador multi-uso                       */
        int j = 0;                /* contador de linhas                       */
        int maxlines = MAX_LINES; /* o de linhas alocadas                     */
        int reverse = FALSE;      /* -r - ordenacao decrescente               */
        int timing = FALSE;       /* -t - contagem de tempo decorrido         */
        time_t start, finish;     /* contadores de tempo                      */
        FILE *input = stdin;      /* nossa entrada, por padrao stdin            */
        
        /* vamos processar os parametros da linha de comando */
        if (argc > 1) {
                for (i = 1; i < argc; i++) {
                        if (argv[i][0] == '-') {
                                switch(argv[i][1]) {
                                        case 'a':
                                                alg = argv[i][2];
                                                break;
                                        case 'f':
                                                cmp = strcasecmp;
                                                break;
                                        case 'n':
                                                cmp = numcmp;
                                                break;
                                        case 'r':
                                                reverse = TRUE;
                                                break;
                                        case 't':
                                                timing = TRUE;
                                                break;
                                        default:
                                                fprintf(stderr, "%s: parametro invalido: -%c\n", argv[0], argv[i][1]);
                                                exit(EXIT_FAILURE);
                                }
                        } else {
                                input = fopen(argv[i], "r");
                                if (input == NULL) {
                                        fprintf(stderr, "%s: erro abrindo arquivo: %s\n", argv[0], argv[i]);
                                        exit(EXIT_FAILURE);
                                }
                        }
                }
        }
        
        /* alocamos um numero de linhas inicial e conforme precisamos alocamos mais */
        lines = mallocX(maxlines * sizeof(char *));
        
        /* agora vamos processar a entrada */
        if (input == stdin) {
                int c;
                
                size = 0;
                text = mallocX(MAX_LENGTH * sizeof(char));
                lines[j++] = text;
                
                while ((c = fgetc(input)) != EOF) {
                        if (c == '\n') {
                                text[size++] = '\0';
                                lines[j++] = &text[size];
                                if (j == maxlines)
                                        realloc_lines(&maxlines, &lines);
                        } else
                                text[size++] = c;
                }
                
                text[size++] = '\0';
        } else {
                size = fsize(input) + 1;
                text = mallocX(size);
                fread(text, sizeof(char), size, input);
                text[size - 1] = '\0';
                lines[j++] = text;
                
                for (i = 0; i < size; i++) {
                        if (text[i] == '\n') {
                                text[i] = '\0';
                                lines[j++] = &text[i + 1];
                                if (j == maxlines)
                                        realloc_lines(&maxlines, &lines);
                        }
                }
        }
        
        /* ordenar e contar o tempo caso preciso */
        if (timing)
                time(&start);
        
        switch(alg) {
                case 'H':
                        heapsort(lines, 0, j, cmp);
                        break;
                case 'I':
                        insertionsort(lines, 0, j, cmp);
                        break;
                case 'M':
                        mergesort(lines, 0, j, cmp);
                        break;
                case 'Q':
                        quicksort(lines, 0, j, cmp);
                        break;
                case 'S':
                        selectionsort(lines, 0, j, cmp);
                        break;
                default:
                        fprintf(stderr, "%s: algoritmo invalido: %c\n", argv[0], alg);
                        exit(EXIT_FAILURE);
        }
        
        if (timing)
                time(&finish);
        
        /* imprimir os resultados */
        if (reverse)
                for (i = j; i > 0; i--)
                        printf("%s\n", lines[i - 1]);
        else
                for (i = 0; i < j; i++)
                        printf("%s\n", lines[i]);
        
        if (timing)
                fprintf(stderr, "\nTempo decorrido:\t%g s\n", difftime(finish,start));
        
        /* adeus e obrigado por todos os peixes! */
        return 0;
}
