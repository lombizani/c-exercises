/**********************************************************
 * Aluno: Carlos Filipe Lombizani De Bernardis            *
 * Numero USP: 6430536                                    *
 * Exercicio-Programa 2 -- Polinomios Esparsos: O Retorno *
 * MAC0122 -- BMAC -- 2008 -- Prof.: Francisco Reverbel   *
 * Compilador: gcc 4.2          Arquivo: polinomio.c      *
 **********************************************************/

/*** Implementacao da biblioteca polinomio ***/
#include <stdlib.h>
#include <stdio.h>
#include "polinomio.h"
#define  LISTAMAX 100           /* tamanho maximo para a nossa lista livre */

typedef struct termo {
        double coef;
        unsigned int exp;
        struct termo *prox;
} Termo;

/* isto funciona como area de troca para reciclagem de monomios */
static Termo *lista_livre = NULL;

/* Funcoes auxiliares */

/* verifica se a lista livre esta vazia */
static int lista_vazia()
{
        return (lista_livre == NULL);
}

/* verifica se a lista livre esta cheia */
static int lista_cheia()
{
        int tamanho = 0;
        Termo *lista;
        
        for (lista = lista_livre; lista != NULL; lista = lista->prox)
                tamanho++;
        
        return (tamanho >= LISTAMAX);
}

/* recebe um termo e o coloca na lista */
static void enlistar(Termo *m)
{
        m->prox = lista_livre;
        lista_livre = m;
}

/* devolve o primeiro termo da lista e o retira da lista */
static Termo *desenlistar()
{
        Termo *desenlistado;
        
        desenlistado = lista_livre;
        lista_livre = lista_livre->prox;
        
        return desenlistado;
}

/* libera definitivamente todos os termos na lista livre */
static void limpar_lista(void)
{
        Termo *lista;
        
        for (lista = lista_livre; lista != NULL; lista = lista_livre) {
                lista_livre = lista_livre->prox;
                free(lista);
        }
}

/* wrapper em volta do malloc para fazer um tratamento especial */
static void *mallocx(size_t size)
{
        void *p;
        
        p = malloc(size);
        
        if (p == NULL) {
                /* nao conseguimos nossa memoria, vamos liberar a lista_livre */
                /* e tentar de novo!                                          */
                limpar_lista();
                p = malloc(size);
                if (p == NULL) {
                        /* agora nao tem jeito mesmo, vamos embora... */
                        fputs("Erro de alocacao de memoria. Programa finalizado.\n", stderr);
                        exit(EXIT_FAILURE);
                }
        }
        
        return p;
}

/* devolve um termo recem-alocado ou reciclado da lista livre */
static Termo *alocar_termo()
{
        Termo *p;
        
        if (lista_vazia())
                p = mallocx(sizeof(Termo));
        else
                p = desenlistar();
        
        return p;
}

/* recebe um termo, cria um novo termo com as mesmas propriedades e o devolve */
static Termo *clona(Termo *m)
{
        Termo *n;
        
        n = alocar_termo();
        n->exp = m->exp;
        n->coef = m->coef;
        n->prox = m->prox;
        
        return n;
}

/* recebe um termo e o descarta, liberando-o ou colocando-o na lista livre */
static void eliminar_termo(Termo *m)
{
        if (lista_cheia())
                free(m);
        else
                enlistar(m);
}

/* recebe um polinomio e devolve o numero de termos que ele tem */
static unsigned int tamanho(Polinomio p)
{
        unsigned int tamanho = 0;
        Polinomio q;
        
        for (q = p->prox; q != NULL; q = q->prox)
                tamanho++;
        
        return tamanho;
}

/* recebe dois numeros e devolve o primeiro elevado ao segundo */
static double potencia(double x, int n)
{    
        if (n == 0)
                return 1;
        else {
                double p;
                p = potencia(x, n / 2);
                p = p * p;
                if (n % 2 != 0)
                        p *= x;
                return p;
        }
}

/* Chamadas da biblioteca */

Polinomio cria_monomio(double coef, unsigned int exp)
{
        static int _atexit = 1;
        Polinomio p;
        
        p = alocar_termo();
        if (coef != 0) {
                p->prox = alocar_termo();
                p->prox->coef = coef;
                p->prox->exp = exp;
                p->prox->prox = NULL;
        } else {
                p->prox = NULL;
        }
        
        /* na primeira vez registramos a funcao limpar_lista para */
        /* executar na saida                                      */
        if (_atexit) {
                _atexit = atexit(limpar_lista);
        }
        
        return p;
}

Polinomio nega(Polinomio p)
{
        Polinomio q;
        
        q = cria_monomio(0, 0);
        subtrai(q, p);
        
        return q;
}

void soma(Polinomio p, Polinomio q)
{
        Polinomio r, s;
        Polinomio t;
        Termo *m;

        r = p;
        
        for (s = q->prox; s != NULL; s = s->prox) {
                for ( ; (r->prox != NULL) && (r->prox->exp < s->exp); r = r->prox);
                if (r->prox != NULL && r->prox->exp == s->exp) {
                        r->prox->coef += s->coef;
                } else {
                        t = clona(s);
                        t->prox = r->prox;
                        r->prox = t;
                }
                if (r->prox->coef == 0) {
                        m = r->prox;
                        r->prox = r->prox->prox;
                        eliminar_termo(m);
                }
        }
}

void subtrai(Polinomio p, Polinomio q)
{
        Polinomio r, s;
        Polinomio t;
        Termo *m;
        
        r = p;
        
        for (s = q->prox; s != NULL; s = s->prox) {
                for ( ; (r->prox != NULL) && (r->prox->exp < s->exp); r = r->prox);
                if (r->prox != NULL && r->prox->exp == s->exp) {
                        r->prox->coef -= s->coef;
                } else {
                        t = clona(s);
                        t->coef = -t->coef;
                        t->prox = r->prox;
                        r->prox = t;
                }
                if (r->prox->coef == 0) {
                        m = r->prox;
                        r->prox = r->prox->prox;
                        eliminar_termo(m);
                }
        }
}

void multiplica(Polinomio p, Polinomio q)
{
        Polinomio r, *s, t;
        unsigned int tam, i = 0; 
        
        tam = tamanho(p);
        
        s = mallocx(tam * sizeof(Polinomio));
        
        for (r = p->prox; r != NULL; r = r->prox) {
                s[i] = duplica(q);
                for (t = s[i]->prox; t != NULL; t = t->prox) {
                        t->coef *= r->coef;
                        t->exp += r->exp;
                }
                i++;
        }
        
        for (i = 1; i < tam; i++) {
                soma(s[0], s[i]);
                libera(s[i]);
        }
        
        libera(p->prox);
        p->prox = s[0]->prox;
        eliminar_termo(s[0]);
        free(s);
}

Polinomio deriva(Polinomio p)
{
        Polinomio q, r;
        Termo *m;
        
        q = duplica(p);
        
        for (r = q; r != NULL && r->prox != NULL; r = r->prox) {
                if (r->prox->exp == 0) {
                        m = r->prox;
                        r->prox = r->prox->prox;
                        eliminar_termo(m);
                }
                if (r->prox != NULL) {
                        r->prox->coef *= r->prox->exp;
                        r->prox->exp--;
                }
        }
        
        return q; 
}

int grau(Polinomio p)
{
        int grau = 0;
        Polinomio q;
        
        for (q = p->prox; q != NULL; q = q->prox)
                if (q->exp > grau) grau = q->exp;
        
        return grau;
}

double calcula(Polinomio p, double x)
{
        double y = 0;
        Polinomio q;
        
        for (q = p->prox; q != NULL; q = q->prox)
                y += q->coef * potencia(x, q->exp);
        
        return y;
}

Polinomio duplica(Polinomio p)
{
        Polinomio q, r, copia;
        
        copia = r = alocar_termo();
        r->prox = p->prox;
        
        for (q = p->prox; q != NULL; q = q->prox) {
                r->prox = alocar_termo();
                r->prox->exp = q->exp;
                r->prox->coef = q->coef;
                r->prox->prox = NULL;
                r = r->prox;
        }
        
        return copia;
}

void imprime(Polinomio p)
{
        Polinomio q;
        
        if (p->prox == NULL)
                printf("0");
        else
                for (q = p; q->prox != NULL; q = q->prox)
                        printf("%+gx^%d ", q->prox->coef, q->prox->exp);

        fflush(stdout);
}

void libera(Polinomio p)
{
        Polinomio q, *s;
        int i, cont = 0, tam;
        
        tam = tamanho(p) + 1; /* mais um para a cabeca da lista! */
        s = mallocx(tam * sizeof(Polinomio));
        
        for (q = p; q != NULL; q = q->prox) {
                s[cont] = q;
                cont++;
        }
        
        /* eliminar_termo se encarrega de descartar os termos para nos */
        for (i = cont - 1; i >= 0; i--)
                eliminar_termo(s[i]);
        
        free(s);
}
