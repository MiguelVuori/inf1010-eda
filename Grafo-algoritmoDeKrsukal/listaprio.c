#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "listaprio.h"

struct item {
  int prioridade;
  int n1, n2;
};

struct _listaprio {
  int max; /* tamanho maximo do heap */
  int pos; /* proxima posicao disponivel no vetor */ 
  struct item *itens;
}; 

static void corrige_abaixo(ListaP* heap, int atual);

ListaP* listap_cria(int max){
  ListaP* heap=(ListaP*)malloc(sizeof(struct _listaprio)); 
  heap->max=max;
  heap->pos=0;
  heap->itens=(struct item *)malloc(max*sizeof(struct item)); 
  return heap;
}

static void troca(int a, int b, struct item* v) {
  struct item f = v[a];
  v[a] = v[b];
  v[b] = f;
}

static void corrige_acima(ListaP* heap, int pos) {
  int pai;
  while (pos > 0){
    pai = (pos-1)/2;
    if (heap->itens[pai].prioridade >  heap->itens[pos].prioridade) {
      troca(pos,pai,heap->itens);
    }
    else 
      break;
    pos=pai;
  }
}

void listap_insere(ListaP* heap, int prioridade, int d1, int d2) {
  if ( heap->pos < heap->max ) {
    heap->itens[heap->pos].prioridade=prioridade; 
    heap->itens[heap->pos].n1 = d1;
    heap->itens[heap->pos].n2 = d2;
    corrige_acima(heap,heap->pos); 
    heap->pos++;
    } 
    else {
      printf("Heap CHEIO!\n"); 
      exit(1);
    }
}

static void corrige_abaixo(ListaP* heap, int atual){
  int pai=atual;
  int filho_esq, filho_dir, filho;
  while (2*pai+1 < heap->pos){
    filho_esq=2*pai+1;
    filho_dir=2*pai+2;
    if (filho_dir >= heap->pos) filho_dir=filho_esq;
    if (heap->itens[filho_esq].prioridade<heap->itens[filho_dir].prioridade)
        filho=filho_esq;
    else
        filho=filho_dir;
    if (heap->itens[pai].prioridade>heap->itens[filho].prioridade) {
            troca(pai,filho,heap->itens);
    }
    else
            break;
    pai=filho;
  }
}

int listap_remove(ListaP* heap, int *i1, int* i2) {
  int prio;
  if (heap->pos>0) {
    *i1=heap->itens[0].n1;
    *i2=heap->itens[0].n2;
    prio = heap->itens[0].prioridade;
    heap->itens[0] = heap->itens[heap->pos-1];
    heap->pos--;
    corrige_abaixo(heap, 0);
    return prio;
  }
  else {
     return -1;
  }
}

void listap_libera (ListaP * h) {
  free(h->itens);
  free(h);
}

void debug_listap_show (ListaP *h, char* title){
	int i;
	printf("%s={",title);
	for (i=0; i<(h->pos); i++)
		printf(" %d-%d,%d,",h->itens[i].prioridade, h->itens[i].n1,
                                 h->itens[i].n2);
	printf("}\n");

}
