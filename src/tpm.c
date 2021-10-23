/*
 * =====================================================================================
 *
 *       Filename:  tpm.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/10/21 10:17:57 AM IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  PARTHAJIT ROY (PR), roy.parthajit@gmail.com
 *   Organization:  The University of Burdwan
 *
 * =====================================================================================
 */


#include "tpm.h"

static void tpm_makeinput(struct ivec v)
{
      for(int i=0; i<v.size; ++i){
	    if(v.data[i] % 2 == 0){
		  v.data[i] = 1;
	    }else{
		  v.data[i] = -1;
	    }
      }
}

static void tpm_makewt(struct ivec v, int L, int U)
{
      int mod = U - L + 1;
      for(int i=0; i<v.size; ++i){
	    v.data[i] = v.data[i] % mod;
	    v.data[i] = v.data[i] + L;
      }
}
static void tpm_parity(struct tpm* tpm, struct ivec* inpvecs)
{
      int parity = 1;
      for(int i=0; i<tpm->K; ++i){
	    percept_sigma(tpm->percepts + i, inpvecs[i]);
	    parity *= tpm->percepts[i].sigma;
      }
      tpm->tau = parity;
}

static void tpm_learn(struct tpm* tpm, struct ivec* inpvecs)
{
      for(int i=0; i<tpm->K; ++i){
//	    if(tpm->percepts[i].sigma != tpm->tau){
		  percept_update(tpm->percepts + i, inpvecs[i], tpm->tau);
//	    }
      }
}

static int gsynchronized(struct tpm* tpms, int gsize)
{
      for(int i=0; i<tpms[0].K; ++i){
	    for(int j=0; j<tpms[0].N; ++j){
		  for(int k=1; k<gsize; ++k){
			if(tpms[0].percepts[i].W.data[j] != tpms[k].percepts[i].W.data[j]){
			      return FALSE;
			}
		  }
	    }
      }
      return TRUE;
}
static int synchronized(struct tpm* tpmA, struct tpm* tpmB)
{
      for(int i=0; i<tpmA->K; ++i){
	    for(int j=0; j<tpmA->N; ++j){
		  if(tpmA->percepts[i].W.data[j] != tpmB->percepts[i].W.data[j]){
			return FALSE;
		  }
	    }
      }
      return TRUE;
}
void tpm_fprint(struct tpm* tpm)
{
      for(int i=0; i<tpm->K; ++i){
	    printf("(");
	    for(int j=0; j<tpm->N; ++j){
		  printf("%d, ",tpm->percepts[i].W.data[j] );
	    }
	    printf(") -> ");
	    printf("[%d],  ", tpm->percepts[i].sigma);
      }
      printf("\n");
      printf("tau = %d\n", tpm->tau);
}
void tpm_gsync(struct tpm* tpms, int gsize, struct tpm* evetpm)
{

      struct ivec* inpvec;

      
      inpvec	= malloc ( tpms[0].K * sizeof(struct ivec) );
      if ( inpvec==NULL ) {
	    fprintf ( stderr, "\ndynamic memory allocation failed in function %s()\n" , __func__);
	    exit (EXIT_FAILURE);
      }


      for(int i=0; i<tpms->K; ++i){
	    ivec_create(inpvec+i, tpms[0].N);
	    tpm_makeinput(inpvec[i]);
      }

      int iter = 0;
      while( gsynchronized(tpms, gsize) == FALSE ){
//	    printf("\n\n\n\n\ninpue vectore iter %d\n", iter);
	    for(int i=0; i<tpms[0].K; ++i){
		  ivec_rnd(inpvec[i]);
		  tpm_makeinput(inpvec[i]);
//		  ivec_print(inpvec[i]);
//		  printf(", ");
	    }
//	    printf("\n");

	    for(int i=0; i<gsize; ++i){
		  tpm_parity(tpms+i, inpvec);
	    }
	    tpm_parity(evetpm, inpvec);
//	    printf("After parity of iter %d\n", iter);
//	    tpm_fprint(tpmA);
//	    tpm_fprint(tpmB);

	    int flag = 1;
	    for(int i=1; i<gsize; ++i){
		  if( tpms[0].tau != tpms[i].tau){
			flag = 0;
			break;
		  }
	    }
	    if(flag == 1){
		  for(int i=0; i<gsize; ++i){
			tpm_learn(tpms + i, inpvec);
		  }
		  tpm_learn(evetpm, inpvec);
	    }
//	    tpm_fprint(tpmA);
//	    tpm_fprint(tpmB);
//	    printf("After learn of iter %d\n", iter);
	    
	    iter ++;
      }
      if( iter > 100000 ){    /* Exception Handling */ 
	    fprintf(stderr, "Error in function %s()... maxiter reached before sync.\n", __func__);
	    exit(EXIT_FAILURE);
      }
      for(int i=0; i<gsize; ++i){
	    tpm_fprint(tpms + i);
      }
      printf("Eve=\n");
      tpm_fprint(evetpm);
      printf("Total iter= %d\n", iter);
}
void tpm_sync(struct tpm* tpmA, struct tpm* tpmB)
{

      if( ( tpmA->K != tpmB->K ) || (tpmA->N != tpmB->N )){    /* Exception Handling */ 
	    fprintf(stderr, "Error in function %s()... architecture disagreement encountered.\n", __func__);
	    exit(EXIT_FAILURE);
      } 
      struct ivec* inpvec;

      inpvec	= malloc ( tpmA->K * sizeof(struct ivec) );
      if ( inpvec==NULL ) {
	    fprintf ( stderr, "\ndynamic memory allocation failed in function %s()\n" , __func__);
	    exit (EXIT_FAILURE);
      }


      for(int i=0; i<tpmA->K; ++i){
	    ivec_create(inpvec+i, tpmA->N);
	    tpm_makeinput(inpvec[i]);
      }

      int iter = 0;
      while( synchronized(tpmA, tpmB) == FALSE ){
//	    printf("\n\n\n\n\ninpue vectore iter %d\n", iter);
	    for(int i=0; i<tpmA->K; ++i){
		  ivec_rnd(inpvec[i]);
		  tpm_makeinput(inpvec[i]);
//		  ivec_print(inpvec[i]);
//		  printf(", ");
	    }
//	    printf("\n");

	    tpm_parity(tpmA, inpvec);
	    tpm_parity(tpmB, inpvec);
//	    printf("After parity of iter %d\n", iter);
//	    tpm_fprint(tpmA);
//	    tpm_fprint(tpmB);
	    if(tpmA->tau == tpmB->tau){
		  tpm_learn(tpmA, inpvec);
		  tpm_learn(tpmB, inpvec);
//	    tpm_fprint(tpmA);
//	    tpm_fprint(tpmB);
//	    printf("After learn of iter %d\n", iter);
	    }
	    iter ++;
	    if( iter > 100000 ){    /* Exception Handling */ 
		  fprintf(stderr, "Error in function %s()... maxiter reached before sync.\n", __func__);
		  exit(EXIT_FAILURE);
	    }
      }
      tpm_fprint(tpmA);
      tpm_fprint(tpmB);
      printf("Total iter= %d\n", iter);
}
void tpm_create(struct tpm* tpm, int numhidden, int numinput, int dn, int up)
{
      tpm->K = numhidden;
      tpm->N = numinput;

      tpm->percepts = (struct percept*) malloc ( numhidden * sizeof(struct percept) );
      if ( tpm->percepts==NULL ) {
	    fprintf ( stderr, "\ndynamic memory allocation failed in function %s()\n" , __func__);
	    exit (EXIT_FAILURE);
      }
      
      for (int i=0; i<tpm->K; ++i) {
	    percept_create(tpm->percepts + i, numinput, dn, up);
      }
}

void tpm_free(struct tpm* tpm)
{
      for(int i=0; i<tpm->N; ++i){
	    percept_free(tpm->percepts);
      }
      free ( tpm->percepts );
      tpm->percepts	= NULL;

      free ( tpm->percepts );
      tpm->percepts	= NULL;
}

