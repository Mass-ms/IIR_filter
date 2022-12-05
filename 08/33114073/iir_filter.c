#include "iir_filter.h"

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

void IIR_resonator(double fc, double Q, double a[], double b[])
{
    fc = tan(M_PI * fc) / (2.0 * M_PI);
    a[0] = 1.0 + 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc;
    a[1] = (8.0 * M_PI * M_PI * fc * fc - 2.0)
    / a[0];
    a[2] = (1.0 - 2.0 * M_PI * fc / Q
    + 4.0 * M_PI * M_PI * fc * fc) / a[0];
    b[0] = 2.0 * M_PI * fc / Q / a[0];
    b[1] = 0.0;
    b[2] = -2.0 * M_PI * fc/Q/ a[0];
    a[0] = 1.0;
}