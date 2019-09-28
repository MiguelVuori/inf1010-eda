#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "set.h"

#define TAM 32

/* cria um conjunto com n elementos */
Set setCreate(void){
  Set set = (Set)malloc(sizeof(int));
  if (set != NULL) *set = 0;
  return set;
}

/* destroi (desaloca) o conjunto */
Set setDestroy(Set set) {
  if (set) free(set);
  return NULL;
}

/* cria uma copia do conjunto */
Set setCopy(Set set)
{
	Set set2 = setCreate();
	*set2 = *set; 
  return set2;
}

/* cria um novo conjunto que e' complementar ao primeiro*/
Set setComplement(Set set){
	Set set2 = setCreate();
	*set2 = ~(*set)
  return set2;
}

/* insere o elemento i no conjunto */
void setInsert(Set set, int i) 
{
	*set = *set | (1<<i);
	
}

/* remove o elemento i do conjunto */
void setRemove(Set set, int i){
	*set = *set & ~(1<<i);
} 


/* testa se o elemento i pertence ao conjunto */
int setIsMember(Set set, int i){
	int aux;
  if (set==NULL) return 0;
  if ((i<0) || (i>32)) return 0;

  return (1<<i) & (*set);
}


/* cria a uniao de dois conjunto que podem ter tamanhos diferentes */
/* naturalmente o numero de elementos do universo e' o do maior dos dois */
Set setUnion( Set set1,  Set set2){
  if (set1==NULL || set2 == NULL) return 0;
  /* aqui fazer o caso normal */
	Set setnovo = copySet(set1);
	*setnovo = *setnovo | *set2;
  return setnovo;
}

/* calcula a intersecao de dois conjuntos */
Set setIntersection( Set set1,  Set set2){
  if (set1==NULL || set2 == NULL) return NULL;
  /* aqui fazer o caso normal */
	Set setnovo = copySet(set1);
	*setnovo = *setnovo & *set2;
  return setnovo;
}

/* calcula a diferenca de set1-set2 */
Set setDifference(Set set1, Set set2){
  if (set1==NULL || set2 == NULL) return NULL;
  /* aqui fazer o caso normal */
	Set setnovo = setCreate();
	*setnovo = *set1 & ~(*set2);

  return setnovo;

}

/* verifica de set2 e' um subconjunto de set1 */
int setIsSubset( Set set1,  Set set2) {
  if (set1==NULL || set2 == NULL) return 0;
  /* aqui fazer o caso normal */
	/*Set setAux = setCreate();
	setAux = setDifference(set2,set1);
	return (*setAux == 0);}
	*/
	if (set1 == NULL || set2 == NULL) return 0;
	return ((*set1 & *set2) == *set2)

}

/* verifica se dois conjuntos sao iguais */
int setIsEqual( Set set1,  Set set2) {
  if (set1==NULL || set2 == NULL) return 0;
  /* aqui fazer o caso normal */
	return *set1 == *set2;
  return 0;
}

/* informa a cardinalidade do conjunto */
int setNumberOfElements(Set set){
	int i,num;
	num = 0;
	for (i=0;i<32;i++)
	{
		if(setIsMember(set,i))
		{
			num++;
		}
	}

  return num;

}

/* mostra os elementos do conjunto */
void setShow(char* title, Set set){
  int i, first=1;
  printf("%s = {", title);
  for (i = 0; i < 32; i++) 
    if (setIsMember(set, i)) {
      if (first) { 
      	printf("%d", i); 
      	first = 0; 
      }
      else 
      	printf(",%d", i);
    }
  printf("}\n\n");
}
