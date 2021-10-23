/*
 * =====================================================================================
 *
 *       Filename:  tpm.h
 *
 *    Description: Tree parity machine 
 *
 *        Version:  1.0
 *        Created:  02/10/21 03:15:55 PM IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  PARTHAJIT ROY (PR), roy.parthajit@gmail.com
 *   Organization:  The University of Burdwan
 *
 * =====================================================================================
 */


#ifndef  __tpm_H__
#define  __tpm_H__
#include "ivec.h"
#include "percept.h"

#define TRUE (1)
#define FALSE (0)

struct tpm{
      int K;
      int N;
      int upper;
      int lower;
      struct percept* percepts;
      int tau;
      int (*learning_rule)(int, struct ivec);
};
void tpm_fprint(struct tpm* tpm);

void tpm_gsync(struct tpm* tpms, int gsize, struct tpm* evetpm);

void tpm_sync(struct tpm* tpmA, struct tpm* tpmB);

void tpm_create(struct tpm* tpm, int numhidden, int numinput, int dn, int up);


void tpm_free(struct tpm* tpm);


#endif   /* ----- #ifndef __tpm_H__  ----- */
