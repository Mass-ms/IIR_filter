#ifndef IIR_FILTER_H
#define IIR_FILTER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void IIR_filtering(double x[], double y[],int L, double a[], double b[], int I, int J);
void IIR_LPF(double fc, double Q, double a[], double b[]);
void IIR_resonator(double fc, double Q, double a[], double b[]);

#endif