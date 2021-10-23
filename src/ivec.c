/*
 * =====================================================================================
 *
 *       Filename:  ivec.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/10/21 10:30:51 AM IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  PARTHAJIT ROY (PR), roy.parthajit@gmail.com
 *   Organization:  The University of Burdwan
 *
 * =====================================================================================
 */

#include "ivec.h"



void ivec_print(struct ivec v)
{
      printf("(");
      for(int i=0; i<v.size; ++i){
	    printf("%d ", v.data[i]);
      }
      printf(")");
}
void ivec_rnd(struct ivec v){
//      srand(time(0));
      for(int i=0; i<v.size; ++i){
	    v.data[i] = rand();
//	    printf("Trace=%d ",v.data[i]);
      }
}
void ivec_create(struct ivec* v, int size)
{
      v->size = size;
      v->maxsize = size;
      v->data	= (int*) malloc ( size * sizeof(int) );
      if ( v->data == NULL ) {
	    fprintf ( stderr, "\ndynamic memory allocation failed in function %s()\n" , __func__);
	    exit (EXIT_FAILURE);
      }
}
int ivec_dot(struct ivec u, struct ivec v){
      if( u.size != v.size ){    /* Exception Handling */ 
	    fprintf(stderr, "Error in function %s()... dimension mismatch.\n", __func__);
	    exit(EXIT_FAILURE);
      }
      int dot = 0;
      for(int i=0; i<u.size; ++i){
	    int x =  u.data[i] * v.data[i];
	    dot += x;
      }
      return dot;
}

void ivec_free(struct ivec* v)
{

      free ( v->data );
      v->data = NULL;
}


