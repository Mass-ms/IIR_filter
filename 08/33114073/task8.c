#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
    MONO_PCM pcm;
    int n;
    double a, f0;

    /* pcmデータの配列(1 dim) */
    pcm.fs = 8000;
    pcm.bits = 16; 
    pcm.length = pcm.fs * 1; 
    pcm.s = calloc(pcm.length, sizeof(double));

    /* サイン波の合成 */
    a = 0.5; //振幅
    f0 = 1000.0; //周波数
    for (n = 0; n < pcm.length; n++)
    {
        pcm.s[n] = a * sin(2.0 * M_PI * f0 * n / pcm.fs);
    }

    /* フェード処理 */
    for (n = 0; n < pcm.length * 0.01; n++)
    {
        pcm.s[n] *= (double)n / (pcm.fs * 0.01);
        pcm.s[pcm.length - n - 1] *= (double)n / (pcm.fs * 0.01);
    }

    /* WAVE ファイル */
    wave_write_16bit_mono(&pcm, "ex1.wav");
    free(pcm.s);
    return 0;
}



