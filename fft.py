import numpy as np
from DFT_slow import *

def FFT(x):
    print "FFT(%g)" % x.shape
    print "x: ", x
    """A recursive implementation of the 1D Cooley-Tukey FFT"""
    x = np.asarray(x, dtype=float)
    N = x.shape[0]

    if N % 2 > 0:
        raise ValueError("size of x must be a power of 2")
    elif N <= 4:  # this cutoff should be optimized
        return DFT_slow(x)
    else:
        X_even = FFT(x[::2])
        X_odd = FFT(x[1::2])
        factor = np.exp(-2j * np.pi * np.arange(N) / N)
        return np.concatenate([X_even + factor[:N / 2] * X_odd,
                               X_even + factor[N / 2:] * X_odd])


import matplotlib.pyplot as plt
import numpy as np


Fs = 8000
f = 500
sample = 64
wf = np.linspace(0.0, Fs/2, sample/2)
x = np.arange(sample)
y = x
#x = np.arange(sample/2)
#y = np.sin(2 * np.pi * f * x / Fs)
#y = np.sin(2 * np.pi * f * x / Fs)
#plt.plot(x, y)
ffty = FFT(y)
#ffty = DFT_slow(y)
plt.semilogy(wf, 2.0/sample * np.abs(ffty[0:sample/2]))
plt.ylabel('voltage(V)')
plt.xlabel('sample(n)')
plt.show()
