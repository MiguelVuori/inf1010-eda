#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mapa.h"

#define TAMINICIAL 11

typedef struct {
  int chave;
  int dados;
  int prox;
} ttabpos;

struct smapa {
  int tam;
  int ocupadas;
  ttabpos *tabpos;
};

static unsigned int hash1 (Mapa* m, int a) {
  return a%(m->tam);
}
  
/*
static unsigned int hash2(Mapa* m, int a) {
  a = (a ^ 61) ^ (a >> 16);
  a = a + (a << 3);
  a = a ^ (a >> 4);
  a = a * 0x27d4eb2d;
  a = a ^ (a >> 15);
  return a%m->tam;
}
*/

static unsigned int hash (Mapa* m, int chave) {
  return hash1(m, chave);
}

Mapa* cria () {
  int i;
  Mapa* m = (Mapa*) malloc (sizeof(Mapa));
  if (m==NULL) {printf("erro na aloca��o! \n"); exit(1);}
  m->tabpos = (ttabpos*) malloc (TAMINICIAL*sizeof(ttabpos)); 
  if (m->tabpos==NULL) {printf("erro na aloca��o! \n"); exit(1);}
  m->tam = TAMINICIAL;
  m->ocupadas = 0;
  for (i=0;i<TAMINICIAL;i++) {
    m->tabpos[i].chave = -1;
    m->tabpos[i].prox = -1;
  }
  return m;
}

static void redimensiona (Mapa* m) {
	int i;
	Mapa* m2 = (Mapa*) malloc (sizeof(Mapa));
    if (m2==NULL) {printf("erro na aloca��o! \n"); exit(1);}
    m2->tabpos = (ttabpos*) malloc ((m->tam + 11)*sizeof(ttabpos)); 
    if (m2->tabpos==NULL) {printf("erro na aloca��o! \n"); exit(1);}
    m2->tam = m->tam + 11;
	m2->ocupadas = 0;
	for (i=0;i<m2->tam;i++) 
	{
		m2->tabpos[i].chave = -1;
		m2->tabpos[i].prox = -1;
	}

	for (i=0;i < m2->tam-11;i++)
	{	
		if(m->tabpos[i].chave != -1)
			insere(m2,m->tabpos[i].chave,m->tabpos[i].dados);
	}
	*m=*m2; /* esqueci de destruir o m antigo */
	destroi(m2);
}

/* inser��o: sup�e que a chave dada N�O est� no mapa */
Mapa* insere (Mapa* m, int chave, int dados) {

  if (m->ocupadas > 0.75*m->tam) redimensiona(m);
  int pos = hash(m, chave);
  
  if (m->tabpos[pos].chave == -1) { /* est� vazia */
    m->tabpos[pos].chave = chave;
    m->tabpos[pos].dados = dados;
    m->tabpos[pos].prox = -1;
  }
  else { /* conflito */
    /* procura proxima posi��o livre */
    int poslivre = pos;
    do 
      poslivre = (poslivre+1) % (m->tam);
    while ((poslivre!=pos) && (m->tabpos[poslivre].chave!=-1));

    if (poslivre==pos) { /* tabela cheia -- n�o deveria acontecer */
      printf ("p�nico, tabela cheia!\n"); exit(1);
    }
    /* achou posicao livre  - verificar quem vai para ela */
    int hashocupadora = hash(m, m->tabpos[pos].chave);
    if (hashocupadora==pos) { /* conflito primario: encadeia */
    /* completar */
		m->tabpos[poslivre].prox = m->tabpos[pos].prox;
		m->tabpos[pos].prox = poslivre;
		m->tabpos[poslivre].chave = chave;
		m->tabpos[poslivre].dados = dados;
		
    }
    else { /* conflito secundario: expulsa o item atual de pos */
    /* completar */
		m->tabpos[poslivre].chave = m->tabpos[pos].chave;
		m->tabpos[poslivre].dados = m->tabpos[pos].dados;
		m->tabpos[poslivre].prox = m->tabpos[pos].prox;

		while(pos != m->tabpos[hashocupadora].prox)
			hashocupadora = m->tabpos[hashocupadora].prox;

		m->tabpos[hashocupadora].prox = poslivre;
		m->tabpos[pos].prox = -1;
		m->tabpos[pos].chave = chave;
		m->tabpos[pos].dados = dados;
    }
  }
  (m->ocupadas)++; /* aumentou o n�mero de itens na tabela */
  return m;
}

int busca (Mapa *m, int chave) {
  if (m==NULL) return -1;
  /* completar */
  int pos = hash(m,chave);
  while(pos != -1)
  {
	if (m->tabpos[pos].chave == chave)return pos;
	pos = m->tabpos[pos].prox;
  }
  return -1;
}

void destroi (Mapa *m) {
  free (m->tabpos);
  free (m);
}

int iguais (Mapa* m1, Mapa* m2) {
  int i;

  if (m1==NULL || m2==NULL) return (m1==NULL && m2==NULL);
  if (m1->tam != m2->tam) return 0;
  ttabpos* tp1 = m1->tabpos;
  ttabpos* tp2 = m2->tabpos;
  for (i = 0; i < m1->tam; i++)
    if ((tp1[i].chave != tp2[i].chave) ||
        (tp1[i].prox != tp2[i].prox))
      return 0;
  return 1;
}

void mostra (Mapa* m) {
  int i;
  for (i=0;i<m->tam;i++) 
    if (m->tabpos[i].chave!=-1) 
      printf ("posicao %d, chave %d, proximo %d\n", i, m->tabpos[i].chave, m->tabpos[i].prox);
}

Mapa* retira (Mapa *m, int chave)
{
	int pos = hash(m,chave);

	if (m->tabpos[pos].chave == chave)
	{
		int prox = m->tabpos[pos].prox;
		if (prox != -1)
		{
			m->tabpos[pos].chave = m->tabpos[prox].chave;
			m->tabpos[pos].dados = m->tabpos[prox].dados;
			m->tabpos[pos].prox = m->tabpos[prox].prox;
			m->tabpos[prox].chave = -1;
			m->tabpos[prox].prox = -1;
		}
		else
		{
			m->tabpos[pos].chave = -1;
			m->tabpos[pos].prox = -1;
		}
	}
	else
	{
		int anterior;
		while(m->tabpos[pos].chave != chave)
		{
			if(pos == -1)return m;
			anterior = pos;
			pos = m->tabpos[pos].prox;
		}
		m->tabpos[anterior].prox = m->tabpos[pos].prox;
		m->tabpos[pos].chave = -1;
		m->tabpos[pos].prox = -1;
	}
	m->ocupadas--;
	return m;

}