/*
 * =====================================================================================
 *
 *       Filename:  percept.c
 *
 *    l 
 *
 *        Version:  1.0
 *        Created:  04/10/21 10:36:45 AM IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  PARTHAJIT ROY (PR), roy.parthajit@gmail.com
 *   Organization:  The University of Burdwan
 *
 * =====================================================================================
 */

#include "percept.h"

void percept_makewt(struct ivec v, int L, int U)                                             
{                                                                                                      
      int mod = U - L + 1;                                                                         
      for(int i=0; i<v.size; ++i){                                                                                    
            v.data[i] = v.data[i] % mod;
            v.data[i] = v.data[i] + L;                                                                                      
      }                                                                                                                     
} 


void percept_makeinput(struct ivec v)
{
      for(int i=0; i<v.size; ++i){
	    if(v.data[i] % 2 == 0){
		  v.data[i] = 1;
	    }else{
		  v.data[i] = -1;
	    }
      }
}
  
void percept_create(struct percept* p, int inputsize, int dn, int up)
{
      ivec_create(&p->W, inputsize);
      p->up = up;
      p->dn = dn;
      ivec_rnd(p->W);
      percept_makewt(p->W, p->dn, p->up);
      /*for(int i=0; i<p->W.size; ++i){
	    printf("\n%d", p->W.data[i]);
      }*/
      
}

void percept_update(struct percept* p, struct ivec inpvec, int tau)
{
      for(int i=0; i<p->W.size; ++i){
	    if(p->sigma * tau > 0){
		  int tmp = p->W.data[i] + (inpvec.data[i] );
		  if(tmp < p->dn){
			p->W.data[i] = p->dn;
		  }else if(tmp > p->up){
			p->W.data[i] = p->up;
		  }else{
			p->W.data[i] = tmp;
		  }
	    }
      }
}
            

void percept_sigma(struct percept* p, struct ivec inpvec)
{
      int val = ivec_dot(p->W, inpvec);
//      printf("Tra sig=%d \n", val);
      p->sigma = (val <= 0) ? -1 : +1;
}
void percept_free(struct percept* p)
{
      ivec_free(&p->W);
}

/*void percept_setwt(struct percept* p, int wt)
{
      p->weight = wt;
}

void percept_learn(struct percept* p, int (*learn)(int, int*))
{
      int newwt = learn(p->weight, vectora);

}*/

