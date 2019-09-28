#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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

/* aux para fila e pilha */
typedef struct _sq SQ;
struct _sq {
  int info;
  SQ* prox;
};

static SQ* newList (){
  return NULL;
}

static SQ* enqueue(SQ* queue, int info){
  SQ* novo = (SQ*)malloc(sizeof(SQ));
  SQ* aux = queue;
  assert(novo);
  novo->info = info;
  novo->prox = NULL;
  if (!queue){
    return novo;
  }
  while (aux->prox){
    aux = aux->prox;
  }
  aux->prox = novo;
  return queue;
}

static SQ* dequeue(SQ* queue,int* info){
  SQ* libera;
  if (!queue){
    *info = -1;
    return NULL;
  }
  *info = queue->info;
  libera = queue;
  queue = queue->prox;
  free(libera);
  return queue;
}

static SQ* pop(SQ* stack,int* popped_info){
  SQ* libera;
  if (!stack){
    *popped_info = -1;
    return NULL;
  }
  *popped_info = stack->info;
  libera = stack;
  stack = stack->prox;
  free(libera);
  return stack;
}

static SQ* push(SQ* stack, int info){
  SQ* novo = (SQ*)malloc(sizeof(SQ));
  assert(novo);
  novo->info = info;
  novo->prox = stack;
  return novo;
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
/* cria grafo nÃ£o orientado - supÃµe que arquivo estÃ¡ correto! */

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

Grafo*  grafoLibera(Grafo* grafo) {
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

void GPP(Grafo *grafo, int no_inicial,int* inseridos)
{
    Viz* viz = grafo->viz[no_inicial];
    
    printf("No atual: %d\n",no_inicial);
    inseridos[no_inicial] = 1;
    while (viz != NULL)
    {
        if(inseridos[viz->noj] == 0)
            GPP(grafo,viz->noj,inseridos);

        viz = viz->prox;
    }
    

}
void grafoPercorreProfundidade(Grafo *grafo, int no_inicial){
  printf ("\n");
  if(no_inicial >= grafo->nv)
  {
    printf("Erro! esse no nao existe\n");
    exit(0);
  }
  int i;
  int inseridos[grafo->nv];
  for(i=0;i<grafo->nv;i++)
  {
    inseridos[i] = 0;
  }
GPP(grafo,no_inicial,inseridos);
} 

void GPL(Grafo *grafo,SQ* fila,int* enfileirados)
{
    while(fila != NULL)
    {
        int no;
        fila = dequeue(fila,&no);
        printf("No atual: %d\n",no);

        Viz* viz = grafo->viz[no];
        while(viz != NULL)
        {
            
            if(enfileirados[viz->noj] == 0)
            {
                fila = enqueue(fila,viz->noj);
                enfileirados[viz->noj] = 1;
            }
            viz = viz->prox;
        }

    }
}

void grafoPercorreLargura(Grafo *grafo, int no_inicial){
    printf ("\n");
    if(no_inicial >= grafo->nv)
    {
      printf("Erro! esse no nao existe\n");
      exit(0);
    }
    int i;
    SQ* fila = newList ();
    int enfileirados[grafo->nv];

    fila = enqueue(fila,no_inicial);

    for(i=0;i<grafo->nv;i++)
    {
        enfileirados[i] = 0;
    }
    enfileirados[no_inicial] = 1;

    GPL(grafo,fila,enfileirados);

    
} 

void GPP2 (Grafo *grafo,SQ* pilha,int* empilharados)
{
    while(pilha != NULL)
    {
        int no;
        pilha = pop(pilha,&no);
        printf("No atual: %d\n",no);

        Viz* viz = grafo->viz[no];
        while(viz != NULL)
        {
            
            if(empilharados[viz->noj] == 0)
            {
                pilha = push(pilha,viz->noj);
                empilharados[viz->noj] = 1;
            }
            viz = viz->prox;
        }

    }

}

void grafoPercorreProfundidade2 (Grafo *grafo, int no_inicial){
printf ("\n");
if(no_inicial >= grafo->nv)
{
  printf("Erro! esse no nao existe\n");
  exit(0);
}
int i;
SQ* pilha = newList ();
int empilharados[grafo->nv];

pilha = push(pilha,no_inicial);

for(i=0;i<grafo->nv;i++)
{
  empilharados[i] = 0;
}
empilharados[no_inicial] = 1;

GPP2(grafo,pilha,empilharados);

}  
/* Implementacao mais correta de dfs */
/*
void grafoPercorreProfundidade2 (Grafo *grafo, int no_inicial){
	if (grafo == NULL) return;
	int pivot;
	int seen[grafo->nv];
	SQ *stack = newList();
	Viz *viz;

	for (pivot = 0; pivot < grafo->nv; pivot++) seen[pivot] = 0;

	pivot = no_inicial;
	while (pivot != -1) {
		if (!seen[pivot]) {
			printf("%d ", pivot);
			seen[pivot] = 1;
			viz = grafo->viz[pivot];
			while (viz) {
				stack = push(stack, viz->noj);
				viz = viz->prox;
			}	
		}
		
		stack = pop(stack, &pivot);
	}

	printf ("\n");

} */

