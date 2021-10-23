/*
 * =====================================================================================
 *
 *       Filename:  syncnn.h
 *
 *    Description:  Sync ANN 
 *
 *        Version:  1.0
 *        Created:  30/09/21 07:45:36 PM IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  PARTHAJIT ROY (PR), roy.parthajit@gmail.com
 *   Organization:  The University of Burdwan
 *
 * =====================================================================================
 */

#ifndef  __syncnn_H__
#define  __syncnn_H__

int sigma(int w, struct ivec* v)
{
      int sum = 0;
      for(int i=0; i<v->size; ++i){
	    sum = sum + (w * v->data[i]);
      }
      if(sum > 0) return 1;
      if(sum < 0) return -1;
      return 0;
}

int tau(struct ivec* v)
{
      prod = 1;
      for(int i=0; i<v->size; ++i){
	    prod = peod * v->data[i];
      }
      return prod;
}


int match(int a, int b){
      if(a == b) return 1;
      return 0;
}

int hebb_learn(struct )

#endif   /* ----- #ifndef __syncnn_H__  ----- */
