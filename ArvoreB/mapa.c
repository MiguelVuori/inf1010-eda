#include <stdio.h>
#include <stdlib.h>

#include "mapa.h"
#include "arvore.h"

enum minhaposicao {ESQ, MEIO, DIR};
typedef enum minhaposicao tpos;

enum resultadoretirada {OK, RETIRA_MENOR, RETIRA_MAIOR};
typedef enum resultadoretirada tresultret;

struct smapa
{
	int  kp, kg;   /* chaves: kp<kg, se kg existir. Se kg=-1, significa que ele não existe. */
	Mapa  *pai;
	Mapa  *esq;
	Mapa  *meio;
	Mapa  *dir;
};

Mapa* cria (void) {
  return NULL;
}

static Mapa* novoNo (int chave) {
  Mapa *m = (Mapa*) malloc(sizeof(struct smapa));
  if (m==NULL) { printf ("erro no malloc! \n"); exit(1);}
  m->pai = NULL;
  m->kp = chave; 
  m->kg = -1;
  m->esq = m->meio = m->dir = NULL;
  return m;
}

static Mapa* overflowQuebra (Mapa *m, int *valorainserir, Mapa* novofilho) {
  Mapa* novo;

  novo = (Mapa*) malloc(sizeof(struct smapa));

  if (novo==NULL) { printf ("erro em malloc! \n"); exit(1);}

  if (*valorainserir < m->kp) {
    novo->esq = novofilho;

    if (novo->esq) novo->esq->pai = novo;

    novo->kp = *valorainserir;
    novo->meio = m->esq;

    if (novo->meio) novo->meio->pai = novo;

    novo->kg = -1;
    novo->dir = NULL;
    *valorainserir = m->kp;
    m->esq = m->meio;
    m->kp = m->kg;
  }
  else if (*valorainserir < m->kg) {
	
    /* completar */
	novo->esq = m->esq;
	novo->meio = novofilho;
	novo->kp = m->kp;

	if (novo->esq) novo->esq->pai = novo;
	if (novo->meio) novo->meio->pai = novo;

	novo->kg = -1;
	novo->dir = NULL;
	m->kp = m->kg;
	m->esq = m->meio;

  } 
  else {
	
    /* completar */
	novo->esq = m->esq;
	novo->meio = m->meio;
	novo->kp = m->kp;

	if (novofilho) novofilho->pai = m;
	if (novo->esq) novo->esq->pai = novo;
	if (novo->meio) novo->meio->pai = novo;

	novo->kg = -1;
	novo->dir = NULL;
	m->kp = *valorainserir;
	*valorainserir = m->kg;
	m->esq = novofilho;
	
  }
  m->meio = m->dir;
  m->kg = -1;
  m->dir = NULL;
 
  return novo;
}

static int insere2 (Mapa* m, int chave, int* valorainserir, Mapa** novofilho) {

  int inseriraqui = 0; /* indica se deve inserir neste nó */

  if (m==NULL) {
    printf("erro! subárvore nula! \n"); exit (1);
  }

  if (m->esq != NULL) { /* não é folha, só insere neste nó se subir um valor  */
    if (chave < m->kp) {
      inseriraqui = insere2(m->esq, chave, valorainserir, novofilho); 
    }
    else if (((m->kg != -1) && (chave < m->kg)) || (m->kg == -1)) {
      /* ou está entre as duas chaves ou só tem uma chave no nó */
      inseriraqui = insere2(m->meio, chave, valorainserir, novofilho);
    }
    else { /* chave > m->kg */
      inseriraqui = insere2(m->dir, chave, valorainserir, novofilho);
    }
  }
  else { /* este nó é folha, tem que inserir nele de qq jeito */
    *valorainserir = chave;
    inseriraqui = 1;
    *novofilho = NULL;
  } 

  if (!inseriraqui) return 0; /* inserção já está completa */

  /* procura espaço no nó */

  if (m->kg==-1) {
  /* tem  espaço no nó */
    if (*valorainserir < m->kp) { /* empurra chave existente para a direita */
      /* COMPLETAR */
		m->kg = m->kp;
		m->dir = m->meio;
		m->meio = m->esq;
		m->kp = *valorainserir;
		m->esq = *novofilho;
		if (m->esq) m->esq->pai = m;
		
    }
    else { /* é maior que a chave que já está lá */
      m->kg = *valorainserir;
      m->dir = m->meio;
      m->meio = *novofilho;
      if (m->meio) m->meio->pai = m;
    }
    return 0; /* como havia espaço, não sobem valores a serem inseridos */
  }
  
  *novofilho = overflowQuebra (m, valorainserir, *novofilho); 
 
  return 1; /* quando há quebra sempre sobe a mediana para nova inserção */ 
}

Mapa* insere (Mapa* m, int chave) {
  int valorquesubiu;
  Mapa* novofilho;
  Mapa* novaraiz;

  if (m==NULL) {
    m = novoNo (chave);
    m->pai = novoNo (-1);
  }
  else {
    if (insere2 (m, chave, &valorquesubiu, &novofilho)) {
      /* cria nova raiz */
      novaraiz = novoNo (valorquesubiu);
      novaraiz->pai = m->pai;
      novaraiz->esq = novofilho;
      novaraiz->esq->pai = novaraiz;
      novaraiz->meio = m;
      novaraiz->meio->pai = novaraiz;
      m = novaraiz;
    } 
  }
  return m;
}

int busca (Mapa *m, int chave) {
	
	if (m == NULL)return 0;

	if (chave == m->kp || chave == m->kg)return 1;

	if (chave < m->kp)return busca(m->esq,chave);
	if (chave < m->kg || (m->kg == -1))return busca(m->meio,chave);
	return busca(m->dir,chave);
}

void destroi (Mapa *m) {
  if (m!= NULL) {
    destroi (m->esq);
    destroi (m->meio);
    destroi (m->dir);
    free (m);
  }
}

static void mostra_int  (Mapa *m) {
  printf ("[");
  if (m!= NULL) {
    printf ("%d:%d->%d ", m->kp, m->kg, m->pai->kp);
    mostra_int (m->esq);
    mostra_int (m->meio);
    if (m->kg != -1) mostra_int(m->dir);
  }
  printf ("]");
}

void mostra  (Mapa *m) {
  mostra_int (m);
  printf("\n");
}

int iguais (Mapa *m1, Mapa *m2) {
  if (m1==NULL) return (m2==NULL);
  if (m2==NULL) return 0;

  return (m1->kp == m2->kp) &&
          (m1->kg == m2->kg) &&
          iguais (m1->esq, m2->esq) &&
          iguais (m1->meio, m2->meio) &&
          iguais (m1->dir, m2->dir) &&
          (m1->pai->kp == m2->pai->kp);
}
