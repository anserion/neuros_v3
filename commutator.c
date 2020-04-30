//commutator.c
#include <stdlib.h>
#include "defs.h"
#include "commutator.h"

void commutator_zero_0(struct COMMUTATOR *c);
void commutator_zero_1(struct COMMUTATOR *c);
void commutator_cpy(struct COMMUTATOR *c);

void commutator_tick(struct COMMUTATOR *c)
{
  if (c==NULL) return;
  commutator_zero_1(c);
  commutator_cpy(c);
  commutator_zero_0(c);
}

void commutator_cpy(struct COMMUTATOR *c)
{int i; 
 for(i=0;i<c->COMM_N;i++)
   c->panel[2*i+1] = c->panel[2*i];
}

void commutator_zero_0(struct COMMUTATOR *c)
{int i;
 for(i=0;i<c->COMM_N;i++) c->panel[2*i]=0;
}

void commutator_zero_1(struct COMMUTATOR *c)
{int i;
 for(i=0;i<c->COMM_N;i++) c->panel[2*i+1]=0;
}

void commutator_set(struct COMMUTATOR *c,int n, double x)
{
  if (c==NULL) return;
  if ((n>=0)&&(n<c->COMM_N)) c->panel[2*n] = x;
} 

void commutator_imd(struct COMMUTATOR *c,int n, double x)
{
  if (c==NULL) return;
  commutator_set(c,n,x);
  if ((n>=0)&&(n<c->COMM_N)) c->panel[2*n+1] = x;
} 

double commutator_get(struct COMMUTATOR *c,int n)
{
 if (c==NULL) return;
 if ((n>=0)&&(n<c->COMM_N)) return c->panel[2*n+1]; else return 0;
}

struct COMMUTATOR *commutator_create(int n)
{struct COMMUTATOR *c;
 if (n<0) return;
 if (n>COMM_MaxN) n=COMM_MaxN;
 c=(struct COMMUTATOR *)malloc(sizeof(struct COMMUTATOR));
 c->COMM_N=n; c->panel=(double *)malloc(2*c->COMM_N*sizeof(double));
 commutator_zero_0(c); commutator_zero_1(c);
 return c;
}

void commutator_destroy(struct COMMUTATOR *c)
{
  if (c==NULL) return;
  free(c->panel); free(c);
}
