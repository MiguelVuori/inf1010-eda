#include <stdio.h>
#include "mapa.h"
#include "arvore.h"

int main (void) {

  int chave;

  Mapa *ummapa = cria();

  while (1) {
    printf ("chave a inserir (-111 para terminar): ");
    scanf ("%d", &chave);
    if (chave==-111) break;

    ummapa = insere (ummapa, chave, 2*chave);
    mostra (ummapa);
  }

  destroi(ummapa); 

  return 0;}

                  

