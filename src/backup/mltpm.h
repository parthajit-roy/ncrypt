/*
 * =====================================================================================
 *
 *       Filename:  mltpm.h
 *
 *    Descriptionl 
 *
 *        Version:  1.0
 *        Created:  04/10/21 10:28:13 AM IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  PARTHAJIT ROY (PR), roy.parthajit@gmail.com
 *   Organization:  The University of Burdwan
 *
 * =====================================================================================
 */


#ifndef  __mltpm_H__
#define  __mltpm_H__

#define TRUE (1)
#define FALSE (0)

#define NUL_LAYER (3)

struct layer{
      int K;// no. of neurons
      int N; // input size
      int L;// range. Follow Adi Samir's paper.
      struct percept* neurons;

};
struct mltpm{
      int numlayer;
      int archi[50];
      struct layer* layers;
};

void mltpm_create(struct mltpm* mltpm, int* archi,  int numlayer, int L)
{
      mltpm->numlayer = numlayer;
      for(int i=0; i<numlayer + 1; ++i){
	    mltpm->archi[i] = archi[i];
      }
      mltpm->layers = (struct layer*) malloc (numlayer * sizeof (struct layer));
      for(int i=0; i<numlayer; ++i){
	    ; //layer_create(mltpm->layers +i, archi + i, archi + i + 1, L);
      }
}
#endif   /* ----- #ifndef __mltpm_H__  ----- */
