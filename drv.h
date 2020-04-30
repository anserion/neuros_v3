#ifndef DRV_H
#define DRV_H
int drv_dummy(struct NWEB *nweb);
int drv_kbd(struct NWEB *nweb);
int drv_load(struct NWEB *nweb);
int drv_save(struct NWEB *nweb);
int drv_display(struct NWEB *nweb);
int drv_mouse(struct NWEB *nweb);
#endif
