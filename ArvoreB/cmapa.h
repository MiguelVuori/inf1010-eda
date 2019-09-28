#include "mapa.h" /* define Mapa */

Mapa* ccria (void);
Mapa* cinsere (Mapa* m, int chave);
int cbusca (Mapa *m, int chave);
Mapa* cretira (Mapa *m, int chave);

void cdestroi (Mapa *m);

int ciguais (Mapa* m1, Mapa* m2);

void cmostra (Mapa* m);
