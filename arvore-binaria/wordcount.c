/***************************************************************/
/**                                                           **/
/** Carlos De Bernardis                                       **/
/** Este programa utiliza árvores binárias para contar a      **/
/** quantidade de palavras que ocorrem n vezes num texto      **/
/**                                                           **/
/***************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX 10000000

typedef struct tnode {
    char *word;
    int c;
    struct tnode *left;
    struct tnode *right;
} TreeNode;

typedef TreeNode *BinTree;

typedef char *String;

/* Recebe um ponteiro str para String e devolve uma String com a primeira 
   palavra que aparece em str, deixa str no fim da palavra */
String sgetword(String *str)
{
    String word;
    int i, size = 0;
    
    while(!isalpha(**str) && **str != '\0') (*str)++;
    
    if (**str == '\0') return NULL;
    
    while(isalpha(**str)) {
        size++;
        (*str)++;
    }
    
    word = malloc(sizeof(char) * (size + 1));
    
    for (i = 0, *str -= size; i < size; i++, (*str)++) word[i] = tolower(**str);
    
    word[size] = '\0';
    
    return word;
}

/* Recebe a raiz de uma subárvore bt, uma String word e um ponteiro para 
   inteiro maxc, coloca word na posição correta da árvore se ela não estiver lá,
   se já estiver incremente o contador, mantém em maxc o número maior de
   ocorrências até agora, devolve a referência da árvore modificada */
BinTree btAddword(BinTree bt, String word, int *maxc)
{
    int cond;
    
    if (bt == NULL) {
        bt = malloc(sizeof(TreeNode));
        bt->word = word;
        bt->c = 1;
        bt->left = bt->right = NULL;
    } else { 
        cond = strcmp(word, bt->word);
        if (!cond) {
            bt->c++;
            if ((bt->c) > *maxc)
                *maxc = bt->c;
            free(word);
        }
        else if (cond > 0)
            bt->right = btAddword(bt->right, word, maxc);
        else
            bt->left = btAddword(bt->left, word, maxc);
    }
    
    return bt;
}

/* Recebe uma subárvore bt e imprime as palavras nela quantas vezes elas ocorreram */
void btPrint(BinTree bt)
{
    if (bt != NULL) {
        int i;
        btPrint(bt->left);
        for (i = 0; i < bt->c; i++)
            printf("%s\n", bt->word);
        btPrint(bt->right);
    }
}

/* Recebe uma subárvore bt e um vetor bucket de tamanho apropriado e guarda 
   nas posições de bucket o número de ocorrências de repetições */
void btCount(BinTree bt, int *bucket)
{ 
    if (bt != NULL) {
        btCount(bt->left, bucket);
        bucket[bt->c - 1]++;
        btCount(bt->right, bucket);
    }
}

/* Libera uma árvore bt da memória */
void btFree(BinTree bt)
{ 
    if (bt != NULL) {
        btFree(bt->left);
        btFree(bt->right);
        free(bt);
    }
}

/* nosso programa pode receber o parâmetro -p na linha de comando para imprimir
   todas as palavras lidas em número ocorrido e em ordem alfabética */
int main(int argc, char **argv)
{
    String str, /* ponteiro para percorrer o texto */
           word; /* guardará as palavras encontradas */
    String text; /* o texto propriamente dito */
    int i, /* contador de impressões */
        size, /* tamanho do texto lido */
        maxc = 0, /* maior número de ocorrências */
        *bucket; /* vetor de ocorrências de repetições */
    BinTree root = NULL /* nossa árvore binária */;
    
    text = malloc((MAX + 1) * sizeof(char));
    size = fread(text, sizeof(char), MAX, stdin);    
    text[size] = '\0';
    str = text;
    
    while((word = sgetword(&str)) != NULL)
        root = btAddword(root, word, &maxc);
    
    bucket = calloc(maxc, sizeof(int));
    
    if (argc > 1 && !strcmp(argv[1], "-p"))
        btPrint(root);
    
    btCount(root, bucket);
    
    for (i = 0; i < maxc; i++)
        if (bucket[i] > 0)
            printf("%3d -> %5d\n", i + 1, bucket[i]);
    
    btFree(root);
    free(text);
    free(bucket);
    
    return 0;
}
