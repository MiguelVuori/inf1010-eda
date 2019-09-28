#include <stdio.h>
#include "listaprio.h"
#include "debug.h"


int main(void) {
  int info;
  int prios[11] = {21,19,16,22,17,20,23,12,34,15,60};
  ListaP* teste = listap_cria(11,11,prios);
  ListaP* lp = listap_cria(30, 0, NULL);
  debug_listap_show(teste,"teste=");
  listap_libera(teste);
  do {
    printf("prioridade a inserir (negativa para parar): ");
    scanf ("%d", &info);
    if (info<0) break;
    listap_insere(lp,info);
    printf("inseriu %d\n", info);
    debug_listap_show(lp, "lp=");
  } while (info>=0);
  do{
    info = listap_remove(lp);
    printf("removeu %d\n", info);
    debug_listap_show(lp, "lp=");
  } while(info>=0);

  listap_libera(lp);
  return 0;
}