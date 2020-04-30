//nweb.c
#include <stdlib.h>
#include "defs.h"
#include "commutator.h"
#include "neuron.h"
#include "nweb.h"
#include "sweb.h"

void nweb_create(int nweb_id, int neurons_num, int comm_pins, struct SWEB *sweb)
{struct NWEB *nweb; int i;
int x_table[NRN_MaxN], a_table[NRN_MaxM]; double l_table[NRN_MaxK];
 if ((nweb_id<0)||(nweb_id>=SWEB_MaxN)) return;
 if (sweb->nweb[nweb_id]!=NULL) return;
 nweb=(struct NWEB*)malloc(sizeof(struct NWEB));
 sweb->nweb[nweb_id]=nweb;
 if (neurons_num>NWEB_MaxN) neurons_num=NWEB_MaxN;
 if (neurons_num<10) neurons_num=10;
 if (comm_pins<50) comm_pins=50;
 nweb->NRN_NUM=neurons_num; nweb->sweb=sweb; nweb->ID=nweb_id;
 //создаем коммутатор
 nweb->comm=commutator_create(comm_pins);
 //создаем массив ссылок на нейроны сети
 nweb->nrn=(struct NEURON**)malloc(neurons_num*sizeof(struct NEURON*));
 //создаем нейроны-пустышки
 for(i=0;i<nweb->NRN_NUM;i++) {
   neuron_create(i,0,F_DUMMY,ALG_DUMMY,ADDR_DUMMY,0,0,0,x_table,l_table,a_table,nweb);
   nweb->nrn[i]->ID= -1;
 }
}

void nweb_destroy(int nweb_id, struct SWEB *sweb)
{int i; struct NWEB *nweb;
 if (sweb==NULL) return;
 if ((nweb_id<0)||(nweb_id>=sweb->NWEBS_N)) return;
 nweb=sweb->nweb[nweb_id];
 if (nweb==NULL) return;
 //уничтожаем все нейроны сети
 for(i=0;i<nweb->NRN_NUM;i++) neuron_destroy(i,nweb);
 free(nweb->nrn);
 //уничтожаем коммутатор
 commutator_destroy(nweb->comm);
 //уничтожаем все что осталось от нейронной сети
 free(nweb); nweb=NULL;
}

void nweb_clear(struct NWEB *nweb)
{int i,nweb_id; int x_table[10],a_table[10]; double l_table[10]; struct SWEB *sweb;
 if (nweb==NULL) return;
 sweb=nweb->sweb;
 //очищаем содержимое всех нейронов сети
 for(i=0;i<nweb->NRN_NUM;i++) neuron_clear(nweb->nrn[i]);

 //создаем клиентскую часть диспетчера нейронной сети

 //2 - перезагрузка сети
 x_table[0]=12; x_table[1]=10; l_table[0]=0; a_table[0]=0;
 neuron_destroy(2,nweb);
 neuron_create(2,0, F_CLEAR,ALG_DUMMY,ADDR_DUMMY, 1,0,0, x_table,l_table,a_table, nweb);

 //3 - активация сети
 x_table[0]=13; x_table[1]=10; l_table[0]=0; a_table[0]=0;
 neuron_destroy(3,nweb);
 neuron_create(3,0, F_TICK,ALG_DUMMY,ADDR_DUMMY, 1,0,0, x_table,l_table,a_table, nweb);

 //4 - загрузка микропрограммы
 x_table[0]=14; l_table[0]=DRV_LOAD; a_table[0]=0;
 neuron_destroy(4,nweb);
 neuron_create(4,0, F_DRV,ALG_DUMMY,ADDR_DUMMY, 0,0,0, x_table,l_table,a_table, nweb);

 //5 - сохранение микропрограммы
 x_table[0]=15; l_table[0]=DRV_SAVE; a_table[0]=0;
 neuron_destroy(5,nweb);
 neuron_create(5,0, F_DRV,ALG_DUMMY,ADDR_DUMMY, 0,0,0, x_table,l_table,a_table, nweb);

 //6 - создание новой нейросети
 x_table[0]=16; x_table[1]=16; x_table[2]=1; x_table[3]=2; l_table[0]=0; a_table[0]=0;
 neuron_destroy(6,nweb);
 neuron_create(6,0, F_CRTNW,ALG_DUMMY,ADDR_DUMMY, 3,0,0, x_table,l_table,a_table, nweb);

 //7 - уничтожение существующей нейросети
 x_table[0]=17; x_table[1]=17; l_table[0]=0; a_table[0]=0;
 neuron_destroy(7,nweb);
 neuron_create(7,0, F_DELNW,ALG_DUMMY,ADDR_DUMMY, 1,0,0, x_table,l_table,a_table, nweb);

 //очищаем все ячейки коммутатора
 for(i=0;i<nweb->comm->COMM_N;i++) commutator_set(nweb->comm,i,0);
 //устанавливаем cлужебные ячейки коммутатора
 commutator_set(nweb->comm,4,0);
 commutator_set(nweb->comm,5,1);
 commutator_set(nweb->comm,10,nweb->ID);
 commutator_set(nweb->comm,18,nweb->NRN_NUM);
 commutator_set(nweb->comm,19,nweb->comm->COMM_N);
 //пересылка данных коммутатором
 commutator_tick(nweb->comm);
}

void nweb_tick(struct NWEB *nweb)
{int ticks_num,i;
 if (nweb==NULL) return;
 //активируем нейроны клиентской части диспетчера
 for(i=0;i<=2;i++) neuron_tick(nweb->nrn[i]);
 for(i=4;i<=7;i++) neuron_tick(nweb->nrn[i]);
 //выполняем активацию нейронов сети если сеть не заблокирована
 if (commutator_get(nweb->comm,9)==0) {
   //установка системного времени
   ticks_num=commutator_get(nweb->comm,3);
   commutator_set(nweb->comm,3,ticks_num+1);
   //устанавливаем cлужебные ячейки коммутатора
   commutator_set(nweb->comm,4,0);
   commutator_set(nweb->comm,5,1);
   commutator_set(nweb->comm,10,nweb->ID);
   commutator_set(nweb->comm,18,nweb->NRN_NUM);
   commutator_set(nweb->comm,19,nweb->comm->COMM_N);
   //пересылка данных коммутатором
   commutator_tick(nweb->comm);
   //активизация всех нейронов сети с номерами больше 9
   for(i=10;i<nweb->NRN_NUM;i++)
     if (nweb->nrn[i]!=NULL) neuron_tick(nweb->nrn[i]);
 }
}
