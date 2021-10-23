/*
 * =====================================================================================
 *
 *       Filename:  stat.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/10/21 06:08:45 PM IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  PARTHAJIT ROY (PR), roy.parthajit@gmail.com
 *   Organization:  The University of Burdwan
 *
 * =====================================================================================
 */

double mean(double* data, int size)
{
      double m = 0.0;
      for(int i=0; i<size; ++i){
	    m += data[i];
      }
      return m / (double)size;
}

double stdev(double* data, int size, double mean)
{
      double diff = 0.0;
      double tmp;
      for(int i=0; i<size; ++i){
	    tmp = (data[i] - mean);
	    tmp = tmp * tmp;
	    diff += tmp;
      }
      double var = diff / (double) (size - 1);
      double sd = sqrt(var);
      return sd;
}

