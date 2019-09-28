#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "grafo.h"


typedef struct _viz Viz;
struct _viz {
  int noj;
  float peso;
  Viz*  prox;
};

struct _grafo {
  int nv;           /* numero de nos ou vertices */
  int na;           /* numero de arestas */
  Viz** viz;         /* viz[i] aponta para a lista de arestas vizinhas do no i */
};

/*-------------------*/
typedef struct _listaprio ListaP;

struct _item {
  float dist;
  int idno;
};

struct _listaprio {
  int max; /* tamanho maximo do heap */
  int pos; /* proxima posicao disponivel no vetor */ 
  struct _item *itens; /*heap propriamente dito */ 
  int* posnos; /*posicao de cada n� no heap */
}; 

ListaP *listap_cria (int max) {
	ListaP *novo = (ListaP *)malloc(sizeof(ListaP));
	novo->max = max;
	novo->pos = 0;
	novo->itens = (struct _item *)malloc(sizeof(struct _item) * max);
	novo->posnos = (int *)malloc(sizeof(int) * max);

	return novo;
}

void listap_libera(ListaP *lp) {
	if (lp == NULL) return;
	free(lp->itens);
	free(lp->posnos);
	free(lp);
	lp = NULL;
}

void debug_listap_show (ListaP *lp, char* title) {
	if (lp == NULL) return;
	int i;

	printf("%s\n", title);
	for (i = 0; i < lp->pos; i++)
		printf("{ %d, %f} ", lp->itens[i].idno, lp->itens[i].dist);
	printf("\n");
	for (i = 0; i < lp->max; i++)
		printf("%d ", lp->posnos[i]);
	printf("\n");
}

void corrige_abaixo(ListaP* lp, int atual) {
	if ((lp == NULL) | ((2*atual + 1) >= lp->pos)) return;
	struct _item r_node;
	struct _item l_node;

	l_node = lp->itens[2*atual + 1];
	if ((2*atual + 2) < lp->pos) {
		r_node = lp->itens[2*atual + 2];
		if (l_node.dist > r_node.dist) {
			lp->itens[2*atual + 1] = r_node;
			lp->posnos[r_node.idno] = 2*atual + 1;
			lp->itens[2*atual + 2] = l_node;	
			lp->posnos[l_node.idno] = 2*atual + 2;
		}
	}
	if (lp->itens[2*atual + 1].dist < lp->itens[atual].dist) {
		l_node = lp->itens[atual];
		lp->itens[atual] = lp->itens[2*atual + 1];
		lp->posnos[lp->itens[2*atual + 1].idno] = atual;
		lp->itens[2*atual + 1] = l_node;
		lp->posnos[l_node.idno] = 2*atual + 1;
		corrige_abaixo(lp, 2*atual + 1);
	}
}

void corrige_acima (ListaP* lp, int atual) {
	int nPai = (atual - 1) / 2;
	struct _item pai;
	if ((lp == NULL) | (atual == 0)) return;

	pai = lp->itens[nPai];

	if (pai.dist > lp->itens[atual].dist) {
		lp->itens[nPai] = lp->itens[atual];
		lp->posnos[lp->itens[atual].idno] = nPai;
		lp->itens[atual] = pai;
		lp->posnos[pai.idno] = atual;
	}

	corrige_acima(lp, nPai);
}

void listap_insere (ListaP *lp, int distancia, int idno) {
	if (lp == NULL) return;
	
	if (lp->pos >= lp->max) {
		printf("\n HEAP CHEIO!!! \n");
		return;
	}

	lp->itens[lp->pos].dist = distancia;
	lp->itens[lp->pos].idno = idno;
	lp->posnos[idno] = lp->pos;
	corrige_acima(lp, lp->pos);
	lp->pos++;
}

int listap_remove(ListaP *lp) {
	if (lp == NULL) return -1;

	if (lp->pos == 0) {
		printf("HEAP VAZIO!!!\n");
		return -1;
	}

	struct _item pop = lp->itens[0];
	lp->posnos[pop.idno] = -1;
	if (lp->pos > 1) {
		lp->itens[0] = lp->itens[lp->pos - 1];
		lp->posnos[lp->itens[0].idno] = 0;
		corrige_abaixo(lp, 0);
	}
	lp->pos--;
	return pop.idno;
}

void listap_corrige (ListaP* lp, int novadist, int idno) {
	if (lp->posnos[idno] == -1) {
			return;
	}
	int pos = lp->posnos[idno];

	lp->itens[pos].dist = novadist;
	corrige_acima(lp, pos); 
}

static Viz* criaViz(Viz* head, int noj, float peso) {
/* insere vizinho no inicio da lista */
  Viz* no = (Viz*) malloc(sizeof(Viz));
  assert(no);
  no->noj = noj;
  no->peso = peso;
  no->prox = head;
  return no;
}

static Grafo* grafoCria(int nv, int na) {

  int i;
  Grafo* g = (Grafo *) malloc(sizeof(Grafo));
  g->nv = nv;
  g->na = na;
  g->viz = (Viz **) malloc(sizeof(Viz *) * nv);
  for (i = 0; i < nv; i++)
    g->viz[i] = NULL;
  return g;
}


Grafo* grafoLe( char* filename ) {
/* cria grafo n�o orientado - sup�e que arquivo est� correto! */

  FILE *arq = fopen(filename,"rt");
  int nv, na, no1, no2 = 0;
  float peso;
  Grafo* novo;
	
  fscanf(arq, "%d %d", &nv, &na);
  novo = grafoCria(nv, na);
  assert(novo);
  while (fscanf(arq, "%d %d %f", &no1, &no2, &peso) == 3) {
    novo->viz[no1] = criaViz(novo->viz[no1], no2, peso);
    novo->viz[no2] = criaViz(novo->viz[no2], no1, peso);
  }
  return novo;
}

Grafo* grafoLibera(Grafo* grafo) {
  if (grafo) {
    int i = 0;
    Viz* no,*aux;
    for (i = 0; i < grafo->nv; i++){
      no = grafo->viz[i];
      while (no){
        aux = no->prox;
        free(no);
        no = aux;
      }
    }
    free(grafo->viz);
    free(grafo);
  }
  return NULL;
}

void grafoMostra (char* title, Grafo * grafo) {
  int i;
  if (title)
    printf("%s", title);
  if (grafo) {
    printf("NV: %d, NA: %d\n", grafo->nv, grafo->na);
    for (i = 0; i < grafo->nv; i++){
    Viz* viz = grafo->viz[i];
    printf("[%d]->", i);
    while (viz) {
      printf("{%d, %g}->", viz->noj, viz->peso);
      viz = viz->prox;
    }
    printf("NULL\n");
    }
  }
}

/* usar essa fun��o (se quiser) na primeira vers�o de menorescaminhos! */
int menordist( int* caminhos, char* visitados) {
  int minimo = INT_MAX; int nomin = -1;
  int i;
  for (i=0;caminhos[i]!=-1;i++)
    if (!visitados[i] && caminhos[i]<minimo) {
      nomin = i;
      minimo = caminhos[i];
    }
  return nomin;
}

int* menoresCaminhos (Grafo *grafo, int no_inicial){
	int i;
  int *menoresCaminhos = (int *)malloc(sizeof(int) * (grafo->nv + 1));
	float *distancias = (float *)malloc(sizeof(float) * grafo->nv);
	ListaP *visitados = listap_cria(grafo->nv);
	Viz *viz;

  if (no_inicial >= grafo->nv) return NULL;

	for (i = 0; i < grafo->nv; i++) {
		if (i == no_inicial) {
			distancias[i] = 0;
			listap_insere(visitados, 0, i);
		}
		else {
			distancias[i] = INT_MAX;
			listap_insere(visitados, INT_MAX, i);
		}
	}
	menoresCaminhos[no_inicial] = no_inicial;
	menoresCaminhos[i] = -1;
	i = listap_remove(visitados);

	while (i != -1) {
		/*printf("\n%d: ", i);
		debug_listap_show(visitados, "");*/
		viz = grafo->viz[i];

		while (viz) {
			float new_dist = distancias[i] + viz->peso;
			if (new_dist < distancias[viz->noj]) {
				listap_corrige(visitados, new_dist, viz->noj);
				distancias[viz->noj] = new_dist;
				menoresCaminhos[viz->noj] = i;
			}
			viz = viz->prox;
		}
		i = listap_remove(visitados);
	}

	/*printf("Que? ");
	for (i = 0; i < grafo->nv; i++)
			printf("%d ", menoresCaminhos[i]);
	printf("%d;\n", menoresCaminhos[i]);*/

	free(distancias);
	listap_libera(visitados);
  return menoresCaminhos;
}
