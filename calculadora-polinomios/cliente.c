/**********************************************************
 * Aluno: Carlos Filipe Lombizani De Bernardis            *
 * Numero USP: 6430536                                    *
 * Exercicio-Programa 2 -- Polinomios Esparsos: O Retorno *
 * MAC0122 -- BMAC -- 2008 -- Prof.: Francisco Reverbel   *
 * Compilador: gcc 4.2          Arquivo: cliente.c        *
 **********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pilha.h"
#include "mallocX.h"
#define MAX_LINHA 4096  /* comprimento maximo de uma linha da entrada */
#define FALSE 0
#define TRUE 1

/* Recebe uma string, original, contendo uma expressão como as dadas no      */
/* arquivo de entrada, e devolve na string posfixa a expressão posfixa       */
/* correspondente.                                                           */
void converte(char original[], char posfixa[])
{
        Pilha pilha;                   /* a pilha em que vamos trabalhar            */
        int i;                         /* o contador usual...                       */
        int acabou;                    /* flag para para de desempilhar operadores  */
        int fim = 0;                   /* onde parou a nossa posfixa                */
        Item empilhado;                /* variavel auxiliar                         */
        
        pilha = cria(strlen(original));
        
        for (i = 0; original[i] != '\0' && original[i] != '\n'; i++) {
                if (original[i] >= 'a' && original[i] <= 'z') {
                        posfixa[fim++] = original[i];
                } else if (original[i] == '(') {
                        empilhado.ch = original[i];
                        empilha(pilha, empilhado);
                } else if (original[i] == ')') {
                        while (!vazia(pilha) && espia_topo(pilha).ch != '(')
                                posfixa[fim++] = desempilha(pilha).ch;
                        if (vazia(pilha))
                                fputs("\tAtencao: a expressao contem parenteses errados.\n", stderr);
                        else
                                desempilha(pilha);
                } else {
                        acabou = FALSE;
                        while (!vazia(pilha) && !acabou)
                                switch (espia_topo(pilha).ch) {
                                case '\'':
                                        posfixa[fim++] = desempilha(pilha).ch;
                                        break;
                                case '~':
                                        if (original[i] != '\'')
                                                posfixa[fim++] = desempilha(pilha).ch;
                                        else
                                                acabou = TRUE;
                                        break;
                                case '*':
                                        if (original[i] != '\'' && original[i] != '~')
                                                posfixa[fim++] = desempilha(pilha).ch;
                                        else
                                                acabou = TRUE;
                                        break;
                                case '+':
                                case '-':
                                        if (original[i] == '+' || original[i] == '-')
                                                posfixa[fim++] = desempilha(pilha).ch;
                                        else
                                                acabou = TRUE;
                                        break;
                                default:
                                        acabou = TRUE;
                                }
                        empilhado.ch = original[i];
                        empilha(pilha, empilhado);
                }
        }
        
        while (!vazia(pilha)) {
                empilhado = desempilha(pilha);
                if (empilhado.ch == '(')
                        fputs("\tAtencao: a expressao contem parenteses errados.\n", stderr);
                else
                        posfixa[fim++] = empilhado.ch;
        }
        
        posfixa[fim++] = '\0';
        destroi(pilha);
}

/* Recebe uma string posfixa e o vetor polinomio  definido acima, e devolve  */
/* um novo Polinomio, recém-criado, que é o resultado da expressão em  	     */
/* posfixa.								     */
Polinomio avalia(char posfixa[], Polinomio polinomio[])
{
        Polinomio resultado;           /* guardar nossos resultados aqui      */
        Pilha pilha;                   /* a pilha em que vamos trabalhar      */
        Polinomio *lixo;               /* para limpar a memoria               */
        Item empilhado;                /* variavel auxiliar                   */
        int i;                         /* o bom e velho contador              */
        int tamanho;                   /* tamanho da pilha e do vetor lixo    */
        int fim = 0;                   /* onde parou o lixo                   */
        
        tamanho = strlen(posfixa);
        pilha = cria(tamanho);
        lixo = mallocX(tamanho * sizeof(Polinomio));
        
        for (i = 0; posfixa[i] != '\0' && posfixa[i] != '\n'; i++) {
                if (posfixa[i] >= 'a' && posfixa[i] <= 'z')
                        if (polinomio[posfixa[i] - 'a'] == NULL)
                                fprintf(stderr, "\tAtencao: polinomio %c nao declarado.\n", posfixa[i]);
                        else {
                                empilhado.pol = polinomio[posfixa[i] - 'a'];
                                empilha(pilha, empilhado);
                        }
                else
                        switch (posfixa[i]) {
                        case '\'':
                                empilhado = desempilha(pilha);
                                resultado = lixo[fim++] = deriva(empilhado.pol);
                                empilhado.pol = resultado;
                                empilha(pilha, empilhado);
                                break;
                        case '~':
                                empilhado = desempilha(pilha);
                                resultado = lixo[fim++] = nega(empilhado.pol);
                                empilhado.pol = resultado;
                                empilha(pilha, empilhado);
                                break;
                        case '*':
                                resultado = lixo[fim++] = duplica(desempilha(pilha).pol);
                                multiplica(resultado, desempilha(pilha).pol);
                                empilhado.pol = resultado;
                                empilha(pilha, empilhado);
                                break;
                        case '+':
                                resultado = lixo[fim++] = duplica(desempilha(pilha).pol);
                                soma(resultado, desempilha(pilha).pol);
                                empilhado.pol = resultado;
                                empilha(pilha, empilhado);
                                break;
                        case '-':
                                resultado = lixo[fim++] = duplica(desempilha(pilha).pol);
                                subtrai(resultado, desempilha(pilha).pol);
                                empilhado.pol = resultado;
                                empilha(pilha, empilhado);
                                break;
                        default:
                                fprintf(stderr, "\tAtencao: operador %c desconhecido.\n", posfixa[i]);
                        }
                        /* sim, eu sei que eu repeti estas duas linhas em   */
                        /* cada caso do switch, mas se eu as colocasse aqui */
                        /* o compilador avisaria da possibilidade de usar   */
                        /* a variavel resultado sem inicializa-la!          */
                        /* empilhado.pol = resultado;                       */
                        /* empilha(pilha, empilhado);                       */
        }
        
        resultado = duplica(desempilha(pilha).pol);
        if (!vazia(pilha))
                fputs("\tAtencao: a expressao contem operandos demais.\n", stderr);
        
        /* vamos limpar os nossos restos */
        for (i = 0; i < fim; i++)
                libera(lixo[i]);
        free(lixo);
        destroi(pilha);
        
        return resultado;
}

/* esta funcao recebe uma string, a processa e devolve um polinomio */
Polinomio ler_polinomio(char *linha)
{
        int ntermos;        /* quantos termos temos que ler         */
	int i = 0;          /* nosso contador                       */
        unsigned int *exps; /* vetor que vai conter os expoentes    */
        double *coefs;      /* vetor que vai conter os coeficientes */
        char *sobra;        /* a parte nao processada da linha      */
        Polinomio p, aux;   /* nosso polinomio                      */
        
        ntermos = (int)strtol(linha, &sobra, 10);
        
        exps = mallocX(ntermos * sizeof(unsigned int));
        coefs = mallocX(ntermos * sizeof(double));
        
        /* aqui nos pegamos os parametros da linha ate ela acabar */
        do {
                coefs[i] = strtod(sobra, &sobra);
                exps[i] = (unsigned int)strtol(sobra, &sobra, 10);
        } while (&sobra != NULL && ++i < ntermos);
        
        /* e aqui nos criamos nosso polinomio com os parametros lidos */
        p = cria_monomio(0, 0);
        for(i = 0; i < ntermos; i++) {
                aux = cria_monomio(coefs[i], exps[i]);
                soma(p, aux);
                libera(aux);
        }
        
        free(exps);
        free(coefs);

        return p;
}

int main(int argc, char *argv[])
{
        char linha[MAX_LINHA];  /* armazenamos cada linha de entrada          */
        char posfixa[MAX_LINHA];/* notacao posfixa convertida pela converte() */
        Polinomio *polinomio;   /* os nossos polinomios, máximo de 26         */
        Polinomio resultado;    /* aqui vai o resultado de cada expressao     */
        int i = 0, j = 0;       /* nossos contadores                          */
        int n = 0;              /* o numero de polinomios                     */
        FILE *entrada = stdin;  /* nossa entrada, por padrao stdin            */
        
        /* o usuario pode passar como parametro um arquivo de entrada */
        if (argc > 1) {
                entrada = fopen(argv[1], "r");
                if (entrada == NULL) {
                        entrada = stdin;
                        fprintf(stderr, "Erro abrindo arquivo \"%s\"!\n\n", argv[1]);
                } else
                        fprintf(stderr, "Usando arquivo \"%s\"...\n\n", argv[1]);
        }
        
        /* vamos procurar o número de polinomios */
        while (n == 0 && fgets(linha, MAX_LINHA, entrada) != NULL)
                n = atoi(linha);
                
        /* vamos inicializar nosso vetor e depois ler os n polinomios */
        polinomio = callocX(26, sizeof(Polinomio));
	
        while (i < n) {
                if (fgets(linha, MAX_LINHA, entrada) != NULL) {
                        for (j = 0; (linha[j] < 'a' || linha[j] > 'z') && linha[j] != '\0' && linha[j] != '\n'; j++);
                        if (linha[j] == '\n' || linha[j] == '\0')
                                continue;
                        else {
                                polinomio[(linha[j] - 'a')] = ler_polinomio(&(linha[j + 1]));
                                i++;
                                printf("\t%c(x) = ", linha[j]);
                                imprime(polinomio[(linha[j] - 'a')]);
                                printf("\n");
                        }
                } else {
                        fputs("Fim de arquivo prematuro. Programa finalizado.\n", stderr);
                        exit(EXIT_FAILURE);
                }
        }
        
        printf("\n");
        
	/* vamos executar as expressoes e dar os resultados para o usuario */
        while (fgets(linha, MAX_LINHA, entrada) != NULL) {
                converte(linha, posfixa);
                printf("expressao original:\t%s", linha);
                printf("versao em posfixa:\t%s\n", posfixa);
                resultado = avalia(posfixa, polinomio);
                printf("resultado:\t\t");
                imprime(resultado);
                printf("\n\n");
                libera(resultado);
        }
        
        return 0;
}
