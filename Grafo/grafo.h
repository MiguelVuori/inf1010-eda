typedef struct _grafo Grafo;

Grafo* grafoLe( char* filename );
Grafo* grafoLibera(Grafo* grafo);
void grafoMostra(char* title, Grafo* grafo);
void grafoPercorreProfundidade(Grafo * grafo, int no_inicial);
void grafoPercorreProfundidade2(Grafo * grafo, int no_inicial);
void grafoPercorreLargura(Grafo * grafo, int no_inicial);

/* formato esperado por grafoLe
6       ! numero de nos 
5       ! numero de arestas 
0 1 20  ! aresta:  noi noj peso 
1 2 30  
1 3 25
3 4 15
4 5 10
*/