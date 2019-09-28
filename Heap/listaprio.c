#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "listaprio.h"
#include "debug.h"

struct _listaprio {
  int max; /* tamanho maximo do heap */
  int pos; /* proxima posicao disponivel no vetor */ 
  int* prioridade; /* vetor das prioridades */
}; 

static void listap_monta (ListaP* heap, int n, int *prios);
static void corrige_abaixo(int* prios, int atual, int tam);

static void listap_monta (ListaP* heap, int n, int *prios){
  if (n>heap->max) {
    printf ("valores demais! \n");
    exit(1);
  }
  /* completar */
  int i;
  for(i = (n-1);i >= 0;i--)
  {
      heap->prioridade[i] = prios[i];
      if(i<n/2)
          corrige_abaixo(heap->prioridade,i,heap->max);
  }
  heap->pos = n;
}

ListaP* listap_cria(int max, int qtos, int* iniciais){
  ListaP* heap=(ListaP*)malloc(sizeof(struct _listaprio)); 
  heap->max=max;
  heap->pos=0;
  heap->prioridade=(int *)malloc(max*sizeof(int)); 
  if (qtos>0) {
    listap_monta (heap, qtos, iniciais);
  }
  return heap;
}

static void troca(int a, int b, int* v) {
  int f = v[a];
  v[a] = v[b];
  v[b] = f;
}

static void corrige_acima(ListaP* heap, int pos) {
  int pai;
  while (pos > 0){
    pai = (pos-1)/2;
    if (heap->prioridade[pai] < heap->prioridade[pos])
      troca(pos,pai,heap->prioridade);
    else 
      break;
    pos=pai;
  }
}

void listap_insere(ListaP* heap, int prioridade) {
  if ( heap->pos < heap->max ) {
    heap->prioridade[heap->pos]=prioridade; 
    corrige_acima(heap,heap->pos); 
    heap->pos++;
    } 
    else {
      printf("Heap CHEIO!\n"); 
      exit(1);
    }
}

static void corrige_abaixo(int* prios, int atual, int tam){
  int pai=atual;
  int filho_esq, filho_dir, filho;
  while (2*pai+1 < tam){
    filho_esq=2*pai+1;
    filho_dir=2*pai+2;
    if (filho_dir >= tam) filho_dir=filho_esq;
    if (prios[filho_esq]>prios[filho_dir])
        filho=filho_esq;
    else
        filho=filho_dir;
    if (prios[pai]<prios[filho])
            troca(pai,filho,prios);
    else
            break;
    pai=filho;
  }
}

int listap_remove(ListaP* heap) {
  if (heap->pos>0) {
    int topo=heap->prioridade[0];
    heap->prioridade[0]=heap->prioridade[heap->pos-1];
    heap->pos--;
    corrige_abaixo(heap->prioridade, 0, heap->pos);
    return topo;
  }
  else {
     printf("Heap VAZIO!");
     return -1;
  }
}

void listap_libera (ListaP * h) {
  free(h->prioridade);
  free(h);
}

void debug_listap_show (ListaP *h, char* title){
	int i;
	printf("%s={",title);
	for (i=0; i<(h->pos); i++)
		printf("%d,",h->prioridade[i]);
	printf("}\n");

}

int *cria_listaordenada (ListaP *lp, int* tam){
  /* completar */
  int aux, n = *tam;
  while(n)
  {
      aux = listap_remove(lp);
      lp->prioridade[lp->pos] = aux;
      lp->pos++;
      n--;
  }
  return lp->prioridade;
}
    
int* meuheapsort (int nums[], int tam) {
  ListaP *lp = listap_cria (tam, tam, nums);
  int *novosnums = cria_listaordenada (lp, &tam);
  listap_libera (lp);
  return novosnums;
}  

int* meuheapsort2 (int nums[], int tam) {
  int i;
  ListaP *lp = listap_cria (tam, 0, NULL);
  for (i=0;i<tam;i++) listap_insere (lp, nums[i]);
  int *novosnums = cria_listaordenada (lp, &tam);
  listap_libera (lp);
  return novosnums;
}  