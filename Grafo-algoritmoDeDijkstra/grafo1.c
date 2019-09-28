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
  int dist;
  int idno;
};

struct _listaprio {
  int max; /* tamanho maximo do heap */
  int pos; /* proxima posicao disponivel no vetor */ 
  struct _item *itens;
  int* posnos;
}; 

ListaP *listap_cria (int max);
void listap_insere (ListaP *lp, int distancia, int idno);
int listap_remove(ListaP *lp);
void listap_libera(ListaP *lp);
void listap_corrige (ListaP* heap, int novadist, int idno);
void debug_listap_show (ListaP *hp, char* title);
void corrige_abaixo(ListaP* heap, int atual);

/* criar funções ! */

/*-------------------*/

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
/* cria grafo não orientado - supõe que arquivo está correto! */

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

Grafo*   grafoLibera(Grafo* grafo) {
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

/* usar essa função (se quiser) na primeira versão de menorescaminhos! */
int menordist( float* caminhos, char* visitados) {
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
	int i,no = no_inicial,nv = grafo->nv;
  float menorCaminhoPN[nv + 1];
	char visitados[nv];
	
  if (no_inicial >= grafo->nv) return NULL;
	int*  ultimosNos = (int*) malloc (sizeof(int)*(nv + 1));
	

  for(i = 0;i<grafo->nv;i++){
  	menorCaminhoPN[i] = INT_MAX;
  	visitados[i] = 0;
  }
  

  menorCaminhoPN[i] = -1.0;
  menorCaminhoPN[no_inicial] = 0.0;
  

  Viz* viz;
  while(no != -1)
  {
  	viz = grafo->viz[no];
  	while( viz != NULL)
  	{
  		if(visitados[viz->noj] == 0 && menorCaminhoPN[viz->noj] > (viz->peso + menorCaminhoPN[no])){
  		menorCaminhoPN[viz->noj] = viz->peso + menorCaminhoPN[no];
			ultimosNos[viz->noj] = no;
		}
  		viz = viz->prox;
  	}
  	visitados[no] = 1;
  	no = menordist(menorCaminhoPN, visitados);
  }
  ultimosNos[no_inicial] = no_inicial;
  ultimosNos[nv] = -1;

  return ultimosNos;
}


