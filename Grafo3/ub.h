typedef struct suniaoBusca UniaoBusca;

UniaoBusca* ub_cria(int tam); 
/* cria particao de conjunto com tam elementos */
/* cada elemento está inicialmente em parte separada */

int ub_busca (UniaoBusca* ub, int u);
/* retorna o representante da parte em que está u */

int ub_uniao (UniaoBusca* ub, int u, int v);
/* retorna o representante do resultado */

void ub_libera (UniaoBusca* ub);
/* libera a estrutura */

void debug (UniaoBusca *ub);
