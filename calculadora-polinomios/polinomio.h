/*** Definicao da interface da biblioteca polinomio ***/

/* Este e o nosso tipo de dados abstrato */
typedef struct termo *Polinomio;

/* Estas sao as chamadas que vamos usar p/ trabalhar com ele */

/* Cria um novo monomio (um polinomio com um so termo) cujo coeficiente e */
/* coef e cujo expoente e exp e devolve o monomio criado.                 */
Polinomio cria_monomio(double coef, unsigned int exp);

/* Recebe um polinômio p e devolve como valor da função um novo polinômio */
/* que é o produto de p por -1, ou seja cada monômio do polinômio         */
/* devolvido é igual a um monômio de p com o coeficiente multiplicado por */
/* -1                                                                     */
Polinomio nega(Polinomio p);

/* Recebe dois polinomios, p e q, e devolve em p a soma de p e q.         */
void soma(Polinomio p, Polinomio q);

/* Recebe dois polinomios, p e q, e devolve em p a diferenca entre p e q, */
/* ou seja, p-q.                                                          */
void subtrai(Polinomio p, Polinomio q);

/* Recebe dois polinomios, p e q, e devolve em p o produto de p e q.      */
void multiplica(Polinomio p, Polinomio q);

/* Recebe um polinomio p e devolve como valor da funcao um novo polinomio */
/* que e a derivada de p.                                                 */
Polinomio deriva(Polinomio p);

/* Recebe um polinomio p e devolve o grau do polinomio.                   */
int grau(Polinomio p);

/* Recebe um polinomio p e um real x e devolve o valor do polinomio p     */
/* em x                                                                   */
double calcula(Polinomio p, double x);

/* Recebe um polinomio p e devolve um novo polinomio que e uma copia de p */
Polinomio duplica(Polinomio p);

/* Recebe um polinomio p e o imprime.                                     */
void imprime(Polinomio p);

/* Libera a memoria ocupada pelo polinomio p.                             */
void libera(Polinomio p);
