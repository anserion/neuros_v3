//kernel.c
#include <GL/gl.h>
#include <GL/glu.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>

#include <stdlib.h>
#include "defs.h"
#include "neuron.h"
#include "commutator.h"
#include "nweb.h"
#include "sweb.h"
#include "drv.h"
#include "debug.h"

GLsizei windH = 480, windW = 640;
struct SWEB *super_network;
int current_nweb;
int global_key, global_x, global_y, global_btn;
int no_clear_flag;

void draw_display()
{
  glutSwapBuffers();
if (no_clear_flag==0) {
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
glFinish();
}

void mouse_func(int btn, int state, int x, int y)
{
global_x=x/2; global_y=(windH-y)/2;
global_btn=btn+1;
}

void mytimer(int value)
{static int busy_flag;
if (no_clear_flag==0) {
if (busy_flag==0) {
//������������ ���������-���������
busy_flag=1;
commutator_imd(super_network->nweb[0]->comm,13,1);
neuron_tick(super_network->nweb[0]->nrn[3]);
busy_flag=0;
}
}
//����� �������� ��������� ���������
nweb_report(super_network->nweb[current_nweb]);
//����� ��������� �������
draw_display();
glutTimerFunc(value,mytimer,value);
}

void keyboard (unsigned char key, int x, int y)
{struct NWEB *nweb;
  static char busy_flag;
  static int console_start;
  static char s_port[100]; int port;
  static int port_ok;
  static char s_value[100]; float value;
  static int value_ok;

    global_key=key; //������������� ��� ������� ������� ��������� ����
    
    //������������� ���� � ������� ��� ������� ESC
    if (key==27) console_start=1;
    
    if ((console_start!=0)&&(port_ok==0)) {
       debug_msgxy(0,0,"port# ");
       port_ok=debug_getstr_xy(6,0,s_port);
    }
    if (port_ok!=0) {
       debug_msgxy(0,1,"value# ");
       value_ok=debug_getstr_xy(7,1,s_value);
    }

  //���� ������ � ���������� ���� ������� �������,
  //�� ��������� ����� ����� � ��������, ������� ����� � ���� ��������
  if ((value_ok!=0)&&(port_ok!=0)) {
   port=atoi(s_port); port_ok=0;
   value=atof(s_value); value_ok=0;
   console_start=0;
  } else port= -1;
  
  no_clear_flag=console_start;
  
    //��������� ������ ��������� ��� ���������� ������
    if ((port==10)&&((int)value>=0)&&((int)value<SWEB_MaxN))
      if (super_network->nweb[(int)value]!=NULL) {
         current_nweb=(int)value;   //������������� ����� ����� ������� ���������
	 }
    //������������� ��������� ��� ���������� ������
    nweb=super_network->nweb[current_nweb]; 

  //������������� ��������� ������ �������
  if (port==8) {
      switch ((int)value) {
      case CONSOLE_DUMMY: break; //������� ����� ���������������
      //����� �������� ��������� ���������
      case CONSOLE_STATUS: nweb_report(nweb); break;
      //��������� �����
      case CONSOLE_EXIT: {sweb_destroy(super_network); debug_endwin(); exit(0);}
      }
  }

  //����������� ��������� ������ �������
  if (busy_flag==0) {
    //�������� ����������
    busy_flag=1;
    //����������� ����������� ����������� ���������, ����� ����� 10
    if ((port>=0)&&(port<nweb->comm->COMM_N)&&(port!=10))
       commutator_imd(nweb->comm,port,value);
    //���� ������� ������ �� �������, �� ������������ ���������
    if ((port==13)&&(value!=0)) neuron_tick(nweb->nrn[3]);
    //��������� ����������
    busy_flag=0;
  }
}

void neuros_init( void)
{int i;
struct NWEB *nweb; struct SWEB *sweb;

debug_init();

  //������� ��������� ������� �� 1000 ����������
  sweb=sweb_create(1000); super_network=sweb;
  //������� ��������� ���������� �������� ���� - ��������� 0
  //� ��� ����� ���������� ���������� ����/������
  nweb_create(0,1000,1000,sweb); nweb=sweb->nweb[0];
  //���������� � ��������� ������� ����� ��������
  nweb_clear(nweb); 
  //�������� �������������� �������� ���������� �� ����� 0.neu
  commutator_imd(nweb->comm,0,0); commutator_imd(nweb->comm,14,1);
  neuron_tick(nweb->nrn[4]);
  commutator_imd(nweb->comm,14,0);
  //������� ������ ������ - ��������� 1
  nweb_create(1,100000,100000,sweb); nweb=sweb->nweb[1];
  nweb_clear(nweb);
  commutator_imd(nweb->comm,0,1); commutator_imd(nweb->comm,14,1);
  neuron_tick(nweb->nrn[4]);
  commutator_imd(nweb->comm,0,0); commutator_imd(nweb->comm,14,0);
  //������� ���������� �����/������ - ��������� 2
  nweb_create(2,100000,100000,sweb); nweb=sweb->nweb[2];
  nweb_clear(nweb);
  commutator_imd(nweb->comm,0,2); commutator_imd(nweb->comm,14,1);
  neuron_tick(nweb->nrn[4]);
  commutator_imd(nweb->comm,0,0); commutator_imd(nweb->comm,14,0);
  //������� ������������ ���������� - ��������� 3
  nweb_create(3,100000,100000,sweb); nweb=sweb->nweb[3];
  nweb_clear(nweb);
  commutator_imd(nweb->comm,0,3); commutator_imd(nweb->comm,14,1);
  neuron_tick(nweb->nrn[4]);
  commutator_imd(nweb->comm,0,0); commutator_imd(nweb->comm,14,0);

  //������� �������� ���� - ��������� 4
  nweb_create(4,100000,100000,sweb); nweb=sweb->nweb[4];
  nweb_clear(nweb);

  //������������� ����� ������� ��������� ��� ���������� ������
  current_nweb=0;
}

int main (int argc, char **argv)
{int freq;

glutInit(&argc,argv);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
glutInitWindowSize(windW,windH);
glutCreateWindow("neuros");

glutMouseFunc(mouse_func);
glutKeyboardFunc(keyboard);
glutDisplayFunc(draw_display);

glViewport(0,0,windW,windH);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0.0,320,0.0,240);
glPointSize(2);
glMatrixMode(GL_MODELVIEW);

if (argc<2) {
      printf("usage: kernel freq\n");
      return 1;
  }
freq=atoi(argv[1]);
if (freq==0) return 0;
glutTimerFunc(freq,mytimer,freq);

neuros_init();

glutMainLoop();
return 0;
}
