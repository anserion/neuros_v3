//neuron.c
#include <stdlib.h>
#include "defs.h"
#include "commutator.h"
#include "neuron.h"
#include "nweb.h"
#include "f.h"
#include "alg.h"
#include "addr.h"
#include "debug.h"

void neuron_tick(struct NEURON *nrn)
{int i;
 //проверим наличие нейрона
 if (nrn==NULL) return;
 if (nrn->ID<0) return;
 //если на нулевом входе сигнала нет, то выходим
 nrn->X[0]=commutator_get(nrn->nweb->comm,nrn->Base+nrn->X_table[0]);
 if (nrn->X[0]==0) return;
 //загрузим значения входов нейрона из коммутатора
 for(i=1;i<=nrn->N;i++)
    nrn->X[i]=commutator_get(nrn->nweb->comm,nrn->Base+nrn->X_table[i]);
 //вызываем рабочую функцию нейрона
 if (nrn->n_f != F_DUMMY) nrn->F(nrn);
 //активируем модуль обучения нейрона
 if (nrn->n_alg != ALG_DUMMY) nrn->ALG(nrn);
 //активируем модуль навигации нейрона
 if (nrn->n_addr != ADDR_DUMMY) nrn->ADDR(nrn);
}

void neuron_create(int id, int base, int f, int alg, int addr,
	    int n, int k, int m, int *x_table, double *l_table, int *a_table,
	    struct NWEB *nweb)
{struct NEURON *nrn; int i;
 if (nweb==NULL) return;
 //проводим проверку параметров нового нейрона
 if ((id<0)||(id>=nweb->NRN_NUM)) return;
 if (n<0) n=0; if (n>NRN_MaxN) n=NRN_MaxN;
 if (k<0) k=0; if (k>NRN_MaxK) k=NRN_MaxK;
 if (m<0) m=0; if (m>NRN_MaxM) m=NRN_MaxM;
 //если проверки пройдены, то создаем новый нейрон
 nrn=(struct NEURON*)malloc(sizeof(struct NEURON));
 
 nrn->ID=id; nrn->Base=base;
 //задаем номера рабочей функции, модуля обучения и модуля навигации
 nrn->n_f=f; neuron_connect_f(f,nrn);
 nrn->n_alg=alg; neuron_connect_alg(alg,nrn);
 nrn->n_addr=addr; neuron_connect_addr(addr,nrn);
 nrn->N=n; nrn->K=k; nrn->M=m; nrn->nweb=nweb;
 //инициализируем внутреннюю память нейрона
 nrn->X=(double*)malloc((nrn->N+1)*sizeof(double));
 nrn->X_table=(int*)malloc((nrn->N+1)*sizeof(int));
 nrn->L=(double*)malloc((nrn->K+1)*sizeof(double));
 nrn->L_table=(int*)malloc((nrn->K+1)*sizeof(int));
 nrn->A=(double*)malloc((nrn->M+1)*sizeof(double));
 nrn->A_table=(int*)malloc((nrn->M+1)*sizeof(int));
 //инициализируем входы нейрона
 for(i=0;i<=nrn->N;i++) nrn->X[i]=0;
 for(i=0;i<=nrn->N;i++) nrn->X_table[i]=x_table[i];
 //инициализируем вспомогательные коэффициенты нейрона
 for(i=0;i<=nrn->K;i++) nrn->L[i]=l_table[i];
 for(i=0;i<=nrn->K;i++) nrn->L_table[i]=4;//l_table[i]; //обязательно доработать!!!
 //инициализируем выходы нейрона
 for(i=0;i<=nrn->M;i++) nrn->A[i]=0;
 for(i=0;i<=nrn->M;i++) nrn->A_table[i]=a_table[i];
 //заносим созданный нейрон в нейросеть
 nweb->nrn[id]=nrn;
}

void neuron_destroy(int id, struct NWEB *nweb)
{struct NEURON *nrn;
  if (nweb==NULL) return;
  if ((id<0)||(id>=nweb->NRN_NUM)) return;
  nrn=nweb->nrn[id];
  free(nrn->X); free(nrn->X_table);
  free(nrn->L); free(nrn->L_table);
  free(nrn->A); free(nrn->A_table);
  free(nrn); nrn=NULL;
}

void neuron_clear(struct NEURON *nrn)
{int i;
  //очищаем занятую нейроном память,
  //если нейрон действительно существовал
  if (nrn==NULL) return;
  nrn->ID= -1; nrn->Base=0;
  nrn->n_f=0; nrn->n_alg=0; nrn->n_addr=0;
  for(i=0;i<=nrn->N;i++) nrn->X[i]=0;
  for(i=0;i<=nrn->N;i++) nrn->X_table[i]=4;
  for(i=0;i<=nrn->K;i++) nrn->L[i]=0;
  for(i=0;i<=nrn->K;i++) nrn->L_table[i]=4;
  for(i=0;i<=nrn->M;i++) nrn->A[i]=4;
  for(i=0;i<=nrn->M;i++) nrn->A_table[i]=4;
}

void neuron_connect_f(int n_f, struct NEURON *nrn)
{
 switch (n_f) {
//типовые функции активации
 case F_SCALAR: nrn->F=f_scalar; break;
 case F_RELSCALAR: nrn->F=f_relscalar; break;
 case F_SIGMA: nrn->F=f_sigma; break;
 case F_RELSIGMA: nrn->F=f_relsigma; break;
 case F_TANH: nrn->F=f_tanh; break;
 case F_RELTANH: nrn->F=f_reltanh; break;
 case F_STEP: nrn->F=f_step; break;
 case F_RELSTEP: nrn->F=f_relstep; break;
 case F_STEPM1: nrn->F=f_stepm1; break;
 case F_RELSTEPM1: nrn->F=f_relstepm1; break;
 case F_VEXP: nrn->F=f_vexp; break;
 case F_RELVEXP: nrn->F=f_relvexp; break;
 case F_STHSTC: nrn->F=f_sthstc; break;
 case F_RELSTHSTC: nrn->F=f_relsthstc; break;
 case F_EUCLID: nrn->F=f_euclid; break;
 case F_RELEUCLID: nrn->F=f_releuclid; break;
 case F_NORMA: nrn->F=f_norma; break;
 case F_MIN: nrn->F=f_min; break;
 case F_ADDRMIN: nrn->F=f_addrmin; break;
 case F_MAX: nrn->F=f_max; break;
 case F_ADDRMAX: nrn->F=f_addrmax; break;
 case F_REPEAT: nrn->F=f_repeat; break;
 case F_RELREPEAT: nrn->F=f_relrepeat; break;
 case F_INTBIN: nrn->F=f_intbin; break;
 case F_RELINTBIN: nrn->F=f_relintbin; break;
 case F_BININT: nrn->F=f_binint; break;
 case F_RELBININT: nrn->F=f_relbinint; break;
//логические рабочие функции
 case F_NOT: nrn->F=f_not; break;
 case F_AND: nrn->F=f_and; break;
 case F_OR: nrn->F=f_or; break;
 case F_XOR: nrn->F=f_xor; break;
 case F_ANDNOT: nrn->F=f_andnot; break;
 case F_ORNOT: nrn->F=f_ornot; break;
//арифметические рабочие функции
 case F_ZERO: nrn->F=f_zero; break;
 case F_ONE: nrn->F=f_one; break;
 case F_ADD: nrn->F=f_add; break;
 case F_RELADD: nrn->F=f_reladd; break;
 case F_SUB: nrn->F=f_sub; break;
 case F_RELSUB: nrn->F=f_relsub; break;
 case F_PROD: nrn->F=f_prod; break;
 case F_RELPROD: nrn->F=f_relprod; break;
 case F_DIV: nrn->F=f_div; break;
 case F_RELDIV: nrn->F=f_reldiv; break;
 case F_MOD: nrn->F=f_mod; break;
 case F_RELMOD: nrn->F=f_relmod; break;
 case F_POW: nrn->F=f_pow; break;
 case F_RELPOW: nrn->F=f_relpow; break;
 case F_RND: nrn->F=f_rnd; break;
 case F_TRUNC: nrn->F=f_trunc; break;
 case F_SQRT: nrn->F=f_sqrt; break;
 case F_EXP: nrn->F=f_exp; break;
 case F_LOG: nrn->F=f_log; break;
 case F_SIN: nrn->F=f_sin; break;
 case F_COS: nrn->F=f_cos; break;
 case F_TAN: nrn->F=f_tan; break;
 case F_ASIN: nrn->F=f_asin; break;
 case F_ACOS: nrn->F=f_acos; break;
 case F_ATAN: nrn->F=f_atan; break;
 case F_ABS: nrn->F=f_abs; break;
//сервисные рабочие функции
 case F_DRV:  nrn->F=f_drv; break;
 case F_DUMMY: nrn->F=f_dummy; break;
//функции межсетевого взаимодействия
 case F_TICK: nrn->F=f_tick; break;
 case F_TICK1: nrn->F=f_tick1; break;
 case F_RELTICK1: nrn->F=f_reltick1; break;
 case F_CLEAR: nrn->F=f_clear; break;
 case F_INTERREAD: nrn->F=f_interread; break;
 case F_RELINTERREAD: nrn->F=f_relinterread; break;
 case F_CRTNW: nrn->F=f_crtnw; break;
 case F_DELNW: nrn->F=f_delnw; break;

 default: nrn->F=f_dummy;
 }
}

void neuron_connect_alg(int n_alg, struct NEURON *nrn)
{
 switch (n_alg) {
 case ALG_DUMMY: nrn->ALG=alg_dummy; break;
 case ALG_IDSET: nrn->ALG=alg_idset; break;
 case ALG_BASESET: nrn->ALG=alg_baseset; break;
 case ALG_X0SET: nrn->ALG=alg_x0set; break;
 case ALG_L0SET: nrn->ALG=alg_l0set; break;
 case ALG_A0SET: nrn->ALG=alg_a0set; break;
 case ALG_FUNCSET: nrn->ALG=alg_funcset; break;
 case ALG_ALGSET: nrn->ALG=alg_algset; break;
 case ALG_ADDRSET: nrn->ALG=alg_addrset; break;
 case ALG_NSET: nrn->ALG=alg_nset; break;
 case ALG_KSET: nrn->ALG=alg_kset; break;
 case ALG_MSET: nrn->ALG=alg_mset; break;
 case ALG_XSET: nrn->ALG=alg_xset; break;
 case ALG_LSET: nrn->ALG=alg_lset; break;
 case ALG_ASET: nrn->ALG=alg_aset; break;
 default: nrn->ALG=alg_dummy;
 }
}

void neuron_connect_addr(int n_addr, struct NEURON *nrn)
{
  switch (n_addr) {
  case ADDR_DUMMY: nrn->ADDR=addr_dummy; break;
  case ADDR_REPEAT: nrn->ADDR=addr_repeat; break;
  case ADDR_IMDREPEAT: nrn->ADDR=addr_imdrepeat; break;
  case ADDR_RELREPEAT: nrn->ADDR=addr_relrepeat; break;
  case ADDR_IMDRELREPEAT: nrn->ADDR=addr_imdrelrepeat; break;
  case ADDR_OUTSTAR: nrn->ADDR=addr_outstar; break;
  case ADDR_IMDOUTSTAR: nrn->ADDR=addr_imdoutstar; break;
  case ADDR_RELOUTSTAR: nrn->ADDR=addr_reloutstar; break;
  case ADDR_IMDRELOUTSTAR: nrn->ADDR=addr_imdreloutstar; break;
  case ADDR_INTERNW: nrn->ADDR=addr_internw; break;
  case ADDR_RELINTERNW: nrn->ADDR=addr_relinternw; break;
  default: nrn->ADDR=addr_dummy;
  }
}
