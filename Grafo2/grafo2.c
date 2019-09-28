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
void corrige_acima(ListaP *heap, int idno);

/* criar fun��es ! */

ListaP *listap_cria(int max)
{
    ListaP *heap = (ListaP*)malloc(sizeof(struct _listaprio));

    heap->max = max;
    heap->pos = 0;
    heap->itens = (struct _item*)malloc(max * sizeof(struct _item));
    heap->posnos = (int*)malloc(max * sizeof(int));

    return heap;
}

void debug_listap_show(ListaP *hp, char *title){
    int i;
    printf("%s={",title);
    for (i=0; i<(hp->pos); i++)
        printf("{%d, %d},", hp->itens[i].idno, hp->itens[i].dist);
    printf("}\n");
}

void listap_insere(ListaP *lp, int distancia, int idno)
{
    if (lp->pos < lp->max){
        lp->itens[lp->pos].dist = distancia;
        lp->itens[lp->pos].idno = idno;
        lp->posnos[idno] = lp->pos;
        corrige_acima(lp, lp->pos);
        lp->pos++;
    }
    else{
        printf("Heap CHEIO!\n");
        exit(1);
    }
}

int listap_remove(ListaP *lp)
{
    if (lp->pos>0){
        int Notopo=lp->itens[0].idno;
        lp->posnos[lp->itens[0].idno] = -1;
        lp->posnos[lp->itens[lp->pos - 1].idno] = 0;
        lp->itens[0].idno = lp->itens[lp->pos - 1].idno;
        lp->itens[0].dist = lp->itens[lp->pos - 1].dist;
        lp->pos--;
        corrige_abaixo(lp, 0);
        return Notopo;
    }
    else{
        printf("Heap VAZIO!");
        return -1;
    }
}

void listap_libera(ListaP *lp)
{
    free(lp->itens);
    free(lp->posnos);
    free(lp);
}

void listap_corrige(ListaP *heap, int novadist, int idno)
{
    heap->itens[heap->posnos[idno]].dist = novadist;
    corrige_acima(heap, idno);
}

static void troca(int pos, int pai, ListaP *heap) {
  int distPai = heap->itens[pai].dist;
  int idnoPai = heap->itens[pai].idno;
  int posnosPai = heap->posnos[heap->itens[pai].idno];

  heap->itens[pai].dist = heap->itens[pos].dist;
  heap->itens[pai].idno = heap->itens[pos].idno;
  heap->posnos[heap->itens[pai].idno] = heap->posnos[heap->itens[pos].idno];

  heap->itens[pos].dist = distPai;
  heap->itens[pos].idno = idnoPai;
  heap->posnos[heap->itens[pos].idno] = posnosPai;  
  
}
void corrige_acima(ListaP *heap, int idno)
{
    int pai, pos = heap->posnos[idno];

    while (pos > 0){
        pai = (pos-1)/2;
        if (heap->itens[pai].dist > heap->itens[pos].dist)
        {
          troca(pos,pai,heap);
        }
        else
            break;
        pos = pai;
    }
}

void corrige_abaixo(ListaP *heap, int atual)
{
    int pai=atual;
    int filho_esq, filho_dir, filho;

    while (2*pai+1 < heap->max){
        filho_esq = 2*pai+1;
        filho_dir = 2*pai+2;
        if (filho_dir >= heap->max) filho_dir = filho_esq;
        if (heap->itens[filho_esq].dist < heap->itens[filho_dir].dist)
            filho = filho_esq;
        else
            filho = filho_dir;
        if (heap->itens[filho].dist < heap->itens[pai].dist)
        {
          troca(filho,pai,heap);
        }
        else
            break;
        pai = filho;
    }
}

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

	int i,no = no_inicial,nv = grafo->nv;
  float menorCaminhoPN[nv + 1];
	ListaP *heap = listap_cria(nv);

  if (no_inicial >= grafo->nv) return NULL;
	int*  ultimosNos = (int*) malloc (sizeof(int)*(nv + 1));
	

  for(i = 0;i<grafo->nv;i++){
  	menorCaminhoPN[i] = INT_MAX;
    listap_insere(heap,INT_MAX,i);
  }
  
  menorCaminhoPN[i] = -1.0;
  menorCaminhoPN[no_inicial] = 0.0;
  

  Viz* viz;
  while( heap->pos > 0)
  {
    printf("\nno = %d\n",no);
  	viz = grafo->viz[no];
  	while(viz != NULL)
  	{
  		if( heap->posnos[viz->noj] != -1 && menorCaminhoPN[viz->noj] > (viz->peso + menorCaminhoPN[no])){
  		menorCaminhoPN[viz->noj] = viz->peso + menorCaminhoPN[no];
      printf("\ninsercao de %d em %d\n",no,viz->noj);
			ultimosNos[viz->noj] = no;
      listap_corrige(heap,viz->peso + menorCaminhoPN[no],viz->noj);
		}
  		viz = viz->prox;
  	}
  	no = listap_remove(heap);
    printf("topo = %d\n",heap->itens[0].idno);

  }
  ultimosNos[no_inicial] = no_inicial;
  ultimosNos[nv] = -1;

  listap_libera(heap);

  return ultimosNos;
}

