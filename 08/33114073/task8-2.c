#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "iir_filter.h"

int main(void)
{
    MONO_PCM pcm0, pcm1;
    int n, m, t0, I, J;
    double *s, F1, F2, F3, F4, B1, B2, B3, B4;
    double fc, Q, a[3], b[3];
    double f0, gain;
    /* 音データ（入力信号） */
    wave_read_16bit_mono(&pcm0, "ex1.wav");
    /* 音データ（出力信号） */
    pcm1.fs = 8000;
    pcm1.bits = 16;
    pcm1.length = pcm0.length;
    pcm1.s = calloc(pcm1.length, sizeof(double));

    s = calloc(pcm1.length, sizeof(double));
    /* フィルタ処理 */
    F1 = 800.0;
    F2 = 1200.0;
    F3 = 2500.0;
    F4 = 3500.0;
    B1 = 100.0;
    B2 = 100.0;
    B3 = 100.0;
    B4 = 100.0;
    I = 2;
    J = 2;
    IIR_resonator(F1 / pcm0.fs, F1 / B1, a, b);
    IIR_filtering(pcm0.s, s, pcm0.length,
                  a, b, I, J);
    for (n = 0; n < pcm1.length; n++)
    {
        pcm1.s[n] += s[n];
        s[n] = 0.0;
    }
    IIR_resonator(F2 / pcm0.fs, F2 / B2, a, b);
    IIR_filtering(pcm0.s, s, pcm0.length,
                  a, b, I, J);
    for (n = 0; n < pcm1.length; n++)
    {
        pcm1.s[n] += s[n];
        s[n] = 0.0;
    }
    IIR_resonator(F3 / pcm0.fs, F3 / B3, a, b);
    IIR_filtering(pcm0.s, s, pcm0.length,
                  a, b, I, J);
    for (n = 0; n < pcm1.length; n++)
    {
        pcm1.s[n] += s[n];
        s[n] = 0.0;
    }
    IIR_resonator(F4 / pcm0.fs, F4 / B4, a, b);
    IIR_filtering(pcm0.s, s, pcm0.length,
                  a, b, I, J);
    for (n = 0; n < pcm1.length; n++)
    {
        pcm1.s[n] += s[n];
        s[n] = 0.0;
    }

    /* ディエンファシス処理 */
    s[0] = pcm1.s[0];
    for (n = 1; n < pcm1.length; n++)
    {
        s[n] = 0.98 * s[n - 1] + pcm1.s[n];
    }
    for (n = 0; n < pcm1.length; n++)
    {
        pcm1.s[n] = s[n];
    }

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
    wave_write_16bit_mono(&pcm1, "ex3.wav");
    free(pcm0.s);
    free(pcm1.s);

    return 0;
}