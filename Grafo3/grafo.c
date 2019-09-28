#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "grafo.h"
#include "listaprio.h"
#include "ub.h"

struct item {
  int prioridade;
  int n1, n2;
};

struct _listaprio {
  int max; /* tamanho maximo do heap */
  int pos; /* proxima posicao disponivel no vetor */ 
  struct item *itens;
}; 

typedef struct _viz Viz;
struct _viz {
  int noj;
  int peso;
  Viz*  prox;
};

struct _grafo {
  int nv;           /* numero de nos ou vertices */
  int na;           /* numero de arestas */
  Viz** viz;         /* viz[i] aponta para a lista de arestas vizinhas do no i */
};

static Viz* criaViz(Viz* head, int noj, int peso) {
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
  assert(g);
  g->nv = nv;
  g->na = na;
  g->viz = (Viz **) malloc(sizeof(Viz *) * nv);
  assert(g->viz);
  for (i = 0; i < nv; i++)
    g->viz[i] = NULL;
  return g;
}


Grafo* grafoLe( char* filename ) {
/* cria grafo não orientado - supõe que arquivo está correto! */

  FILE *arq = fopen(filename,"rt");
  int nv, na, no1, no2 = 0;
  int peso;
  Grafo* novo;
	
  fscanf(arq, "%d %d", &nv, &na);
  novo = grafoCria(nv, na);
  assert(novo);
  while (fscanf(arq, "%d %d %d", &no1, &no2, &peso) == 3) {
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

void grafoMostra (Grafo * grafo) {
  int i;
  if (grafo) {
    printf("NV: %d, NA: %d\n", grafo->nv, grafo->na);
    for (i = 0; i < grafo->nv; i++){
      Viz* viz = grafo->viz[i];
      printf("[%d]->", i);
      while (viz) {
        printf(" {%d, %d}", viz->noj, viz->peso);
        viz = viz->prox;
      }
      printf("\n");
    }
  }
}

Grafo* arvoreCustoMinimo (Grafo* g) {

  UniaoBusca* ub = ub_cria(g->nv);
  ListaP* lista = listap_cria(g->na);
  Grafo* ag = grafoCria(g->nv,g->nv-1);
  int aresta,n1,n2;
  int rep1,rep2,i = 0;


  for(i = 0; i < g->nv; i++)
  {
    Viz* viz = g->viz[i];
    while(viz != NULL)
    {
      if (viz->noj > i)
        listap_insere(lista,viz->peso,i,viz->noj);
      viz = viz->prox;
    }
    //debug_listap_show(lista,"meu heap = ");
  }

  while(lista->pos != 0)
  {
    aresta = listap_remove(lista,&n1,&n2);
    rep1 = ub_busca(ub,n1);
    rep2 = ub_busca(ub,n2);

    if(rep1 != rep2)
    {
      ub_uniao(ub,rep1,rep2);
      ag->viz[n1] = criaViz(ag->viz[n1],n2,aresta);
      ag->viz[n2] = criaViz(ag->viz[n2],n1,aresta);
    }
  }

  listap_libera(lista);
  ub_libera(ub);

  return ag;
}
