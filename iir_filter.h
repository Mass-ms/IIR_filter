#ifndef IIR_FILTER_H
#define IIR_FILTER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LPF 0
#define HPF 1
#define BPF 2
#define BEF 3
#define Reso 4
#define Notch 5
#define LowShelf 6
#define HighShelf 7
#define Peak 8

//フィルタの設定
void IIR_setup(int filterType, int fs, double *fc, double *fc1, double *fc2, double *Q, double *g);

//フィルタリング
void IIR_filtering(double x[], double y[],int L, double a[], double b[], int I, int J);

//以下フィルタ係数の計算関数

//ローパスフィルタ
void IIR_LPF(double fc, double Q, double a[], double b[]);

//ハイパスフィルタ
void IIR_HPF(double fc, double Q, double a[], double b[]);

//バンドパスフィルタ
void IIR_BPF(double fc1, double fc2, double a[], double b[]);

//バンドストップフィルタ
void IIR_BEF(double fc1, double fc2, double a[], double b[]);

//共振フィルタ
void IIR_resonator(double fc, double Q, double a[], double b[]);

//ノッチフィルタ
void IIR_notch(double fc, double Q, double a[], double b[]);

//ローシェルビングフィルタ
void IIR_low_shelving(double fc, double Q, double g, double a[], double b[]);

//ハイシェルビングフィルター
void IIR_high_shelving(double fc, double Q, double g, double a[], double b[]);

//ピーキングフィルタ
void IIR_peaking(double fc, double Q, double g, double a[], double b[]);

#endif