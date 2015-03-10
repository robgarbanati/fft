import numpy as np
from scipy.fftpack import fft, ifft
import pylab as plt

#def plotfft(data_list, sample_freq, start=0, end=0):
def plotfft(data_list, sample_freq):
    nyquist_freq = sample_freq/2
    #if end == 0:
	#end = nyquist_freq
    num_samples = len(data_list)
    #x = np.linspace(start_freq, end_freq, num_samples/2)
    x = np.linspace(0.0, nyquist_freq, num_samples/2)
    fft_list = fft(data_list)
    plt.semilogy(x, 2.0/num_samples * np.abs(fft_list[0:num_samples/2]))
    return

