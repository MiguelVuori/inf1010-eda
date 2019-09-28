#include <stdio.h>
#include <stdlib.h>
#include "mapa.h"



int main(void) {

  int chavedada;

  Mapa *mapa1 = cria();

  mostra(mapa1);

  mapa1 = cria();
  while (1) {
    printf ("inserir em mapa1 (digite -111 para terminar): ");
    scanf ("%d", &chavedada);
    if (chavedada==-111) break;
    mapa1 = insere(mapa1, chavedada, 2*chavedada);
    mostra(mapa1);
  }
  while (1) {
    printf ("buscar em mapa1 (digite -111 para terminar): ");
    scanf ("%d", &chavedada);
    if (chavedada==-111) break;
    printf("resultado da busca por %d: %d\n", chavedada, busca(mapa1, chavedada));
  }
    while (1) {
    printf ("retirar em mapa1 (digite -111 para terminar): ");
    scanf ("%d", &chavedada);
    if (chavedada==-111) break;
    mapa1 = retira(mapa1, chavedada);
    mostra(mapa1);
  }
  
  destroi(mapa1);

  return 0;
}