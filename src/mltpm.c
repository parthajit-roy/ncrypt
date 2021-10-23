/*
 * =====================================================================================
 *
 *       Filename:  mltpm.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/10/21 04:12:23 PM IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  PARTHAJIT ROY (PR), roy.parthajit@gmail.com
 *   Organization:  The University of Burdwan
 *
 * =====================================================================================
 */



#include "mltpm.h"

void result_printhead(FILE* fp)
{
      fprintf(fp, "     N-H1-H2-H3... L  gsize  numintru iter time(s) jac-mean  jac-sd success%%\n");
}

void result_fprint(FILE* fp, struct result* result){
      fprintf(fp, "DATA ");
      for(int i=0; i<result->layer+1; ++i){
	    fprintf(fp, "%2d - ", result->archi[i]);
      }
      fprintf(fp, "%3d ", result->L);
      fprintf(fp, "%3d ", result->gsize);
      fprintf(fp, "%3d ", result->numintru);
      fprintf(fp, "%8d ", result->iter);
      fprintf(fp, "%8.4lf ", result->time);
      fprintf(fp, "%8.4lf ", result->avgjaccard);
      fprintf(fp, "%8.4lf ", result->sdjaccard);
      double successrate = (double)result->numsuccess / (double)result->numintru;
      fprintf(fp, "%8.4lf\n", successrate);
}

void layer_create(struct layer* layer, int N, int K, int L)
{
      layer->K = K;
      layer->N = N;
      layer->L = L;
      layer->neurons = (struct percept*) malloc ( K * sizeof(struct percept) );
      for(int i=0; i<K; ++i){
	    percept_create(layer->neurons + i, N, -L, +L);
      }
}

void layer_opvec(struct ivec* v, struct layer* layer)
{
      if( layer->K > v->maxsize ){    /* Exception Handling */ 
	    fprintf(stderr, "Error in function %s()... vector overflow encountered.\n", __func__);
	    exit(EXIT_FAILURE);
      }
      for(int i=0; i<layer->K; ++i){
	    v->data[i] = layer->neurons[i].sigma;
      }
      v->size = layer->K;
}

void layer_free(struct layer* layer)
{
      for(int i=0; i<layer->K; ++i){
	    percept_free(layer->neurons + i);
      }
      free(layer->neurons);
      layer->neurons = NULL;
}


void layer_wtvec(struct ivec* v, struct layer* layer)
{

      
      for(int i=0; i<layer->K; ++i){
	    for(int j=0; j<layer->N; ++j){
		  v->data[i*layer->K + j] = layer->neurons[i].W.data[j];
	    }
      }
}

void mltpm_create(struct mltpm* mltpm, int* archi,  int numlayer, int L)
{
      ivec_create(&mltpm->opvec, 500);
      mltpm->numlayer = numlayer;
      for(int i=0; i<numlayer + 1; ++i){
	    mltpm->archi[i] = archi[i];
      }
      mltpm->layers = (struct layer*) malloc (numlayer * sizeof (struct layer));
      for(int i=0; i<numlayer; ++i){

	    
	    layer_create(mltpm->layers +i, archi[i], archi[i+1], L);
      }
      
}



void mltpm_parity(struct mltpm* p, struct ivec* ivecs)
{
      int numlayer = p->numlayer;
      struct ivec* opvec = &p->opvec;
     
      struct layer* layer;
      layer = p->layers + 0;
      for(int i=0; i<layer->K; ++i){
	    percept_sigma(layer->neurons + i, ivecs[i]);
      }
      layer_opvec(opvec, layer);
      for(int i=1; i<numlayer; ++i){
	    layer = p->layers + i;
	    for(int j=0; j<layer->K; ++j){
		  percept_sigma(layer->neurons + j, *opvec);
	    }
	    layer_opvec(opvec, layer);
      }
      int tau = 1;
      for(int i=0; i<opvec->size; ++i){
	    tau = tau * opvec->data[i];
      }
      p->tau = tau;
}

static void mltpm_learn(struct mltpm* p, struct ivec* inpvecs)
{
      struct ivec* opvec = &p->opvec;
      int numlayer = p->numlayer;
      struct layer* layer = p->layers + 0;
      for(int j=0; j<layer->K; ++j){
	    percept_update(layer->neurons +j, inpvecs[j], p->tau);
      }
      layer_opvec(opvec, layer);
      for(int i=1; i<numlayer; ++i){
	    layer = p->layers + i;
	    for(int j=0; j<layer->K; ++j){
		  percept_update(layer->neurons +j, *opvec, p->tau);
	    }
	    layer_opvec(opvec, layer);
      }
}

int is_sync(struct mltpm* p1, struct mltpm* p2)
{
      int numlayer =1;// p1->numlayer;
      for(int i=0; i< numlayer; ++i){
	    struct layer* l1 = p1->layers + i;
	    struct layer* l2 = p2->layers + i;
	    for(int j=0; j<l1->K; ++j){
		  for(int k=0; k<l1->N; ++k){
			if(l1->neurons[j].W.data[k] != l2->neurons[j].W.data[k]){
			      return FALSE;
			}
		  }
	    }
      }
      return TRUE;
}

int is_gsync(struct mltpm* mltpms, int gsize)
{
      int flag = 1;
      for(int i=1; i<gsize; ++i){
	    flag = is_sync(mltpms + 0, mltpms + i);
	    if( flag == 0 ){
		  return FALSE;
	    }
      }
      return TRUE;
}

void mltpm_fprint(struct mltpm* p1)
{
      for(int i=0; i< p1->numlayer; ++i){
	    struct layer* l1 = p1->layers + i;
	    for(int j=0; j<l1->K; ++j){
		printf("(, ");
		  for(int k=0; k<l1->N; ++k){
			printf("%d, ", l1->neurons[j].W.data[k]);
		  }
			
		printf("), ");
	    }
		printf("\n");
      }
		printf("End\n");
}
void mltpm_gsync(struct mltpm* mltpms, int gsize, struct mltpm* evemltpms, int intru, struct result* result)
{
      struct ivec* inpvec;
      

      int K = mltpms[0].archi[1];
      int N = mltpms[0].archi[0];
      
      

      inpvec	= malloc ( K * sizeof(struct ivec) );
      if ( inpvec==NULL ) {
	    fprintf ( stderr, "\ndynamic memory allocation failed in function %s()\n" , __func__);
	    exit (EXIT_FAILURE);
      }


      struct ivec bobvec;
      struct ivec evevec;
      ivec_create(&bobvec, K * N);
      ivec_create(&evevec, K * N);

      
//      for(int i=0; i<gsize; ++i){
//	    mltpm_fprint(mltpms +i);
//      }
//      for(int i=0; i<intru; ++i){
//	    mltpm_fprint(evemltpms + i);
//      }
      for(int i=0; i<K; ++i){
	    ivec_create(inpvec+i, N);
	    ivec_rnd(inpvec[i]);
	    percept_makeinput(inpvec[i]);
      }

      
      int iter = 0;

      int success = 1;
      clock_t begin = clock();
      result->numsuccess = 0;
      while( is_gsync(mltpms, gsize) == FALSE ){

	    
	    for(int i=0; i<K; ++i){
		  ivec_rnd(inpvec[i]);
		  percept_makeinput(inpvec[i]);
	    }

	    for(int i=0; i<gsize; ++i){
		  mltpm_parity(mltpms+i, inpvec);
	    }
	    for(int i=0; i<intru; ++i){
		  mltpm_parity(evemltpms + i, inpvec);
	    }

	    int flag = 1;
	    for(int i=1; i<gsize; ++i){
		  if( mltpms[0].tau != mltpms[i].tau){
			flag = 0;
			break;
		  }
	    }
	    if(flag == 1){
		  for(int i=0; i<gsize; ++i){
			mltpm_learn(mltpms + i, inpvec);
		  }
		  
		  for(int i=0; i<intru; ++i){
			mltpm_learn(evemltpms + i, inpvec);
		  }
	    }
//	    tpm_fprint(tpmA);
//	    tpm_fprint(tpmB);
//	    printf("After learn of iter %d\n", iter);
	    
	    iter ++;
	    if( iter > 1000000 ){    /* Exception Handling */ 
		  fprintf(stderr, "Error in function %s()... maxiter reached before sync.\n", __func__);
		  success = 0;
		  break;
	    }
      }

      if(success == 0){
	    fprintf(result->fp, "UNSYNC iter=%d\n", iter - 1);
      }
      clock_t end = clock();
      result->time = (double)(end - begin) / (double)CLOCKS_PER_SEC;
//     for(int i=0; i<gsize; ++i){
//         mltpm_fprint(mltpms + i);
//     }
//      printf("Eve=\n");
//      mltpm_fprint(evemltpm);
      layer_wtvec(&bobvec, &mltpms[0].layers[0]);

      result->iter = iter;
      for(int i=0; i<intru; ++i){
	    layer_wtvec(&evevec, &evemltpms[i].layers[0]);
	    double jacc = jaccard(bobvec, evevec);
	    result->stat[i] = jacc;
	    if(jacc > 0.99) result->numsuccess ++;
//	    printf("jaccard=%lf\n", jacc);
      }
      printf("Total iter= %d\n", iter);
}

void mltpm_free(struct mltpm* mltpm)
{
      for(int i=0; i<mltpm->numlayer; ++i){
	    layer_free(mltpm->layers +i);
      }
      free (mltpm->layers);
      mltpm->layers = NULL;
}
