#ifndef NEURON_H
#define NEURON_H
#include "defs.h"
struct NEURON {
  int ID,Base;
  int n_f, n_alg, n_addr;
  void (*F)(struct NEURON *nrn);
  void (*ALG)(struct NEURON *nrn);
  void (*ADDR)(struct NEURON *nrn);
  struct NWEB *nweb;
  int N,K,M;
  double *X, *L, *A;
  int *X_table, *L_table, *A_table;
};

void neuron_create(int id, int base, int f, int alg, int addr,
	    int n, int k, int m, int *x_table, double* l_table, int *a_table,
	    struct NWEB *nweb);
void neuron_connect_f(int n_f, struct NEURON *nrn);
void neuron_connect_alg(int n_alg, struct NEURON *nrn);
void neuron_connect_addr(int n_addr, struct NEURON *nrn);
void neuron_destroy(int id, struct NWEB *nweb);
void neuron_clear(struct NEURON *neuron);
void neuron_tick(struct NEURON *neuron);
#endif
