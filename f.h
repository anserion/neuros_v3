#ifndef F_H
#define F_H
//типовые функции активации
void f_scalar(struct NEURON *nrn);
void f_relscalar(struct NEURON *nrn);
void f_sigma(struct NEURON *nrn);
void f_relsigma(struct NEURON *nrn);
void f_tanh(struct NEURON *nrn);
void f_reltanh(struct NEURON *nrn);
void f_step(struct NEURON *nrn);
void f_relstep(struct NEURON *nrn);
void f_stepm1(struct NEURON *nrn);
void f_relstepm1(struct NEURON *nrn);
void f_vexp(struct NEURON *nrn);
void f_relvexp(struct NEURON *nrn);
void f_sthstc(struct NEURON *nrn);
void f_relsthstc(struct NEURON *nrn);
void f_euclid(struct NEURON *nrn);
void f_releuclid(struct NEURON *nrn);
void f_norma(struct NEURON *nrn);
void f_min(struct NEURON *nrn);
void f_addrmin(struct NEURON *nrn);
void f_max(struct NEURON *nrn);
void f_addrmax(struct NEURON *nrn);
void f_repeat(struct NEURON *nrn);
void f_relrepeat(struct NEURON *nrn);
void f_intbin(struct NEURON *nrn);
void f_relintbin(struct NEURON *nrn);
void f_binint(struct NEURON *nrn);
void f_relbinint(struct NEURON *nrn);

//логические рабочие функции
void f_not(struct NEURON *nrn);
void f_and(struct NEURON *nrn);
void f_or(struct NEURON *nrn);
void f_xor(struct NEURON *nrn);
void f_andnot(struct NEURON *nrn);
void f_ornot(struct NEURON *nrn);

//арифметические рабочие функции
void f_zero(struct NEURON *nrn);
void f_one(struct NEURON *nrn);
void f_add(struct NEURON *nrn);
void f_reladd(struct NEURON *nrn);
void f_sub(struct NEURON *nrn);
void f_relsub(struct NEURON *nrn);
void f_prod(struct NEURON *nrn);
void f_relprod(struct NEURON *nrn);
void f_div(struct NEURON *nrn);
void f_reldiv(struct NEURON *nrn);
void f_mod(struct NEURON *nrn);
void f_relmod(struct NEURON *nrn);
void f_pow(struct NEURON *nrn);
void f_relpow(struct NEURON *nrn);
void f_trunc(struct NEURON *nrn);
void f_rnd(struct NEURON *nrn);
void f_sqrt(struct NEURON *nrn);
void f_exp(struct NEURON *nrn);
void f_log(struct NEURON *nrn);
void f_sin(struct NEURON *nrn);
void f_cos(struct NEURON *nrn);
void f_tan(struct NEURON *nrn);
void f_asin(struct NEURON *nrn);
void f_acos(struct NEURON *nrn);
void f_atan(struct NEURON *nrn);
void f_abs(struct NEURON *nrn);
//сервисные рабочие функции
void f_drv(struct NEURON *nrn);
void f_dummy(struct NEURON *nrn);

//рабочие функции межсетевого взаимодействия
void f_tick(struct NEURON *nrn);
void f_tick1(struct NEURON *nrn);
void f_reltick1(struct NEURON *nrn);
void f_clear(struct NEURON *nrn);
void f_interread(struct NEURON *nrn);
void f_relinterread(struct NEURON *nrn);
void f_crtnw(struct NEURON *nrn);
void f_delnw(struct NEURON *nrn);
#endif
