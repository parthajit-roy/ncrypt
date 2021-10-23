/*
 * =====================================================================================
 *
 *       Filename:  percept.h
 *
 *    Description: Single layer percept. 
 *
 *        Version:  1.0
 *        Created:  02/10/21 03:11:13 PM IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  PARTHAJIT ROY (PR), roy.parthajit@gmail.com
 *   Organization:  The University of Burdwan
 *
 * =====================================================================================
 */

#ifndef  __percept_H__
#define  __percept_H__

#include "ivec.h"

struct percept{
      struct ivec W;
      int sigma;
      int up;
      int dn;
};


void percept_makewt(struct ivec v, int L, int U);  
void percept_makeinput(struct ivec v);                                          
void percept_create(struct percept* p, int inputsize, int dn, int up);

void percept_update(struct percept* p, struct ivec inpvec, int tau);
            

void percept_sigma(struct percept* p, struct ivec inpvec);
void percept_free(struct percept* p);

#endif   /* ----- #ifndef __percept_H__  ----- */
