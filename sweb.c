//sweb.c
#include <stdlib.h>
#include "defs.h"
#include "commutator.h"
#include "neuron.h"
#include "nweb.h"
#include "sweb.h"

struct SWEB *sweb_create(int nwebs_num)
{struct SWEB *sweb; int i;
 sweb=(struct SWEB*)malloc(sizeof(struct SWEB));
 //создаем массив ссылок на нейросети в суперсети
 if (nwebs_num<1) return NULL;
 if (nwebs_num>=SWEB_MaxN) nwebs_num=SWEB_MaxN;
 sweb->NWEBS_N=nwebs_num;
 sweb->nweb=(struct NWEB**)malloc(nwebs_num*sizeof(struct NWEB*));
 for(i=0;i<nwebs_num;i++) sweb->nweb[i]=NULL;
 //возвращаем указатель на суперсеть
 return sweb;
}

void sweb_destroy(struct SWEB *sweb)
{int i;
 //уничтожаем все нейронные сети в суперсети
 for(i=0;i<sweb->NWEBS_N;i++) nweb_destroy(i,sweb);
 free(sweb);
}
