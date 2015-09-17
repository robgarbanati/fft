#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <complex.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

#define ADC_BUFFER_SIZE  32
#define PI 3.14159265359

typedef struct 
{
    double *array;
    int length;
} double_array;

typedef struct 
{
    complex double *array;
    int length;
} complex_array;


double cmag(complex double number)
{
    double realnum = creal(number);
    double inum = cimag(number);
    return sqrt(realnum*realnum + inum*inum);
}


/*void init_x(int16_t* x)*/
void init_x(double *x)
{
    FILE* data_fd = fopen("data.txt", "r");
    if(data_fd < ((FILE* ) 0))
    {
        perror("open test.txt");
        return;
    }

    int i=0;
    /*int16_t sample;*/
    double sample;
    char buffer[5];
    char temp[5];

    for(i=0;i<ADC_BUFFER_SIZE;i++)
    {
        if ((fscanf(data_fd, "%s %s %s ", temp, buffer, temp)) == 3)
        {
            /*sample = (int16_t) atoi(buffer);*/
            sample = atof(buffer);
            x[i] = sample;
            printf("buffer %s, sample %.2f, x[%d] %.2f\n", buffer, sample, i, x[i]);
        }
        else
        {
            puts("size mismatch");
            assert(0);
        }
    }

    fclose(data_fd);
}

/*int16_t find_discrete_freq(int16_t *x)*/
complex double find_discrete_freq(double_array *x, int k)
{
    int n;
    complex double Xk = 0;
    double N = (double) x->length;

    for(n=0;n<x->length;n++)
    {
        Xk += cexp(-2*I*PI*k*n/N) * (complex double) x->array[n];
        /*printf("x[%d]=%.2f, cexp=%.2f+%.2fi, Xk=%.2f\n", n, x->array[n], creal(cexp(-2*I*PI*k*n/N)), cimag(cexp(-2*I*PI*k*n/N)), cmag(Xk));*/
    }
    /*printf("Xk=%.2f+%.2fi\n", creal(Xk), cimag(Xk));*/
    /*return cmag(Xk);*/
    return Xk;
}

void slow_DFT(double_array *x, complex_array *y)
{
    int k;
    for(k=0;k<y->length;k++)
    {
        y->array[k] = find_discrete_freq(x, k);
        /*printf("Xk=%.2f+%.2fi\n", creal(Xk), cimag(Xk));*/
        printf("y[%d] = %.2f+%.2fi\n", k, creal(y->array[k]), cimag(y->array[k]));
        /*printf("y[%d] = %.2f\n", k, y->array[k]);*/
    }
}

void copy_DFT(double_array *x, double_array *y)
{
    int k;
    for(k=0;k<x->length;k++)
    {
        y->array[k] = x->array[k];
    }
}

void fast_FFT(double_array *x, complex_array *y)
{
    /*int k;*/
    int i = 0;
    int newlength = x->length/2;
    double even_xarray[newlength];
    double odd_xarray[newlength];
    complex double even_yarray[newlength];
    complex double odd_yarray[newlength];

    double_array x_even = 
    {
        even_xarray,
        newlength
    };

    double_array x_odd = 
    {
        odd_xarray,
        newlength
    };

    complex_array y_odd = 
    {
        odd_yarray,
        newlength
    };

    complex_array y_even = 
    {
        even_yarray,
        newlength
    };

    for(i=0;i<x->length/2;i++)
    {
        odd_xarray[i] = x->array[i*2+1];
    }

    for(i=0;i<x->length/2;i++)
    {
        even_xarray[i] = x->array[i*2];
    }

    for(i=0;i<x->length;i++)
    {
        printf("%.2f ", x->array[i]);
    }
    puts("\n");

    if(x->length % 2 != 0)
    {
        fprintf(stderr, "xarray should be a power of 2");
        exit(-1);
    }
    else if (x->length <= 4)
    {
        slow_DFT(x, y);
    }
    else
    {
        fast_FFT(&x_even, &y_even);
        fast_FFT(&x_odd, &y_odd);

        complex double factor_array[x->length];
        double N = x->length;

        for(i=0;i<x->length;i++)
        {
            factor_array[i] = cexp(-2I * PI * (double) i / N);
            /*printf("factor[%d] = %.2f + %.2fi\n", i, creal(factor_array[i]), cimag(factor_array[i]));*/
        }

        printf("y_even:\n");
        for(i=0;i<y_even.length;i++)
        {
            printf("%.2f %.2f\n", creal(y_even.array[i]), cimag(y_even.array[i]));
        }
        puts("");
        printf("y_odd:\n");
        for(i=0;i<y_even.length;i++)
        {
            printf("%.2f %.2f\n", creal(y_odd.array[i]), cimag(y_odd.array[i]));
        }
        puts("");
        for(i=0;i<y_even.length;i++)
        {
            y->array[i] = y_even.array[i] + factor_array[i] * y_odd.array[i];
            y->array[i+y_even.length] = y_even.array[i] + factor_array[i+y_even.length] * y_odd.array[i];
        }
        puts("");
        puts("y:");
        for(i=0;i<y->length;i++)
        {
            printf("%.2f %.2f\n", creal(y->array[i]), cimag(y->array[i]));
        }
    }
}


/*
 *def FFT(x):
 *    """A recursive implementation of the 1D Cooley-Tukey FFT"""
 *    x = np.asarray(x, dtype=float)
 *    N = x.shape[0]
 *
 *    if N % 2 > 0:
 *        raise ValueError("size of x must be a power of 2")
 *    elif N <= 32:  # this cutoff should be optimized
 *        return DFT_slow(x)
 *    else:
 *        X_even = FFT(x[::2])
 *        X_odd = FFT(x[1::2])
 *        factor = np.exp(-2j * np.pi * np.arange(N) / N)
 *        return np.concatenate([X_even + factor[:N / 2] * X_odd,
 *                               X_even + factor[N / 2:] * X_odd])
 */


int main(void)
{
    int i;
    /*int16_t x[ADC_BUFFER_SIZE];*/
    /*int16_t y[ADC_BUFFER_SIZE];*/
    double x[ADC_BUFFER_SIZE];
    complex double y[ADC_BUFFER_SIZE];
    double_array xarray = 
    {
        x,
        ADC_BUFFER_SIZE
    };
    complex_array yarray = 
    {
        y,
        ADC_BUFFER_SIZE
    };
    /*
     *struct rusage ru;
     *struct timeval utime;
     *struct timeval stime;
     */

    /*init_x(x);*/
    for(i=0;i<ADC_BUFFER_SIZE;i++)
    {
        x[i] = i;
    }

    /*slow_DFT(x, y);*/
    fast_FFT(&xarray, &yarray);

    /*
     *    FILE* fft_fd = fopen("fft_output.txt", "w");
     *
     *    if(fft_fd < ((FILE* ) 0))
     *    {
     *        perror("open fft_output.txt");
     *        return -1;
     *    }
     *    puts("opened fft_output.txt!");
     *
     *    for(i=0;i<ADC_BUFFER_SIZE/2;i++)
     *    {
     *        fprintf(fft_fd, "%.2f\n", y[i]);
     *        printf("%.2f\n", y[i]);
     *    }
     *
     *    getrusage(RUSAGE_SELF, &ru);
     *
     *    utime = ru.ru_utime;
     *    stime = ru.ru_stime;
     *
     *    printf("RUSAGE :ru_utime => %lld [sec] : %lld [usec], :ru_stime => %lld [sec] : %lld [usec]\n",
     *            (int64_t)utime.tv_sec, (int64_t)utime.tv_usec,
     *            (int64_t)stime.tv_sec, (int64_t)stime.tv_usec);
     *    fclose(fft_fd);
     */

}


/*
 *def find_discrete_freq(x, k):
 *    num_samples = len(x)
 *    print 'num_samples'
 *    print num_samples
 *    n = np.arange(num_samples)
 *    array_of_exp = np.exp(-2j * np.pi * k * n / num_samples)
 *    Xk = np.dot(x, array_of_exp)
 *    print 'Xk'
 *    print Xk
 *    return Xk
 *
 *def slow_DFT(x, start_freq, end_freq, sample_freq):
 *    num_samples = len(x)
 *    nyquist_freq = sample_freq/2
 *    start_k = int(start_freq*num_samples/2/nyquist_freq)
 *    end_k = int(end_freq*num_samples/2/nyquist_freq)
 *    fft_array = range(start_k,end_k,1)
 *    frequencies = range(start_k,end_k,1)
 *    print fft_array
 *    for i,k in enumerate(frequencies):
 *        fft_array[i] = np.abs(find_discrete_freq(x, k))
 *    return fft_array
 */
