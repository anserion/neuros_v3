//addr.c
#include <stdlib.h>
#include "defs.h"
#include "neuron.h"
#include "commutator.h"
#include "nweb.h"
#include "sweb.h"
#include "addr.h"

//фиктивный модуль навигации
void addr_dummy(struct NEURON *nrn) { }

//многоканальный повторитель A.
//в A_table содержатся адреса ячеек назначения
void addr_repeat(struct NEURON *nrn)
{int i,A_mx; double value; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 for(i=1;i<=nrn->M;i++) {
    A_mx=nrn->A_table[i];
    if (i<=nrn->N) value=nrn->A[i]; else value=0;
    commutator_set(comm,nrn->Base+A_mx,value);
 }
}

void addr_imdrepeat(struct NEURON *nrn)
{int i,A_mx; double value; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 for(i=1;i<=nrn->M;i++) {
    A_mx=nrn->A_table[i];
    if (i<=nrn->N) value=nrn->A[i]; else value=0;
    commutator_imd(comm,nrn->Base+A_mx,value);
 }
}

//многоканальный повторитель A
//в A_table содержатся адреса ячеек, содержащих реальные адреса назначения
void addr_relrepeat(struct NEURON *nrn)
{int i,A_mx; double value; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 for(i=1;i<=nrn->M;i++) {
    A_mx=(int)commutator_get(comm,nrn->A_table[i]);
    if (i<=nrn->N) value=nrn->A[i]; else value=0;
    commutator_set(comm,A_mx,value);
 }
}

void addr_imdrelrepeat(struct NEURON *nrn)
{int i,A_mx; double value; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 for(i=1;i<=nrn->M;i++) {
    A_mx=(int)commutator_get(comm,nrn->A_table[i]);
    if (i<=nrn->N) value=nrn->A[i]; else value=0;
    commutator_imd(comm,A_mx,value);
 }
}

//выходная звезда Гроссберга
void addr_outstar(struct NEURON *nrn)
{int i,A_mx; double value; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm; value=nrn->A[0];
 for(i=1;i<=nrn->M;i++) {
    A_mx=nrn->A_table[i];
    commutator_set(nrn->nweb->comm,nrn->Base+A_mx,value);
 }
}

void addr_imdoutstar(struct NEURON *nrn)
{int i,A_mx; double value; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm; value=nrn->A[0];
 for(i=1;i<=nrn->M;i++) {
    A_mx=nrn->A_table[i];
    commutator_imd(nrn->nweb->comm,nrn->Base+A_mx,value);
 }
}

//выходная звезда Гроссберга с относительной адресацией выходов
void addr_reloutstar(struct NEURON *nrn)
{int i,A_mx; double value; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm; value=nrn->A[0];
 for(i=1;i<=nrn->M;i++) {
    A_mx=(int)commutator_get(comm,nrn->Base+nrn->A_table[i]);
    commutator_set(comm,A_mx,value);
 }
}

void addr_imdreloutstar(struct NEURON *nrn)
{int i,A_mx; double value; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm; value=nrn->A[0];
 for(i=1;i<=nrn->M;i++) {
    A_mx=(int)commutator_get(comm,nrn->Base+nrn->A_table[i]);
    commutator_imd(comm,A_mx,value);
 }
}

//запись ответа в ячейку a2 нейросети a1
void addr_internw(struct NEURON *nrn)
{struct COMMUTATOR *far_comm; struct NWEB *far_nw; int nw_num,nw_n;
 nw_num=nrn->A_table[1];
 nw_n=nrn->A_table[2];
 if((nw_num>=0)&&(nw_num<SWEB_MaxN)) {
   far_nw=nrn->nweb->sweb->nweb[nw_num]; far_comm=far_nw->comm;
   commutator_imd(far_comm,nw_n,nrn->A[0]);
 }
}

void addr_relinternw(struct NEURON *nrn)
{struct COMMUTATOR *far_comm; struct NWEB *far_nw; int nw_num,nw_n;
 nw_num=(int)commutator_get(nrn->nweb->comm,nrn->Base+nrn->A_table[1]);
 nw_n=(int)commutator_get(nrn->nweb->comm,nrn->Base+nrn->A_table[2]);
 if((nw_num>=0)&&(nw_num<SWEB_MaxN)) {
 far_nw=nrn->nweb->sweb->nweb[nw_num]; far_comm=far_nw->comm;
 commutator_imd(far_comm,nw_n,nrn->A[0]);
 }
}
