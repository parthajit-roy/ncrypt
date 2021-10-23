/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  main 
 *
 *        Version:  1.0
 *        Created:  02/10/21 07:10:29 PM IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  PARTHAJIT ROY (PR), roy.parthajit@gmail.com
 *   Organization:  The University of Burdwan
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "ivec.h"
#include "tpm.h"
#include "mltpm.h"
#include "stat.h"

#define GRP_SIZE (1000)
void run()
{



      int gsize = GRP_SIZE;
      struct tpm tpms[GRP_SIZE];
      struct tpm evetpm;
      int K = 3;
      int N = 5;

      int dn = -8;
      int up = 8;

      srand(time(0));
//      tpm_create(&tpmA, K, N, dn, up);
//      tpm_create(&tpmB, K, N, dn, up);

      for(int i=0; i<gsize; ++i){
	    tpm_create(tpms+i, K, N, dn, up);
      }
      tpm_create(&evetpm, K, N, dn, up);
//      tpm_fprint(&tpmA);
//      tpm_fprint(&tpmB);

      tpm_gsync(tpms, gsize, &evetpm);

      for(int i=0; i<gsize; ++i){
	    tpm_free(tpms+i);
      }
      tpm_free(&evetpm);
//      tpm_sync(&tpmA, &tpmB);
      

}

void runmltp(int architecture[], int numlayer, int gsize, int L, int intru, struct result* result)
{


      struct mltpm mltpms[GRP_SIZE];
      struct mltpm* evemltpms = (struct mltpm*) malloc (intru * sizeof(struct mltpm));

//      tpm_create(&tpmA, K, N, dn, up);
//      tpm_create(&tpmB, K, N, dn, up);

      for(int i=0; i<gsize; ++i){
	    mltpm_create(mltpms+i, architecture, numlayer, L);
      }
      for(int i=0; i<intru; ++i){
	    mltpm_create(evemltpms + i, architecture, numlayer, L);
      }
      
      mltpm_gsync(mltpms, gsize, evemltpms, intru, result);

      for(int i=0; i<gsize; ++i){
	    mltpm_free(mltpms+i);
      }
      for(int i=0; i<intru; ++i){
	    mltpm_free(evemltpms + i);
      }
      free(evemltpms);
      

}

int main( int argc, char* argv[] )
{

      int layer = 1;
      int L = 5;
      int gsize = 5;
      int architecture[100];
      architecture[0] = 5;
      architecture[1] = 3; 
      int numintru = 1;
      int runsize = 10;

      struct result result;
      for(int i=1; i<argc; ++i){
	    if(strncmp(argv[i], "arch=", 5) == 0){
		  char archi[512];
		  layer = 0;
		  strcpy(archi, argv[1]+5);
		  char* token = strtok(archi, "-");
		  while(token != NULL){
			architecture[layer] = atoi(token);
			result.archi[layer] = architecture[layer];
			token = strtok(NULL, "-");
			layer ++;
		  }
		  layer --;
	    }else if(strncmp(argv[i], "L=", 2) == 0){
		  L = atoi(argv[i]+2);
	    }else if(strncmp(argv[i], "gsize=", 6) == 0){
		  gsize = atoi(argv[i]+6);
	    }else if(strncmp(argv[i], "intru=", 6) == 0){
		  numintru = atoi(argv[i] + 6);
	    }else if(strncmp(argv[i], "runsize=", 8) == 0){
		  runsize = atoi(argv[i]+8);
	    }else{
		  fprintf(stderr, "Error in function %s()... wrong argument.\n", __func__);
		  exit(EXIT_FAILURE);
	    }
      }
      for(int i=0; i<layer+1; ++i){
	    printf("%d-", architecture[i]);
      }
      result.layer = layer;
      result.L = L;
      result.numintru = numintru;
      result.gsize    = gsize;
      printf("%d,  layer=%d, gsize=%d, intru=%d ", L, layer, gsize, numintru);
      result.stat = (double*) malloc (numintru * sizeof(double));
      result.statsize = numintru;
      
      FILE	*fp;										/* output-file pointer */
      char	fp_file_name[512];		/* output-file name    */
      if(layer == 1){
	    sprintf(fp_file_name, "test-%d-%d-%d-%d-%d.txt",architecture[0], architecture[1], L, gsize, numintru);
      }else if(layer == 2){
	    sprintf(fp_file_name, "test-%d-%d-%d-%d-%d-%d.txt",architecture[0], architecture[1], architecture[2], L, gsize, numintru);
      }else if(layer == 3){
	    sprintf(fp_file_name, "test-%d-%d-%d-%d-%d-%d-%d.txt",architecture[0], architecture[1], architecture[2], architecture[3], L, gsize, numintru);
      }else{
	    fprintf(stderr, "unalbe to write to file\n");
	    exit(EXIT_FAILURE);
      }
    
      fp	= fopen( fp_file_name, "w" );
      if ( fp == NULL ) {
	    fprintf ( stderr, "couldn't open file '%s'; %s\n",
			fp_file_name, strerror(errno) );
	    exit (EXIT_FAILURE);
 
     }
     
      result.fp = fp;
      printf("opfile= %s\n", fp_file_name);

      result_printhead(fp);
      srand(time(0));
      for(int i=0; i<runsize; ++i){
	    runmltp(architecture, layer, gsize, L, numintru, &result);
	    result.avgjaccard = mean(result.stat, result.statsize);
	    result.sdjaccard = stdev(result.stat, result.statsize, result.avgjaccard);

	    result_fprint(fp, &result);
      }
      free(result.stat);
      result.stat = NULL;
      if( fclose(fp) == EOF ) {			/* close output file   */
	    fprintf ( stderr, "couldn't close file '%s'; %s\n",
			fp_file_name, strerror(errno) );
	    exit (EXIT_FAILURE);
      }
//      srand(time(0));
//      for(int i=0; i<100; ++i){
//	    int k= rand();
//	    printf("%d ", k);
//      }
//      printf("\n");
      return 0;
}
