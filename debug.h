#ifndef DEBUG_H
#define DEBUG_H
void nweb_report(struct NWEB *nweb);
void debug_init();
void debug_msg(const char *str);
void debug_msgxy(int x, int y, const char *str);
int debug_getstr_xy(int x, int y, char *str);
void debug_clear();
void debug_endwin();
#endif
