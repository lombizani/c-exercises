/**********************************************************
 * Aluno: Carlos Filipe Lombizani De Bernardis            *
 * Numero USP: 6430536                                    *
 * Exercicio-Programa 3 -- Ordenacao de Arquivos de texto *
 * MAC0122 -- BMAC -- 2008 -- Prof.: Francisco Reverbel   *
 * Compilador: gcc 4.2          Arquivo: mallocX.c        *
 **********************************************************/

/*** implementação do wrapper ***/
#include <stdlib.h>
#include <stdio.h>

void *mallocX(size_t nbytes)
{
        void *ptr;
        
        ptr = malloc(nbytes);
        if (ptr == NULL) {
                fputs("Erro de alocação de memória. Programa finalizado.\n", stderr);
                exit(EXIT_FAILURE);
        }
        return ptr;
}

void *callocX(size_t nobj, size_t size)
{
        void *ptr;
        
        ptr = calloc(nobj, size);
        if (ptr == NULL) {
                fputs("Erro de alocação de memória. Programa finalizado.\n", stderr);
                exit(EXIT_FAILURE);
        }
        return ptr;
}
