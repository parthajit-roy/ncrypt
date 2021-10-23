/*
 * =====================================================================================
 *
 *       Filename:  ivec.h
 *
 *    Description:  Integer vector 
 *
 *        Version:  1.0
 *        Created:  02/10/21 03:03:44 PM IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  PARTHAJIT ROY (PR), roy.parthajit@gmail.com
 *   Organization:  The University of Burdwan
 *
 * =====================================================================================
 */



#ifndef  __ivec_H__
#define  __ivec_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


struct ivec{
      int* data;
      int size;
      int maxsize;
};

void ivec_print(struct ivec v);
void ivec_rnd(struct ivec v);
void ivec_create(struct ivec* v, int size);
int ivec_dot(struct ivec u, struct ivec v);

void ivec_free(struct ivec* v);


 
#endif   /* ----- #ifndef __ivec_H__  ----- */
