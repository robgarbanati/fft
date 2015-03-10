import numpy as np
import pylab as plt

def faster_find_discrete_freq(x, k, N):
    num_samples = len(x)
    assert num_samples % 2 == 0
    #print 'num_samples'
    #print num_samples
    if num_samples <= 16:
    #if num_samples <= 128:
    #if num_samples <= 512:
	print x
	return find_discrete_freq(x, k)
    else:
	x_even = x[::2]
	print x_even
	x_odd = x[1::2]
	print x_odd
	even = faster_find_discrete_freq(x_even, k, N)
	odd = faster_find_discrete_freq(x_odd, k, N)
	print 'even'
	print even
	print 'odd'
	print odd
	W = np.exp(-2j * np.pi * k / num_samples)
	Xk = even + W*odd
	print 'Xk'
	print Xk
	return even + W*odd

def find_discrete_freq(x, k):
    num_samples = len(x)
    n = np.arange(num_samples)
    array_of_exp = np.exp(-2j * np.pi * k * n / num_samples)
    plt.plot(array_of_exp)
    print array_of_exp
    Xk = np.dot(x, array_of_exp)
    plt.show()
    return Xk

def slow_DFT(x, start_freq, end_freq, sample_freq):
    num_samples = len(x)
    nyquist_freq = sample_freq/2
    start_k = int(start_freq*num_samples/2/nyquist_freq)
    end_k = int(end_freq*num_samples/2/nyquist_freq)
    fft_array = range(start_k,end_k,1)
    frequencies = range(start_k,end_k,1)
    for i,k in enumerate(frequencies):
	fft_array[i] = np.abs(find_discrete_freq(x, k))
    return fft_array

def medium_DFT(x, start_freq, end_freq, sample_freq):
    num_samples = len(x)
    nyquist_freq = sample_freq/2
    start_k = int(start_freq*num_samples/2/nyquist_freq)
    end_k = int(end_freq*num_samples/2/nyquist_freq)
    fft_array = range(start_k,end_k,1)
    frequencies = range(start_k,end_k,1)
    print fft_array
    for i,k in enumerate(frequencies):
	fft_array[i] = np.abs(faster_find_discrete_freq(x, k, num_samples))
	#fft_array[i] = np.abs(practice_method(x, k))
	#fft_array[i] = np.abs(find_discrete_freq(x, k))
    return fft_array

