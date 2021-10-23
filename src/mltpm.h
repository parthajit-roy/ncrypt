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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ivec.h"
#include "percept.h"
#include "index.h"
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
      int tau;
      struct ivec opvec;
};

struct result{
      FILE* fp;
      int archi[100];
      int layer;
      int L;
      int gsize;
      int numintru;
      double avgjaccard;
      double sdjaccard;
      int iter;
      double time;
      int numsuccess;
      double* stat;
      int statsize;
};

void result_printhead(FILE* fp);
void result_fprint(FILE* fp, struct result* result);

void mltpm_create(struct mltpm* mltpm, int* archi,  int numlayer, int L);



//void mltpm_gsync(struct mltpm* mltpms, int gsize, struct mltpm* evemltpm);
void mltpm_gsync(struct mltpm* mltpms, int gsize, struct mltpm* evemltpms, int intru, struct result* result);

void mltpm_free(struct mltpm* mltpm);
#endif   /* ----- #ifndef __mltpm_H__  ----- */
