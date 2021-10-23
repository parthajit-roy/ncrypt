/*
 * =====================================================================================
 *
 *       Filename:  index.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/10/21 09:47:37 AM IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  PARTHAJIT ROY (PR), roy.parthajit@gmail.com
 *   Organization:  The University of Burdwan
 *
 * =====================================================================================
 */

#include "index.h"


double jaccard(struct ivec u, struct ivec v)
{
      if( u.size != v.size ){    /* Exception Handling */ 
	    fprintf(stderr, "Error in function %s()... vector dimension mismatch.\n", __func__);
	    exit(EXIT_FAILURE);
      }
      int common = 0;
      for(int i=0; i<u.size; ++i){
	    if(u.data[i] == v.data[i]){
		  common ++;
	    }
      }
      double n = (double)common;
      double d = (double)(u.size + v.size);
      double jacc = n / (d-n);
      return jacc;
}


