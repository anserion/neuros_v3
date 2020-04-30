//alg.c
#include <stdlib.h>
#include "defs.h"
#include "neuron.h"
#include "commutator.h"
#include "nweb.h"
#include "sweb.h"
#include "alg.h"

void alg_dummy(struct NEURON *nrn) {}

//================================================
//модули перенастройки нейронов
//================================================
//изменение значения ID другого нейрона
void alg_idset(struct NEURON *nrn)
{int id,nrn_num; struct NEURON *far_nrn;
  nrn_num=(int)nrn->X[1]; id=(int)nrn->X[2];
  if ((nrn_num>=10)&&(nrn_num<nrn->nweb->NRN_NUM)) {
    far_nrn=nrn->nweb->nrn[nrn_num];
    far_nrn->ID=id;
  }
}

//изменение значение base другого нейрона
void alg_baseset(struct NEURON *nrn)
{int base,nrn_num; struct NEURON *far_nrn;
  nrn_num=(int)nrn->X[1]; base=(int)nrn->X[2];
  if ((nrn_num>=10)&&(nrn_num<nrn->nweb->NRN_NUM)) {
    far_nrn=nrn->nweb->nrn[nrn_num];
    far_nrn->Base=base;
  }
}

//установка новой ячейки коммутатора для включения другого нейрона
void alg_x0set(struct NEURON *nrn)
{int on,nrn_num; struct NEURON *far_nrn;
  nrn_num=(int)nrn->X[1]; on=(int)nrn->X[2];
  if ((nrn_num>=10)&&(nrn_num<nrn->nweb->NRN_NUM)&&
      (on>=0)&&(on<nrn->nweb->comm->COMM_N)) {
    far_nrn=nrn->nweb->nrn[nrn_num];
    far_nrn->X_table[0]=on;
  }
}

//установка нового значения ячейки lambda_0 другого нейрона
void alg_l0set(struct NEURON *nrn)
{int nrn_num; struct NEURON *far_nrn; double value;
  nrn_num=(int)nrn->X[1]; value=nrn->X[2];
  if ((nrn_num>=10)&&(nrn_num<nrn->nweb->NRN_NUM)) {
    far_nrn=nrn->nweb->nrn[nrn_num];
    far_nrn->L[0]=value;
  }
}

//установка нового значения ячейки a_0 таблицы адресов аксона другого нейрона
void alg_a0set(struct NEURON *nrn)
{int a0,nrn_num; struct NEURON *far_nrn;
  nrn_num=(int)nrn->X[1]; a0=(int)nrn->X[2];
  if ((nrn_num>=10)&&(nrn_num<nrn->nweb->NRN_NUM)&&
      (a0>=0)&&(a0<nrn->nweb->comm->COMM_N)) {
    far_nrn=nrn->nweb->nrn[nrn_num];
    far_nrn->A_table[0]=a0;
  }
}

//замена рабочей функции другого нейрона
void alg_funcset(struct NEURON *nrn)
{int n_f,nrn_num; struct NEURON *far_nrn;
  nrn_num=(int)nrn->X[1]; n_f=(int)nrn->X[2];
  if ((nrn_num>=10)&&(nrn_num<nrn->nweb->NRN_NUM)) {
    far_nrn=nrn->nweb->nrn[nrn_num];
    far_nrn->n_f=n_f;
    neuron_connect_f(n_f,far_nrn);
  }
}

//замена модуля обучения другого нейрона
void alg_algset(struct NEURON *nrn)
{int n_alg,nrn_num; struct NEURON *far_nrn;
  nrn_num=(int)nrn->X[1]; n_alg=(int)nrn->X[2];
  if ((nrn_num>=10)&&(nrn_num<nrn->nweb->NRN_NUM)) {
    far_nrn=nrn->nweb->nrn[nrn_num];
    far_nrn->n_alg=n_alg;
    neuron_connect_alg(n_alg,far_nrn);
  }
}

//замена модуля навигации другого нейрона
void alg_addrset(struct NEURON *nrn)
{int n_nav,nrn_num; struct NEURON *far_nrn;
  nrn_num=(int)nrn->X[1]; n_nav=(int)nrn->X[2];
  if ((nrn_num>=10)&&(nrn_num<nrn->nweb->NRN_NUM)) {
    far_nrn=nrn->nweb->nrn[nrn_num];
    far_nrn->n_addr=n_nav;
    neuron_connect_addr(n_nav,far_nrn);
  }
}

//изменение числа входов другого нейрона
void alg_nset(struct NEURON *nrn)
{int i,n,nrn_num; struct NEURON *far_nrn;
  nrn_num=(int)nrn->X[1]; n=(int)nrn->X[2];
  if ((nrn_num>=10)&&(nrn_num<nrn->nweb->NRN_NUM)) {
    far_nrn=nrn->nweb->nrn[nrn_num];
    far_nrn->N=n;
    free(far_nrn->X); free(far_nrn->X_table);
    far_nrn->X=(double*)malloc((n+1)*sizeof(double));
    far_nrn->X_table=(int*)malloc((n+1)*sizeof(int));
    for(i=0;i<=n;i++) {far_nrn->X[i]=0; far_nrn->X_table[i]=4-far_nrn->Base;}
  }
}

//изменение числа вспомогательных коэффициентов другого нейрона
void alg_kset(struct NEURON *nrn)
{int i,k,nrn_num; struct NEURON *far_nrn;
  nrn_num=(int)nrn->X[1]; k=(int)nrn->X[2];
  if ((nrn_num>=10)&&(nrn_num<nrn->nweb->NRN_NUM)) {
    far_nrn=nrn->nweb->nrn[nrn_num];
    far_nrn->K=k;
    free(far_nrn->L);
    far_nrn->L=(double*)malloc((k+1)*sizeof(double));
    for(i=0;i<=k;i++) far_nrn->L[i]=0;
  }
}

//изменение числа возможных точек подключения аксона другого нейрона
void alg_mset(struct NEURON *nrn)
{int i,m,nrn_num; struct NEURON *far_nrn;
  nrn_num=(int)nrn->X[1]; m=(int)nrn->X[2];
  if ((nrn_num>=10)&&(nrn_num<nrn->nweb->NRN_NUM)) {
    far_nrn=nrn->nweb->nrn[nrn_num];
    far_nrn->M=m;
    free(far_nrn->A); free(far_nrn->A_table);
    far_nrn->A=(double*)malloc((m+1)*sizeof(double));
    far_nrn->A_table=(int*)malloc((m+1)*sizeof(int));
    for(i=0;i<=m;i++) {far_nrn->A[i]=0; far_nrn->A_table[i]=4-far_nrn->Base;}
  }
}

//перезагрузка точек подключения дендритов другого нейрона
void alg_xset(struct NEURON *nrn)
{int base,nrn_num,i; struct NEURON *far_nrn; struct COMMUTATOR *comm;
  nrn_num=(int)nrn->X[1]; base=(int)nrn->X[2];
  comm=nrn->nweb->comm;
  if ((nrn_num>=10)&&(nrn_num<nrn->nweb->NRN_NUM)) {
    far_nrn=nrn->nweb->nrn[nrn_num];
    if ((base>=0)&&(base<comm->COMM_N - far_nrn->N)) {
      for(i=1;i<=far_nrn->N;i++)
        far_nrn->X_table[i]=(int)commutator_get(comm,base+i-1);
    }
  }
}

//перезагрузка вспомогательных коэффициентов другого нейрона
void alg_lset(struct NEURON *nrn)
{int base,nrn_num,i; struct NEURON *far_nrn; struct COMMUTATOR *comm;
  nrn_num=(int)nrn->X[1]; base=(int)nrn->X[2];
  comm=nrn->nweb->comm;
  if ((nrn_num>=10)&&(nrn_num<nrn->nweb->NRN_NUM)) {
    far_nrn=nrn->nweb->nrn[nrn_num];
    if ((base>=0)&&(base<comm->COMM_N - far_nrn->K)) {
      for(i=1;i<=far_nrn->K;i++)
        far_nrn->L[i]=commutator_get(comm,base+i-1);
    }
  }
}

//перезагрузка таблицы возможных точек подключения аксона другого нейрона
void alg_aset(struct NEURON *nrn)
{int base,nrn_num,i; struct NEURON *far_nrn; struct COMMUTATOR *comm;
  nrn_num=(int)nrn->X[1]; base=(int)nrn->X[2];
  comm=nrn->nweb->comm;
  if ((nrn_num>=10)&&(nrn_num<nrn->nweb->NRN_NUM)) {
    far_nrn=nrn->nweb->nrn[nrn_num];
    if ((base>=0)&&(base<comm->COMM_N - far_nrn->M)) {
      for(i=1;i<=far_nrn->M;i++)
        far_nrn->A_table[i]=(int)commutator_get(comm,base+i-1);
    }
  }
}
