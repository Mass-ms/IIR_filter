#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "iir_filter.h"

int main(void)
{
MONO_PCM pcm0, pcm1;
int n, m, t0, I, J;
double fc, Q, a[3], b[3];
double f0, gain;
/* 音データ（入力信号） */
wave_read_16bit_mono(&pcm0, "ex1.wav");
/* 音データ（出力信号） */
pcm1.fs = 8000;
pcm1.bits = 16;
pcm1.length = pcm0.length;
pcm1.s = calloc(pcm1.length, sizeof(double));
/* フィルタ処理 */
fc = 250.0 / pcm0.fs;
Q = 1.0 / sqrt(2.0);
I = 2;
J = 2;
IIR_LPF(fc, Q, a, b);
IIR_filtering(pcm0.s, pcm1.s, pcm0.length,a, b, I, J);
/* ゲインの調節 */
gain = 2.0;
for (n = 0; n < pcm1.length; n++)
{
pcm1.s[n] *= gain;
}
/* フェード処理 */
for (n = 0; n < pcm1.fs * 0.01; n++)
{
pcm1.s[n] *= (double)n / (pcm1.fs * 0.01);
pcm1.s[pcm1.length - n - 1] *= (double)n / (pcm1.fs * 0.01);
}
/* WAVE ファイル */
wave_write_16bit_mono(&pcm1, "ex2.wav");
free(pcm0.s);
free(pcm1.s);
return 0;
}
