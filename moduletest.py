import himodule
import DFT_slow
import datatools as dtools
import numpy as np
import pylab as plt
import fft
import discrete_freq as df
import time

#data = np.genfromtxt('data.txt', delimiter=', ', names=True, skiprows=3)
data = np.genfromtxt('fft_output.txt', delimiter=', ', names=True, skiprows=3)
list_data = [list(row) for row in data]
x = np.asarray(list_data, dtype=float)
x = np.transpose(x)
#N = len(x[1])

plt.semilogy(x[0])

#df.find_discrete_freq(x[1], 4)

#FFT1 = df.slow_DFT(x[1], 0, 4000, 8000)
#FFT2 = df.slow_DFT(x[2], 0, 4000, 8000)

#FFT2 = DFT_slow.DFT_slow(x[2])
#FFT2 = fft.FFT(x[2])

#print FFT1
#print FFT2
#plt.semilogy(np.abs(FFT1))
#plt.semilogy(np.abs(FFT2))
plt.show()

