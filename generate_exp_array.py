import numpy as np
N = 512
k = 1
n = np.arange(N)

cfile=open('./exponent_table.h', 'w')

print >>cfile, 'double real_exp_array[] = {'

array_of_exp = np.exp(-2j * np.pi * k * n / N)
for i in array_of_exp[:-1]:
    print >>cfile, "%g,\n" % np.real(i),
else:
    print >>cfile, "%g\n" % np.real(i),

print >>cfile, '};\n'


print >>cfile, 'double imag_exp_array[] = {'

array_of_exp = np.exp(-2j * np.pi * k * n / N)
for i,num in enumerate(array_of_exp[:-1]):
    print >>cfile, "%g,\n" % np.imag(num),
#else:
    #print >>cfile, "%g\n" % np.imag(num),

print >>cfile, '};\n'

