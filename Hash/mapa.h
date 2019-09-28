typedef struct smapa Mapa;

Mapa* cria (void);
Mapa* insere (Mapa* m, int chave, int novodado);
int busca (Mapa *m, int chave);
Mapa* retira (Mapa *m, int chave);
void mostra(Mapa *m);
void destroi (Mapa *m);

