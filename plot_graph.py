import numpy as np
import matplotlib.pyplot as plt
import os
from scipy.optimize import curve_fit


folder_path = os.getcwd()
file_list = os.listdir(folder_path)

for file_name in file_list:
    if not file_name.endswith(".txt"):
        continue

    x_values = []
    y_means = []
    y_abs = []
    y_stddevs = []
    
    file_path = os.path.join(folder_path, file_name)
    with open(file_path, 'r') as file:
        for line in file:
            data = [float(value) for value in line.strip().split(',')]
            x_values.append(data[0])
            y_mean = np.mean(data[1:11])
            y_stddev = np.std(data[1:11])
            y_means.append(y_mean)
            y_stddevs.append(y_stddev)
    if (y_means[-1] < 0 ):
        for i in range(len(y_means)):
            y_means[i] *= -1
    
    y_abs = np.abs(y_means)


    def tanh_ft(x, a, b, c, d):
        x = np.asarray(x)
        return a*np.tanh(b*x+c)+d
    popt, pcov = curve_fit(tanh_ft, x_values, y_means, p0=[-0.5, 1, -2, 1])
    x_tanh_values = np.linspace(0.6, 3, 200)
            
    h = plt.errorbar(x_values, y_means, yerr=y_stddevs, ecolor='red' , elinewidth=1)
    h.get_children()[1].set_linestyle('--')
    # plt.plot(x_values, y_abs, color = 'green')
    plt.plot(x_tanh_values, tanh_ft(x_tanh_values, *popt), color = 'orange')
    plt.xlabel('T')
    plt.ylabel('<m>')
    plt.suptitle('IsingModel_graph', fontsize = 15)
    plt.title(file_name[6:-4] , fontsize = 10)
    plt.savefig(f'{file_name[:-4]}.jpg', dpi=3000)
    plt.show()

