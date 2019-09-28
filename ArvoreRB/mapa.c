#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "mapa.h"

struct smapa {
  int chave;
  int dado;
  char vermelho;
  struct smapa* esq;
  struct smapa *dir;
};

typedef enum result {OK, RED, LEFTRED, RIGHTRED} Result;

static char *strstatus[] = {"ok", "red", "leftred", "rightred"};

static Mapa *cria_no (int c, int novodado) {
  Mapa *nn = (Mapa *)malloc(sizeof(Mapa));
  if (nn!=NULL) {
    nn->esq = nn->dir = NULL;
    nn->chave =c;
    nn->vermelho = 1;
    nn->dado = novodado;
  }
  return nn;
}

Mapa* cria (void) {
  return NULL;
}

void destroi (Mapa *m) {
  if (m!= NULL) {
    destroi (m->esq);
    destroi (m->dir);
    free(m);
  }
}

static void mostra_mapa_int (Mapa* m) {

  printf("(");
  if (m != NULL) {
    printf("%d-%d", m->chave,m->vermelho);
    mostra_mapa_int(m->esq);
    mostra_mapa_int(m->dir);
  }
  printf(")");
}

void mostra (Mapa* m) {
  mostra_mapa_int (m); printf ("\n");
}

static Mapa* rotaciona_dir(Mapa *m) {
  Mapa* esq = m->esq;
  m->esq = esq->dir;
  esq->dir = m;
  return esq;
}

static Mapa* rotaciona_esq(Mapa *m) {
  Mapa* dir = m->dir;
  m->dir = dir->esq;
  dir->esq = m;
  return dir;
}

static Mapa* trocaCores (Mapa *m) {
  char corpai = m->vermelho; 
  m->vermelho = !corpai;
  m->esq->vermelho = corpai;
  m->dir->vermelho = corpai;
  return m;
}

static Mapa* corrigeDir (Mapa *m, Result* status) {
  switch (*status) {
    case OK:
      break;
    case RED:
      if (m->vermelho) *status = RIGHTRED;
      else *status = OK;
      break;
    case RIGHTRED:
      if (!m->esq || !m->esq->vermelho) {  /* tio vazio/preto */
		printf("RRb\n");
		m = rotaciona_esq(m);
		m = trocaCores(m);
		*status = OK;
      }
      else if (m->esq->vermelho) {  /* tio vermelho */
		printf("RRr\n");
		m = trocaCores(m);
		*status = RED;
      }
      break;
    case LEFTRED:
      if (!m->esq || !m->esq->vermelho) { /* tio vazio/preto */
		printf("RLb\n"); 
        m->dir = rotaciona_dir (m->dir);
        m = rotaciona_esq (m);
        m = trocaCores(m);
        *status = OK;
      }
      else if (m->esq->vermelho) {
		printf("RLr\n");
        m = trocaCores (m);
        *status = RED;
      }
      break;
  }
  return m;
}

static Mapa* corrigeEsq (Mapa *m, Result* status) {

  switch (*status) {
    case OK:
      break;
    case RED:
      if (m->vermelho) *status = LEFTRED;
      else *status = OK;
      break;
    case LEFTRED:
      if (!m->dir || !m->dir->vermelho) {  /* tio vazio/preto */
		printf("LLb - LEFTRED: tio preto\n");
		m = rotaciona_dir(m);
		m = trocaCores(m);
		*status = OK;
      }
      else if (m->dir->vermelho) { /* tio é vermelho */
		printf("LLr - LEFTRED: tio vermelho\n");
		m = trocaCores(m);
		*status = RED;
      }
      break;
    case RIGHTRED:
      if (!m->dir || !m->dir->vermelho) {  /* tio vazio/preto */
		printf("LRb\n");
		m->esq = rotaciona_esq (m->esq);
        m = rotaciona_dir (m);
        m = trocaCores(m);
        *status = OK;
      }
      else if (m->dir->vermelho) {
		printf("LRr\n");
		 m = trocaCores (m);
        *status = RED;
      }
      break;
  }
  return m;
}
          

static Mapa* insereRec (Mapa* m, int chave, int novodado, Result* status){
  if (m==NULL) {
    m = cria_no (chave, novodado);
    *status = RED;
  }
  else if (chave < m->chave) {
    m->esq = insereRec (m->esq, chave, novodado, status);
    m = corrigeEsq (m, status);
  }
  else if (chave > m->chave) {
    m->dir = insereRec (m->dir, chave, novodado, status);
    m = corrigeDir (m, status);
  }
  return m;
}


Mapa* insere (Mapa* m, int chave, int novodado) {
  Result status;
  m = insereRec (m, chave, novodado, &status);
  if (status == RED) m->vermelho = 0;
  else if (status != OK) {
    printf ("erro ao voltar para a raiz: status invalido %s !\n", strstatus[status]);
    mostra(m);
    exit(1);
  }
  return m;
}