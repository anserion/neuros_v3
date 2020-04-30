//drv.c
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include "neuron.h"
#include "commutator.h"
#include "nweb.h"
#include "drv.h"
#include "debug.h"
#include "defs.h"

extern int global_key;
extern int global_x;
extern int global_y;
extern int global_btn;

int drv_dummy(struct NWEB *nweb)
{
return 0;
}

int drv_kbd(struct NWEB *nweb)
{char value;
value=(char)global_key;
if (value!=0) {
 commutator_set(nweb->comm,0,value);
}
return value;
}

int drv_load(struct NWEB *nweb)
{char filename[100],str[50];
  FILE *src; int fname_dec;
  int i,id,n,k,m,base,on,addr0,f,alg,addr; float lambda0;
  int *x_table, *a_table; double *l_table;

  fname_dec=(int)commutator_get(nweb->comm,0);
  sprintf(filename,"%d.neu",fname_dec);
  src=fopen(filename,"r");
  if (src==NULL) {
    sprintf(str,"drv_load: error loading %s",filename); debug_msgxy(40,23,str);
    }
  else {
    sprintf(str,"drv_load: load %s",filename); debug_msgxy(0,0,str);
    while(feof(src)==0){
      fscanf(src,"%s",str); id=atoi(str);
      if(feof(src)!=0) break;
      fscanf(src,"%s",str); base=atoi(str);
      fscanf(src,"%s",str); on=atoi(str);
      fscanf(src,"%s",str); lambda0=atof(str);
      fscanf(src,"%s",str); addr0=atoi(str);

      fscanf(src,"%s",str); f=atoi(str);
      fscanf(src,"%s",str); alg=atoi(str);
      fscanf(src,"%s",str); addr=atoi(str);

      fscanf(src,"%s",str); n=atoi(str);
      fscanf(src,"%s",str); k=atoi(str);
      fscanf(src,"%s",str); m=atoi(str);

      x_table=(int *)malloc((n+1)*sizeof(int));
      l_table=(double *)malloc((k+1)*sizeof(double));
      a_table=(int *)malloc((m+1)*sizeof(int));

      x_table[0]=on; l_table[0]=lambda0; a_table[0]=addr0;
      for(i=1;i<=n;i++) {fscanf(src,"%s",str); x_table[i]=atoi(str);}
      for(i=1;i<=k;i++) {fscanf(src,"%s",str); l_table[i]=atof(str);}
      for(i=1;i<=m;i++) {fscanf(src,"%s",str); a_table[i]=atoi(str);}

      if (id>=10) {
      neuron_destroy(id,nweb);
      neuron_create(id,base,f,alg,addr,n,k,m,x_table,l_table,a_table,nweb);
      }
      free(x_table); free(l_table); free(a_table);
    }
    fclose(src);
  }
  return 0;
}

int drv_save(struct NWEB *nweb)
{char filename[100]; char str[50];
  FILE *dst;
  int fname_dec,id,n,k,m,i,j,n_f,n_alg,n_addr;
  struct NEURON *nrn;

  fname_dec=(int)commutator_get(nweb->comm,0);
  if (fname_dec==0) return;
  sprintf(filename,"%d.neu",fname_dec);
  dst=fopen(filename,"w");
  if(dst==NULL) {
     sprintf(str,"drv_save: error saving %s",filename); debug_msgxy(20,23,str);
     }
  else {
    sprintf(str,"drv_save: save %s",filename); debug_msgxy(20,23,str);
    for(i=10;i<nweb->NRN_NUM;i++)
      if(nweb->nrn[i]!=NULL) {
        nrn=nweb->nrn[i];
	if (nrn->ID>=0) {
        fprintf(dst,"%d %d ",nrn->ID, nrn->Base);
	fprintf(dst," %d %f %d ",nrn->X_table[0],nrn->L[0],nrn->A_table[0]);
        fprintf(dst," %d %d %d ", nrn->n_f, nrn->n_alg, nrn->n_addr);
        fprintf(dst," %d %d %d  ", nrn->N, nrn->K, nrn->M);
        for(j=1;j<=nrn->N;j++) fprintf(dst,"%d ",nrn->X_table[j]);
        fprintf(dst,"  ");
        for(j=1;j<=nrn->K;j++) fprintf(dst,"%f ",nrn->L[j]);
        fprintf(dst,"  ");
        for(j=1;j<=nrn->M;j++) fprintf(dst,"%d ",nrn->A_table[j]);
        fprintf(dst,"\n");
	}
    }
    fclose(dst);
  }
  return 0;
}

int drv_display(struct NWEB *nweb)
{double c; int x_res,y_res,cell,v_ram_start,v_ram_end,x,y;
 struct COMMUTATOR *comm;
  comm=nweb->comm;
  v_ram_start=commutator_get(comm,1000);
  x_res=commutator_get(comm,1001);
  y_res=commutator_get(comm,1002);
  v_ram_end=v_ram_start+x_res*y_res;
  x=0; y=0;
  glBegin(GL_POINTS);
  for(cell=v_ram_start;cell<v_ram_end;cell++) {
    c=commutator_get(comm,cell);
    glVertex2f(x,y); glColor3f(0,c,0);
    x++; if (x>=x_res) {x=0; y++;}
  }
  glEnd();
return 0;
}

int drv_mouse(struct NWEB *nweb)
{struct COMMUTATOR *comm;
  comm=nweb->comm;
  commutator_set(comm,1,global_x);
  commutator_set(comm,2,global_y);
  commutator_set(comm,11,global_btn);
return 0;
}
