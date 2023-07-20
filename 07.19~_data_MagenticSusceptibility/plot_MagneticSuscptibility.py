import numpy as np
import matplotlib.pyplot as plt

file_ss = "Ising_MSLF_32x32_MCeff2000_intv0.300.txt"
file_s = "Ising_32x32_MCeff2000_intv0.300<|m|>.txt"

x_values1 = []
_SS = []
x_values2 = []
_S = []

with open(file_ss, 'r') as file1:
    for line in file1:
        data = [float(value) for value in line.strip().split(',')]
        x_values1.append(data[0])
        _SS.append(np.mean(data[1:11]))
with open(file_s, 'r') as file2:
    for line in file2:
        data = [float(value) for value in line.strip().split(',')]
        x_values2.append(data[0])
        _S.append(np.mean(data[1:11]))

if (len(x_values1) != len(x_values2)):
    print("ERROR:12083779dh892hbje")
    exit()

N_lattice = 32**2
y_value = []
for i in range(len(_SS)):
    y_value.append(1/(x_values1[i]*N_lattice)*(_SS[i] - _S[i]*_S[i]))

plt.xlabel("T")
plt.ylabel('MagneticSuscptibility')
plt.savefig(f'{file_ss[:-4]}.jpg', dpi=3000)
plt.show()