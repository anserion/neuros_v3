#ifndef SWEB_H
#define SWEB_H
struct SWEB {
  struct NWEB **nweb;
  int NWEBS_N;
};

struct SWEB *sweb_create(int nwebs_num);
void sweb_destroy(struct SWEB *sweb);
#endif
