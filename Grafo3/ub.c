#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ub.h"

struct suniaoBusca {
  int n; 
  int *v;};

UniaoBusca* ub_cria(int tam) {
  int i;
  UniaoBusca *ub = (UniaoBusca *) malloc (sizeof(UniaoBusca));
  assert(ub);
  ub->n = tam;
  ub->v = (int *) malloc (tam*sizeof(int));
  assert(ub->v);
  for (i=0;i<tam;i++)
    ub->v[i] = -1;
  return ub;
}

int ub_busca (UniaoBusca* ub, int u){
  int x = u;
  int aux;
  if ((u < 0) || (u > ub->n)) return -1;
  while (ub->v[u] >= 0) u = ub->v[u];
  while (ub->v[x] >= 0) {
    aux = x;
    x = ub->v[x];
    ub->v[aux] = u;
  }
  return u; 
}

int ub_uniao (UniaoBusca* ub, int u, int v) { 
  u = ub_busca (ub, u);
  v = ub_busca (ub, v);
  if ((u<0) || (v<0)) return -1;
  if (ub->v[u] > ub->v[v]) { /* negativos: v[u] menor em modulo! */
    ub->v[v] += ub->v[u];
    ub->v[u] = v;
    return v;
  }
  else {
    ub->v[u] += ub->v[v];
    ub->v[v] = u;
    return u;
  }
}

void  ub_libera (UniaoBusca *ub) {
  free(ub->v);
  free(ub); 
}

void debug (UniaoBusca *ub) {
  int i;
  for (i=0;i<ub->n;i++) printf ("ub[%d]=%d\n", i, ub->v[i]);
}
