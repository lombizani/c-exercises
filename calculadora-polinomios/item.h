/**********************************************************
 * Aluno: Carlos Filipe Lombizani De Bernardis            *
 * Numero USP: 6430536                                    *
 * Exercicio-Programa 2 -- Polinomios Esparsos: O Retorno *
 * MAC0122 -- BMAC -- 2008 -- Prof.: Francisco Reverbel   *
 * Compilador: gcc 4.2          Arquivo: item.h           *
 **********************************************************/

/*** Definicao o tipo Item ***/
#include "polinomio.h"

typedef union {
        char ch;
        Polinomio pol;
} Item;
