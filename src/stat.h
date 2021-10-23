/*
 * =====================================================================================
 *
 *       Filename:  stat.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/10/21 06:00:59 PM IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  PARTHAJIT ROY (PR), roy.parthajit@gmail.com
 *   Organization:  The University of Burdwan
 *
 * =====================================================================================
 */


#ifndef  __stat_H__
#define  __stat_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
double mean(double* data, int size);

double stdev(double* data, int size, double mean);

#endif   /* ----- #ifndef __stat_H__  ----- */
