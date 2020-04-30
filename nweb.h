#ifndef NWEB_H
#define NWEB_H
struct NWEB {
  int NRN_NUM,ID;
  struct NEURON **nrn;
  struct COMMUTATOR *comm;
  struct SWEB *sweb;
};

void nweb_create(int nweb_id, int neurons_num, int comm_pins, struct SWEB *sweb);
void nweb_destroy(int nweb_id, struct SWEB *sweb);
void nweb_clear(struct NWEB *nweb);
void nweb_tick(struct NWEB *nweb);
#endif
