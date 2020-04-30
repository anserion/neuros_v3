//debug.c
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include "defs.h"
#include "commutator.h"
#include "neuron.h"
#include "nweb.h"
#include "debug.h"

extern int global_key;
extern int global_x;
extern int global_y;
extern int global_btn;

//инициализация ввода-вывода консоли
void debug_init()
{
}

//вывод текстового сообщения консоли в заданных координатах экрана
void debug_msgxy(int x, int y, const char *str)
{int i=0;
glColor3f(1,0,0);
glPushMatrix();
while(str[i]!=0) {
  glRasterPos2f((x+i)*4, y*8);
  glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (char)str[i]);
  i++;
}
glPopMatrix();
}

//очистка экрана
void debug_clear()
{
glClearColor(1,1,1,1);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//завершение работы с подсистемой ввода/вывода консоли
void debug_endwin()
{
}

//подсистема ввода для консоли
int debug_getstr_xy(int x, int y, char *str)
{static int first_flag; static int n; int m;
if (global_key==13) {first_flag=0; m=n; n=0; return m;}
if (first_flag==0) {first_flag=1; n=0;}
if (global_key>=32) {
  str[n]=(char)global_key; str[n+1]='\0'; n++; 
  debug_msgxy(x,y,str);
}
return 0;
}

//вывод общей статистики поведения нейросети
void nweb_report(struct NWEB *nweb)
{int i,nrns,nweb_id,x,y; char str[100];
nweb_id=nweb->ID;
nrns=0;
for(i=0;i<nweb->NRN_NUM;i++)
  if(nweb->nrn[i]!=NULL) if(nweb->nrn[i]->ID!= -1) nrns++;
sprintf(str,"nweb_id=%d, neurons=%d",nweb_id,nrns); debug_msgxy(1,2,str);
for(i=0;i<20;i++) {
 sprintf(str,"%d %4.2f",i,(float)commutator_get(nweb->comm,i));
 debug_msgxy(1,i+3,str);
}
for(i=100;i<200;i++) {
  sprintf(str,"%d %4.2f",i,(float)commutator_get(nweb->comm,i));
  debug_msgxy(20+11*((i-100)/20),3+i%20,str);
}
//вывод кусочка видеопамяти на случай отказа OpenGL
/*
printf("\n================================\n");
for(y=19;y>=0;y--) {
printf("*");
for(x=0;x<70;x++) printf("%c",32+(char)(commutator_get(nweb->comm,10000+y*320+x)));
  printf("*\n");
  }
printf("================================\n");
*/
//for(i=5256;i<5500;i++)
//  printf("%d--%f, ",i,commutator_get(nweb->comm,i));
//printf("\n================================\n");
}
