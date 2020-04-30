#ifndef DEFS_H
#define DEFS_H
//================================================
#define KERNEL_VERSION "3.CURRENT"
//================================================
//1 FREEBSD
//2 WINDOWS
#define TARGET_OS_CODE 2
//================================================
#define DEBUG_VERBOSE 0
//================================================
#define SWEB_MaxN 1000
#define NWEB_MaxN 1000000
#define NRN_MaxN 2000
#define NRN_MaxK 2000
#define NRN_MaxM 2000
#define COMM_MaxN 1000000
//================================================
//коды команд консоли управления
//================================================
#define CONSOLE_DUMMY      0
#define CONSOLE_STATUS     1
#define CONSOLE_EXIT       6
//================================================
//коды рабочих функций
//================================================
//типовые функции активации
#define F_SCALAR 100
#define F_RELSCALAR 101
#define F_SIGMA  102
#define F_RELSIGMA 103
#define F_TANH     104
#define F_RELTANH 105
#define F_STEP   106
#define F_RELSTEP 107
#define F_STEPM1 108
#define F_RELSTEPM1 109
#define F_VEXP   110
#define F_RELVEXP 111
#define F_STHSTC 112
#define F_RELSTHSTC 113
#define F_EUCLID 114
#define F_RELEUCLID 115
#define F_NORMA 116
#define F_MIN 117
#define F_ADDRMIN 118
#define F_MAX 119
#define F_ADDRMAX 120
#define F_REPEAT 121
#define F_RELREPEAT 122
#define F_INTBIN 123
#define F_RELINTBIN 124
#define F_BININT 125
#define F_RELBININT 126
//логические рабочие функции
#define F_NOT    30
#define F_AND    31
#define F_OR     32
#define F_XOR    33
#define F_ANDNOT 34
#define F_ORNOT  35
//арифметические рабочие функции
#define F_ZERO  0
#define F_ONE   1
#define F_ADD   2
#define F_RELADD 3
#define F_SUB   4
#define F_RELSUB 5
#define F_PROD  6
#define F_RELPROD 7
#define F_DIV   8
#define F_RELDIV 9
#define F_MOD 10
#define F_RELMOD 11
#define F_POW 12
#define F_RELPOW 13
#define F_RND   14
#define F_TRUNC 15
#define F_SQRT 16
#define F_EXP 17
#define F_LOG 18
#define F_SIN 19
#define F_COS 20
#define F_TAN 21
#define F_ASIN 22
#define F_ACOS 23
#define F_ATAN 24
#define F_ABS 25
//сервисные рабочие функции
#define F_DRV  41
#define F_DUMMY 42
//рабочие функции межсетевого взаимодействия
#define F_TICK  50
#define F_TICK1 51
#define F_RELTICK1 57
#define F_CLEAR 52
#define F_INTERREAD  53
#define F_RELINTERREAD  54
#define F_CRTNW 55
#define F_DELNW 56

//================================================
//коды внешних драйверов
//================================================
#define DRV_DUMMY 0
#define DRV_KBD 1
#define DRV_LOAD 2
#define DRV_SAVE 3
#define DRV_DISPLAY 4
#define DRV_MOUSE 5

//================================================
//коды модулей обучения (перенастройки нейронов)
//================================================
#define ALG_DUMMY   0
#define ALG_IDSET   1
#define ALG_BASESET 2
#define ALG_X0SET   3
#define ALG_L0SET   4
#define ALG_A0SET   5
#define ALG_FUNCSET 6
#define ALG_ALGSET  7
#define ALG_ADDRSET 8
#define ALG_NSET    9
#define ALG_KSET    10
#define ALG_MSET    11
#define ALG_XSET    12
#define ALG_LSET    13
#define ALG_ASET    14

//================================================
//коды модулей навигации
//================================================
#define ADDR_DUMMY    0
#define ADDR_REPEAT     1
#define ADDR_IMDREPEAT  2
#define ADDR_RELREPEAT  3
#define ADDR_IMDRELREPEAT  4
#define ADDR_OUTSTAR  5
#define ADDR_IMDOUTSTAR  6
#define ADDR_RELOUTSTAR  7
#define ADDR_IMDRELOUTSTAR  8
#define ADDR_INTERNW  9
#define ADDR_RELINTERNW  10
#endif
