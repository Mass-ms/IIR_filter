#include "iir_filter.h"

void IIR_setup(int filterType, int fs, double *fc, double *fc1, double *fc2, double *Q, double *G)
{
    double f, q, g, f1, f2;
    if (filterType != BPF && filterType != BEF)
    {
        printf("遮断周波数を入力して下さい。\n");
        printf("例: 1000.0\n");
        do
        {
            scanf("%lf", &f);
            if (f < 0)
            {
                printf("\a有効な値を入力してください。\n");
            }
        } while (f < 0);
        printf("Q値を入力して下さい。\n");
        printf("例: 1.0\n");
        do
        {
            scanf("%lf", &q);
            if (q < 0)
            {
                printf("\a有効な値を入力してください。\n");
            }
        } while (q < 0);
        *fc = f / fs;       /* 遮断周波数 */
        *Q = q / sqrt(2.0); /* クオリティファクタ */
    }
    else
    {
        printf("第一遮断周波数を入力して下さい。\n");
        printf("例: 1000.0\n");
        do
        {
            scanf("%lf", &f1);
            if (f1 < 0)
            {
                printf("\a有効な値を入力してください。\n");
            }
        } while (f1 < 0);
        printf("第二遮断周波数を入力して下さい。\n");
        printf("例: 2000.0\n");
        do
        {
            scanf("%lf", &f2);
            if (f2 < 0)
            {
                printf("\a有効な値を入力してください。\n");
            }
        } while (f2 < 0);
        *fc1 = f1 / fs; /* 第一遮断周波数 */
        *fc2 = f2 / fs; /* 第二遮断周波数 */
    }
    if (filterType == LowShelf || filterType == HighShelf || filterType == Peak)
    {
        printf("ゲインを入力して下さい。\n");
        printf("例: 1.0\n");
        scanf("%lf", &g);
        *G = g; /* ゲイン */
    }
}

void IIR_filtering(double x[], double y[], int L, double a[], double b[], int I, int J)
{
    int n, m;
    for (n = 0; n < L; n++)
    {
        for (m = 0; m <= J; m++)
        {
            if (n - m >= 0)
            {
                y[n] += b[m] * x[n - m];
            }
        }
        for (m = 1; m <= I; m++)
        {
            if (n - m >= 0)
            {
                y[n] += -a[m] * y[n - m];
            }
        }
    }
}

void IIR_LPF(double fc, double Q, double a[], double b[])
{
    fc = tan(M_PI * fc) / (2.0 * M_PI);

    a[0] = 1.0 + 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc;
    a[1] = (8.0 * M_PI * M_PI * fc * fc - 2.0) / a[0];
    a[2] = (1.0 - 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc) / a[0];
    b[0] = 4.0 * M_PI * M_PI * fc * fc / a[0];
    b[1] = 8.0 * M_PI * M_PI * fc * fc / a[0];
    b[2] = 4.0 * M_PI * M_PI * fc * fc / a[0];
    a[0] = 1.0;
}

void IIR_HPF(double fc, double Q, double a[], double b[])
{
    fc = tan(M_PI * fc) / (2.0 * M_PI);

    a[0] = 1.0 + 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc;
    a[1] = (8.0 * M_PI * M_PI * fc * fc - 2.0) / a[0];
    a[2] = (1.0 - 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc) / a[0];
    b[0] = 1.0 / a[0];
    b[1] = -2.0 / a[0];
    b[2] = 1.0 / a[0];

    a[0] = 1.0;
}

void IIR_BPF(double fc1, double fc2, double a[], double b[])
{
    fc1 = tan(M_PI * fc1) / (2.0 * M_PI);
    fc2 = tan(M_PI * fc2) / (2.0 * M_PI);

    a[0] = 1.0 + 2.0 * M_PI * (fc2 - fc1) + 4.0 * M_PI * M_PI * fc1 * fc2;
    a[1] = (8.0 * M_PI * M_PI * fc1 * fc2 - 2.0) / a[0];
    a[2] = (1.0 - 2.0 * M_PI * (fc2 - fc1) + 4.0 * M_PI * M_PI * fc1 * fc2) / a[0];
    b[0] = 2.0 * M_PI * (fc2 - fc1) / a[0];
    b[1] = 0.0;
    b[2] = -2.0 * M_PI * (fc2 - fc1) / a[0];

    a[0] = 1.0;
}

void IIR_BEF(double fc1, double fc2, double a[], double b[])
{
    fc1 = tan(M_PI * fc1) / (2.0 * M_PI);
    fc2 = tan(M_PI * fc2) / (2.0 * M_PI);

    a[0] = 1.0 + 2.0 * M_PI * (fc2 - fc1) + 4.0 * M_PI * M_PI * fc1 * fc2;
    a[1] = (8.0 * M_PI * M_PI * fc1 * fc2 - 2.0) / a[0];
    a[2] = (1.0 - 2.0 * M_PI * (fc2 - fc1) + 4.0 * M_PI * M_PI * fc1 * fc2) / a[0];
    b[0] = (4.0 * M_PI * M_PI * fc1 * fc2 + 1.0) / a[0];
    b[1] = (8.0 * M_PI * M_PI * fc1 * fc2 - 2.0) / a[0];
    b[2] = (4.0 * M_PI * M_PI * fc1 * fc2 + 1.0) / a[0];

    a[0] = 1.0;
}

void IIR_resonator(double fc, double Q, double a[], double b[])
{
    fc = tan(M_PI * fc) / (2.0 * M_PI);
    a[0] = 1.0 + 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc;
    a[1] = (8.0 * M_PI * M_PI * fc * fc - 2.0) / a[0];
    a[2] = (1.0 - 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc) / a[0];
    b[0] = 2.0 * M_PI * fc / Q / a[0];
    b[1] = 0.0;
    b[2] = -2.0 * M_PI * fc / Q / a[0];
    a[0] = 1.0;
}

void IIR_notch(double fc, double Q, double a[], double b[])
{
    fc = tan(M_PI * fc) / (2.0 * M_PI);

    a[0] = 1.0 + 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc;
    a[1] = (8.0 * M_PI * M_PI * fc * fc - 2.0) / a[0];
    a[2] = (1.0 - 2.0 * M_PI * fc / Q + 4 * M_PI * M_PI * fc * fc) / a[0];
    b[0] = (4.0 * M_PI * M_PI * fc * fc + 1.0) / a[0];
    b[1] = (8.0 * M_PI * M_PI * fc * fc - 2.0) / a[0];
    b[2] = (4.0 * M_PI * M_PI * fc * fc + 1.0) / a[0];

    a[0] = 1.0;
}

void IIR_low_shelving(double fc, double Q, double g, double a[], double b[])
{
    fc = tan(M_PI * fc) / (2.0 * M_PI);

    a[0] = 1.0 + 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc;
    a[1] = (8.0 * M_PI * M_PI * fc * fc - 2.0) / a[0];
    a[2] = (1.0 - 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc) / a[0];
    b[0] = (1.0 + sqrt(1.0 + g) * 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc * (1.0 + g)) / a[0];
    b[1] = (8.0 * M_PI * M_PI * fc * fc * (1.0 + g) - 2.0) / a[0];
    b[2] = (1.0 - sqrt(1.0 + g) * 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc * (1.0 + g)) / a[0];

    a[0] = 1.0;
}

void IIR_high_shelving(double fc, double Q, double g, double a[], double b[])
{
    fc = tan(M_PI * fc) / (2.0 * M_PI);

    a[0] = 1.0 + 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc;
    a[1] = (8.0 * M_PI * M_PI * fc * fc - 2.0) / a[0];
    a[2] = (1.0 - 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc) / a[0];
    b[0] = ((1.0 + g) + sqrt(1.0 + g) * 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc) / a[0];
    b[1] = (8.0 * M_PI * M_PI * fc * fc - 2.0 * (1.0 + g)) / a[0];
    b[2] = ((1.0 + g) - sqrt(1.0 + g) * 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc) / a[0];

    a[0] = 1.0;
}

void IIR_peaking(double fc, double Q, double g, double a[], double b[])
{
    fc = tan(M_PI * fc) / (2.0 * M_PI);

    a[0] = 1.0 + 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc;
    a[1] = (8.0 * M_PI * M_PI * fc * fc - 2.0) / a[0];
    a[2] = (1.0 - 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc) / a[0];
    b[0] = (1.0 + 2.0 * M_PI * fc / Q * (1.0 + g) + 4.0 * M_PI * M_PI * fc * fc) / a[0];
    b[1] = (8.0 * M_PI * M_PI * fc * fc - 2.0) / a[0];
    b[2] = (1.0 - 2.0 * M_PI * fc / Q * (1.0 + g) + 4.0 * M_PI * M_PI * fc * fc) / a[0];

    a[0] = 1.0;
}