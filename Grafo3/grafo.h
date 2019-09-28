typedef struct _grafo Grafo;

Grafo* grafoLe( char* filename );
Grafo* grafoLibera(Grafo* grafo);
void grafoMostra(Grafo* grafo);
Grafo* arvoreCustoMinimo (Grafo* grafo);

/* formato do arquivo de teste.  grafo  
6       ! numero de nos 
5       ! numero de arestas 
0 1 20  ! aresta:  noi noj peso 
1 2 30  
1 3 25
3 4 15
4 5 10
*/
