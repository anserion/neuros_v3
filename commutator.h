#ifndef COMMUTATOR_H
#define COMMUTATOR_H
struct COMMUTATOR{
   int COMM_N;
   double *panel;
};

struct COMMUTATOR *commutator_create(int n);
void commutator_destroy(struct COMMUTATOR *c);
void commutator_tick(struct COMMUTATOR *c);
void commutator_set(struct COMMUTATOR *c, int n,double x);
double commutator_get(struct COMMUTATOR *c, int n);
void commutator_imd(struct COMMUTATOR *c, int n,double x);
#endif
