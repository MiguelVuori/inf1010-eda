typedef struct _listaprio ListaP;

ListaP *listap_cria (int tam);
/* cria nova lista de prioridades */

void listap_insere (ListaP *lp, int prioridade, int no1, int no2); 
/* insere um novo dado na lista */

int listap_remove(ListaP *lp, int* no1, int* no2);
/* remove e retorna a maior prioridade e os dois nós associados a ela */
/* retorna -1 caso a lista esteja vazia */

void listap_libera(ListaP *lp);
/* libera a lista de prioridades */

void debug_listap_show (ListaP *hp, char* title);
/* só para debug: enxerga representação interna! */



