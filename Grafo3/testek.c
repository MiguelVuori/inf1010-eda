#include <stdio.h>
#include <stdlib.h>
#include "ub.h"
#include "listaprio.h"
#include "grafo.h"

int main (int argc, char** argv) {

  Grafo* original, *arvore;

  if (argc != 2) {
    printf ("uso: %s <nomearquivografo>\n", argv[0]);
    exit(1);
  }
  
  original = grafoLe(argv[1]);

  printf("grafo dado: \n");
  grafoMostra(original);

  arvore = arvoreCustoMinimo (original);

  printf("árvore geradora de custo mínimo: \n");
  grafoMostra(arvore);

  return 0;

} 
