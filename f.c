//f.c
#include <stdlib.h>
#include <math.h>
#include "defs.h"
#include "neuron.h"
#include "commutator.h"
#include "drv.h"
#include "nweb.h"
#include "sweb.h"
#include "f.h"

//================================================
//типовые функции активации нейронов
//================================================
//рабочая функция, рассчитывающая скалярное произведение входного
//вектора на вектор вспомогательных коэффициентов
void f_scalar(struct NEURON *nrn)
{int i; double f=0;
 for(i=1;i<=nrn->N;i++)
    if (i<=nrn->K) f=f+nrn->X[i] * nrn->L[i];
 nrn->A[0]=f;
}

void f_relscalar(struct NEURON *nrn)
{int i; double f=0; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 for(i=1;i<=nrn->N;i++)
   if (i<=nrn->K)
     f=f+nrn->X[i] * commutator_get(comm,(int)(nrn->Base+nrn->L[i]));
 nrn->A[0]=f;
}

//сигмовидная функция активации нейрона
void f_sigma(struct NEURON *nrn)
{double f;
 f_scalar(nrn);
 f=1/(1+exp(- nrn->L[0]*nrn->A[0]));
 nrn->A[0]=f;
}

void f_relsigma(struct NEURON *nrn)
{double f; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 f_relscalar(nrn);
 f=1/(1+exp(-commutator_get(comm,(int)(nrn->Base+nrn->L[0]))*nrn->A[0]));
 nrn->A[0]=f;
}

//гиперболический тангенс
void f_tanh(struct NEURON *nrn)
{double scalar,f;
 f_scalar(nrn); scalar=nrn->A[0];
 f=(exp(scalar)-exp(-scalar))/(exp(scalar)+exp(-scalar));
 nrn->A[0]=f;
}

void f_reltanh(struct NEURON *nrn)
{double scalar,f;
 f_relscalar(nrn); scalar=nrn->A[0];
 f=(exp(scalar)-exp(-scalar))/(exp(scalar)+exp(-scalar));
 nrn->A[0]=f;
}

//функция, обратная экспоненте от скалярного произведения
void f_vexp(struct NEURON *nrn)
{double f;
 f_scalar(nrn); f=exp(-nrn->A[0]);
 nrn->A[0]=f;
}

void f_relvexp(struct NEURON *nrn)
{double f;
 f_relscalar(nrn); f=exp(-nrn->A[0]);
 nrn->A[0]=f;
}

//жесткая ступенька
void f_step(struct NEURON *nrn)
{
 f_scalar(nrn);
 if (nrn->A[0]>=nrn->L[0]) nrn->A[0]=1; else nrn->A[0]=0;
}

void f_relstep(struct NEURON *nrn)
{struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 f_relscalar(nrn);
 if (nrn->A[0]>=commutator_get(comm,(int)(nrn->Base+nrn->L[0]))) nrn->A[0]=1; else nrn->A[0]=0;
}

//симметричная относительно начала координат жесткая ступенька
void f_stepm1(struct NEURON *nrn)
{
 f_scalar(nrn);
 if (nrn->A[0]>=nrn->L[0]) nrn->A[0]=1; else nrn->A[0]= -1;
}

void f_relstepm1(struct NEURON *nrn)
{struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 f_relscalar(nrn);
 if (nrn->A[0]>=commutator_get(comm,(int)(nrn->Base+nrn->L[0]))) nrn->A[0]=1; else nrn->A[0]= -1;
}

//стохастическая функция
void f_sthstc(struct NEURON *nrn)
{double P;
 f_scalar(nrn);
 P=1/(1+exp(-nrn->A[0]/nrn->L[0]));
 if (rand()%1000000 <= P*1000000) nrn->A[0]=1; else nrn->A[0]= -1;
}

void f_relsthstc(struct NEURON *nrn)
{double P; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 f_relscalar(nrn);
 P=1/(1+exp(-nrn->A[0]/commutator_get(comm,(int)(nrn->Base+nrn->L[0]))));
 if (rand()%1000000 <= P*1000000) nrn->A[0]=1; else nrn->A[0]= -1;
}

//евклидова мера близости векторов
void f_euclid(struct NEURON *nrn)
{double s=0; int i;
 for(i=1;i<nrn->N;i++)
    if (i<=nrn->K) s=s+(nrn->X[i]-nrn->L[i])*(nrn->X[i]-nrn->L[i]);
 nrn->A[0]=s;
}

void f_releuclid(struct NEURON *nrn)
{double s=0; int i; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 for(i=1;i<=nrn->N;i++)
   if (i<=nrn->K)
     s=s+(nrn->X[i]-commutator_get(comm,(int)(nrn->Base+nrn->L[i])))*
         (nrn->X[i]-commutator_get(comm,(int)(nrn->Base+nrn->L[i])));
 nrn->A[0]=s;
}

//приведение входного вектора X к единичной длине
void f_norma(struct NEURON *nrn)
{double s,value; int i,addr; struct COMMUTATOR *comm;
 f_euclid(nrn); s=nrn->A[0];
 if (s>0) for(i=1;i<=nrn->N;i++) nrn->X[i]=nrn->X[i]/s;
}

//нахождение минимального значения из множества входов
void f_min(struct NEURON *nrn)
{int i; double value=0;
if (nrn->N>0) value=nrn->X[1];
for(i=1;i<=nrn->N;i++)
  if (nrn->X[i]<value) value=nrn->X[i];
nrn->A[0]=value;
}

//нахождение адреса минимального значения из множества входов
void f_addrmin(struct NEURON *nrn)
{int i,addr=0; double value=0;
if (nrn->N>0) {addr=nrn->X_table[1]; value=nrn->X[1];}
for(i=1;i<=nrn->N;i++)
  if (nrn->X[i]<value) {addr=nrn->X_table[i]; value=nrn->X[i];}
nrn->A[0]=addr;
}

//нахождение максимального значения из множества входов
void f_max(struct NEURON *nrn)
{int i; double value=0;
if (nrn->N>0) value=nrn->X[1];
for(i=1;i<=nrn->N;i++)
  if (nrn->X[i]>value) value=nrn->X[i];
nrn->A[0]=value;
}

//нахождение адреса максимального значения из множества входов
void f_addrmax(struct NEURON *nrn)
{int i,addr=0; double value=0;
if (nrn->N>0) {addr=nrn->X_table[1]; value=nrn->X[1];}
for(i=1;i<=nrn->N;i++)
  if (nrn->X[i]>value) {addr=nrn->X_table[i]; value=nrn->X[i];}
nrn->A[0]=addr;
}

//повторитель входов нейрона x_0,..,x_n
void f_repeat(struct NEURON *nrn)
{int i;
 for(i=0;i<=nrn->N;i++) nrn->A[i]=nrn->X[i];
}

void f_relrepeat(struct NEURON *nrn)
{int i; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 for(i=0;i<=nrn->N;i++) nrn->A[i]=commutator_get(comm,(int)(nrn->X[i]));
}

//преобразователь скалярного произведения входов нейрона в двоичный код (-1,1)
void f_intbin(struct NEURON *nrn)
{int i; int P;
 f_scalar(nrn);
 P=(int)nrn->A[0];
 for(i=1;i<=nrn->N;i++) nrn->A[i]=2*((P>>(i-1))&1)-1;
}

void f_relintbin(struct NEURON *nrn)
{int i; int P;
 f_relscalar(nrn);
 P=(int)nrn->A[0];
 for(i=1;i<=nrn->N;i++) nrn->A[i]=2*((P>>(i-1))&1)-1;
}

//преобразователь двоичного кода (-1,1) входов нейрона в целое число
void f_binint(struct NEURON *nrn)
{int i; int S=0;
 for(i=nrn->N;i>=1;i--) S=S*2+(nrn->X[i]+1)/2;
 nrn->A[0]=S;
}

void f_relbinint(struct NEURON *nrn)
{int i; int S=0; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 for(i=nrn->N;i>=1;i--) S=S*2+(commutator_get(comm,(int)(nrn->X[i]))+1)/2;
 nrn->A[0]=S;
}

//===============================================
//логические рабочие функции
//===============================================
//логическое отрицание
void f_not(struct NEURON *nrn)
{int i;
 for(i=1;i<=nrn->N;i++) nrn->A[i]=(double)(!((int)nrn->X[1]));
}

//логическое побитовое И
void f_and(struct NEURON *nrn)
{int i,res;
res=(int)nrn->X[1];
for(i=2;i<=nrn->N;i++) res=res & (int)nrn->X[i];
nrn->A[0]=(double)res;
}

//логическое побитовое ИЛИ
void f_or(struct NEURON *nrn) 
{int i,res;
res=(int)nrn->X[1];
for(i=2;i<=nrn->N;i++) res=res | (int)nrn->X[i];
nrn->A[0]=(double)res;
}

//логическое побитовое ИСКЛЮЧАЮЩЕЕ ИЛИ
void f_xor(struct NEURON *nrn)
{int i,res;
res=(int)nrn->X[1];
for(i=2;i<=nrn->N;i++) res=res ^ (int)nrn->X[i];
nrn->A[0]=(double)res;
}

//логическое И-НЕ
void f_andnot(struct NEURON *nrn)
{int i,res;
res=(int)nrn->X[1];
for(i=2;i<=nrn->N;i++) res=res && (int)nrn->X[i];
nrn->A[0]=(double)(!res);
}

//логическое ИЛИ-НЕ
void f_ornot(struct NEURON *nrn)
{int i,res;
res=(int)nrn->X[1];
for(i=2;i<=nrn->N;i++) res=res || (int)nrn->X[i];
nrn->A[0]=(double)(!res);
}

//===============================================
//арифметические рабочие функции
//===============================================
//всегда возвращающая нуль рабочая функция
void f_zero(struct NEURON *nrn) 
{int i;
 for(i=0;i<=nrn->N;i++) if (i<=nrn->M) nrn->A[i]=0;
}

//всегда возвращающая единицу рабочая функция
void f_one(struct NEURON *nrn) 
{int i;
 for(i=0;i<=nrn->N;i++) if (i<=nrn->M) nrn->A[i]=1;
}

//рабочая функция, генерирующая псевдослучайное число
void f_rnd(struct NEURON *nrn) 
{int i;
 for(i=1;i<=nrn->N;i++) if (i<=nrn->M) nrn->A[i]=rand()%((int)nrn->X[i]);
}

//рабочая функция, отбрасывающая дробную часть числа
void f_trunc(struct NEURON *nrn) 
{int i;
 for(i=1;i<=nrn->N;i++) if (i<=nrn->M) nrn->A[i]=(int)nrn->X[i];
}

//вычисление корня квадратного числа x_i
void f_sqrt(struct NEURON *nrn)
{int i;
 for(i=1;i<=nrn->N;i++) if (i<=nrn->M) nrn->A[i]=sqrt(nrn->X[i]);
}

//вычисление экспоненты числа x_i
void f_exp(struct NEURON *nrn)
{int i;
 for(i=1;i<=nrn->N;i++) if (i<=nrn->M) nrn->A[i]=exp(nrn->X[i]);
}

//вычисление натурального логарифма числа x_i
void f_log(struct NEURON *nrn)
{int i;
 for(i=1;i<=nrn->N;i++) if (i<=nrn->M) 
   if (nrn->X[i]>0) nrn->A[i]=log(nrn->X[i]); else nrn->A[i]=0;
}

//расчет синуса от значения входа x_i нейрона
void f_sin(struct NEURON *nrn)
{int i;
 for(i=1;i<=nrn->N;i++) if (i<=nrn->M) nrn->A[i]=sin(nrn->X[i]);
}

//расчет косинуса от значения входа x_i нейрона
void f_cos(struct NEURON *nrn)
{int i;
 for(i=1;i<=nrn->N;i++) if (i<=nrn->M) nrn->A[i]=cos(nrn->X[i]);
}

//расчет тангенса от значения входа x_i нейрона
void f_tan(struct NEURON *nrn)
{int i;
 for(i=1;i<=nrn->N;i++) if (i<=nrn->M) nrn->A[i]=tan(nrn->X[i]);
}

//расчет арксинуса от значения входа x_i нейрона
void f_asin(struct NEURON *nrn)
{int i;
 for(i=1;i<=nrn->N;i++) if (i<=nrn->M) nrn->A[i]=asin(nrn->X[i]);
}

//расчет арккосинуса от значения входа x_i нейрона
void f_acos(struct NEURON *nrn)
{int i;
 for(i=1;i<=nrn->N;i++) if (i<=nrn->M) nrn->A[i]=acos(nrn->X[i]);
}

//расчет арктангенса от значения входа x_i нейрона
void f_atan(struct NEURON *nrn)
{int i;
 for(i=1;i<=nrn->N;i++) if (i<=nrn->M) nrn->A[i]=atan(nrn->X[i]);
}

//расчет модуля (абсолютной величины) от значения входа x_i нейрона
void f_abs(struct NEURON *nrn)
{int i;
 for(i=1;i<=nrn->N;i++) if (i<=nrn->M) nrn->A[i]=fabs(nrn->X[i]);
}

//вычисление числа x_i в степени L_i
void f_pow(struct NEURON *nrn)
{int i;
 for(i=1;i<=nrn->N;i++) if (i<=nrn->K) nrn->A[i]=pow(nrn->X[i],nrn->L[i]);
}

void f_relpow(struct NEURON *nrn)
{int i; double value; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 for(i=1;i<=nrn->N;i++)
   if (i<=nrn->K)
     nrn->A[i]=pow(nrn->X[i],commutator_get(comm,(int)(nrn->Base+nrn->L[i])));
}

//арифметическое сложение
void f_add(struct NEURON *nrn)
{int i;
 for(i=1;i<=nrn->N;i++) if (i<=nrn->K) nrn->A[i]=(nrn->X[i] + nrn->L[i]);
}

void f_reladd(struct NEURON *nrn)
{int i; double value; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 for(i=1;i<=nrn->N;i++)
   if (i<=nrn->K)
     nrn->A[i]=nrn->X[i]+commutator_get(comm,(int)(nrn->Base+nrn->L[i]));
}

//арифметическое вычитание
void f_sub(struct NEURON *nrn)
{int i;
 for(i=1;i<=nrn->N;i++) if (i<=nrn->K) nrn->A[i]=(nrn->X[i] - nrn->L[i]);
}

void f_relsub(struct NEURON *nrn)
{int i; double value; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 for(i=1;i<=nrn->N;i++)
   if (i<=nrn->K)
     nrn->A[i]=nrn->X[i]-commutator_get(comm,(int)(nrn->Base+nrn->L[i]));
}

//арифметическое умножение
void f_prod(struct NEURON *nrn)
{int i;
 for(i=1;i<=nrn->N;i++) if (i<=nrn->K) nrn->A[i]=(nrn->X[i] * nrn->L[i]);
}

void f_relprod(struct NEURON *nrn)
{int i; double value; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 for(i=1;i<=nrn->N;i++)
   if (i<=nrn->K)
     nrn->A[i]=nrn->X[i] * commutator_get(comm,(int)(nrn->Base+nrn->L[i]));
}

//арифметическое деление
void f_div(struct NEURON *nrn)
{int i;
 for(i=1;i<=nrn->N;i++)
   if (i<=nrn->K)
     if (nrn->L[i] != 0) nrn->A[i]= nrn->X[i] / nrn->L[i]; else nrn->A[i]=0;
}

void f_reldiv(struct NEURON *nrn)
{int i; double value,f; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 for(i=1;i<=nrn->N;i++)
   if (i<=nrn->K) {
     f=commutator_get(comm,(int)(nrn->Base+nrn->L[i]));
     if (f!=0) nrn->A[i]=nrn->X[i]/f; else nrn->A[i]=0;
     }
}

//нахождение остатков от деления
void f_mod(struct NEURON *nrn)
{int i; double value,f; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 for(i=1;i<=nrn->N;i++)
   if (i<=nrn->K) {
     f=nrn->L[i];
     if (f!=0) nrn->A[i]=(int)(nrn->X[i])%(int)f; else nrn->A[i]=0;
     }
}

void f_relmod(struct NEURON *nrn)
{int i; double value,f; struct COMMUTATOR *comm;
 comm=nrn->nweb->comm;
 for(i=1;i<=nrn->N;i++)
   if (i<=nrn->K) {
     f=commutator_get(comm,(int)(nrn->Base+nrn->L[i]));
     if (f!=0) nrn->A[i]=(int)(nrn->X[i])%(int)f; else nrn->A[i]=0;
     }
}

//================================================
//сервисные рабочие функции
//================================================
//точки входа во внешние драйверы
void f_drv(struct NEURON *nrn)
{
  switch ((int)nrn->L[0]) {
  case DRV_DUMMY: drv_dummy(nrn->nweb); break;
  case DRV_KBD: drv_kbd(nrn->nweb); break;
  case DRV_LOAD: drv_load(nrn->nweb); break;
  case DRV_SAVE: drv_save(nrn->nweb); break;
  case DRV_DISPLAY: drv_display(nrn->nweb); break;
  case DRV_MOUSE: drv_mouse(nrn->nweb); break;
  }
}

//ничего не делающая рабочая функция
void f_dummy(struct NEURON *nrn) { }

//================================================
//функции, отвечающие за межсетевое взаимодействие
//================================================
//активация других нейросетей
void f_tick(struct NEURON *nrn)
{int nw_num,i;
for(i=1;i<nrn->N;i++) {
  nw_num=(int)nrn->X[i];
  if ((nw_num>=0)&&(nw_num<SWEB_MaxN))
    nweb_tick(nrn->nweb->sweb->nweb[nw_num]);
}
}

//активация одиночных нейронов других нейросетей
void f_tick1(struct NEURON *nrn)
{struct NWEB *far_web; int nw_num,far_nrn,i;
for(i=1;i<nrn->N;i++){
  nw_num=(int)nrn->X[i];
  far_nrn=(int)nrn->L[i];
  if ((nw_num>=0)&&(nw_num<SWEB_MaxN)) {
    far_web=nrn->nweb->sweb->nweb[nw_num];
    if(far_web!=NULL) {
    if ((far_nrn>=0)&&(far_nrn<far_web->NRN_NUM)) {
      neuron_tick(far_web->nrn[far_nrn]);
      }
    }
  }
}
}

void f_reltick1(struct NEURON *nrn)
{struct NWEB *far_web; int nw_num,far_nrn,i; struct COMMUTATOR *comm;
comm=nrn->nweb->comm;
for(i=1;i<nrn->N;i++){
  nw_num=(int)nrn->X[i];
  far_nrn=(int)commutator_get(comm,(int)(nrn->Base+nrn->L[i]));
  if ((nw_num>=0)&&(nw_num<SWEB_MaxN)) {
    far_web=nrn->nweb->sweb->nweb[nw_num];
    if(far_web!=NULL) {
    if ((far_nrn>=0)&&(far_nrn<far_web->NRN_NUM)) {
      neuron_tick(far_web->nrn[far_nrn]);
      }
    }
  }
}
}

//перезагрузка других нейросетей
void f_clear(struct NEURON *nrn)
{int nw_num,i;
for(i=1;i<nrn->N;i++) {
  nw_num=(int)nrn->X[i];
  if ((nw_num>=0)&&(nw_num<SWEB_MaxN))
    nweb_clear(nrn->nweb->sweb->nweb[nw_num]);
}
}

//создание новой нейросети и ее инициализация
void f_crtnw(struct NEURON *nrn)
{int nw_num,neurons_num,comm_pins; struct SWEB *sweb;
  nw_num=(int)nrn->X[1];
  neurons_num=(int)nrn->X[2]; comm_pins=(int)nrn->X[3];
  //проверка корректности введенных параметров
  if ((nw_num>=0)&&(nw_num<SWEB_MaxN)) {
    sweb=nrn->nweb->sweb;
    //нейросеть будем создавать если ее действительно не было
    if (sweb->nweb[nw_num]==NULL) {
      nweb_create(nw_num,neurons_num,comm_pins,sweb);
      nweb_clear(sweb->nweb[nw_num]);
    }
  }
}

//удаление существующих нейросетей
void f_delnw(struct NEURON *nrn)
{int nw_num,i;
for(i=1;i<nrn->N;i++) {
  nw_num=(int)nrn->X[i];
  if (nw_num!=nrn->nweb->ID) nweb_destroy(nw_num, nrn->nweb->sweb);
}
}

//чтение содержимого ячейки коммутатора другой нейросети
void f_interread(struct NEURON *nrn)
{struct COMMUTATOR *far_comm; struct NWEB *far_nweb;
 double value=0; int nw_num,far_n,i;
 for(i=1;i<nrn->N;i++) {
  nw_num=(int)nrn->X[i];
  far_n=(int)nrn->L[i];
  if ((nw_num>=0)&&(nw_num<SWEB_MaxN)) {
    far_nweb=nrn->nweb->sweb->nweb[nw_num];
    if (far_nweb!=NULL) {
       far_comm=far_nweb->comm;
       value=commutator_get(far_comm,far_n);
       nrn->A[i]=value;
       }
  }
 }
}

void f_relinterread(struct NEURON *nrn)
{struct COMMUTATOR *far_comm; struct NWEB *far_nweb; struct COMMUTATOR *comm;
 double value=0; int nw_num,far_n,i;
 comm=nrn->nweb->comm;
 for(i=1;i<nrn->N;i++) {
  nw_num=(int)nrn->X[i];
  far_n=(int)commutator_get(comm,nrn->Base+(int)nrn->L[i]);
  if ((nw_num>=0)&&(nw_num<SWEB_MaxN)) {
    far_nweb=nrn->nweb->sweb->nweb[nw_num];
    if (far_nweb!=NULL) {
       far_comm=far_nweb->comm;
       value=commutator_get(far_comm,far_n);
       nrn->A[i]=value;
       }
  }
 }
}
