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

#define ADC_BUFFER_SIZE  512
#define N   ADC_BUFFER_SIZE
#define PI 3.14159265359


double cmag(complex number)
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
	    printf("buffer %s, sample %lf, x[%d] %lf\n", buffer, sample, i, x[i]);
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
double find_discrete_freq(double *x, int k)
{
    int n;
    complex Xk = 0;

    for(n=0;n<ADC_BUFFER_SIZE;n++)
    {
	Xk += cexp(-2*I*PI*k*n/N) * (complex) x[n];
	/*printf("x[%d]=%lf, cexp=%lf+%lfi, Xk=%lf\n", n, x[n], creal(cexp(-2*I*PI*k*n/N)), cimag(cexp(-2*I*PI*k*n/N)), Xk);*/
    }
    printf("Xk=%lf+%lfi\n", creal(Xk), cimag(Xk));
    return cmag(Xk);
}

void slow_DFT(double *x, double *y)
{
    int k;
    for(k=0;k<ADC_BUFFER_SIZE/2;k++)
    {
	y[k] = find_discrete_freq(x, k);
	printf("y[%d] = %lf\n", k, y[k]);
    }

}

int main(void)
{
    int i;
    /*int16_t x[ADC_BUFFER_SIZE];*/
    /*int16_t y[ADC_BUFFER_SIZE];*/
    double x[ADC_BUFFER_SIZE];
    double y[ADC_BUFFER_SIZE/2];

    struct rusage ru;
    struct timeval utime;
    struct timeval stime;

    init_x(x);
    slow_DFT(x, y);

    FILE* fft_fd = fopen("fft_output.txt", "w");

    if(fft_fd < ((FILE* ) 0))
    {
	perror("open fft_output.txt");
	return;
    }
    puts("opened fft_output.txt!");

    for(i=0;i<ADC_BUFFER_SIZE/2;i++)
    {
	fprintf(fft_fd, "%lf\n", y[i]);
	printf("%lf\n", y[i]);
    }

    getrusage(RUSAGE_SELF, &ru);

    utime = ru.ru_utime;
    stime = ru.ru_stime;

    printf("RUSAGE :ru_utime => %lld [sec] : %lld [usec], :ru_stime => %lld [sec] : %lld [usec]\n",
	    (int64_t)utime.tv_sec, (int64_t)utime.tv_usec,
	    (int64_t)stime.tv_sec, (int64_t)stime.tv_usec);
    fclose(fft_fd);

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
