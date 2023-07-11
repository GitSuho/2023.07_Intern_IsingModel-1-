import numpy as np
import matplotlib.pyplot as plt
import os
from scipy.optimize import curve_fit
folder_path = os.getcwd()
file_list = os.listdir(folder_path)

#open files that end with ".txt"
for file_name in file_list:
    if not file_name.endswith(".txt"):
        continue
    x_values = []
    y_means = []
    y_abs = []
    y_stddevs = []
    
#file open and store mean and std datas    
    file_path = os.path.join(folder_path, file_name)
    with open(file_path, 'r') as file:
        for line in file:
            data = [float(value) for value in line.strip().split(',')]
            x_values.append(data[0])
            y_means.append(np.mean(data[1:11]))
            y_stddevs.append(np.std(data[1:11]))
    
#flip graph data when <m> is goes to minus one.
    if (y_means[-1] < 0 ):
        for i in range(len(y_means)):
            y_means[i] *= -1
    # y_abs = np.abs(y_means)

#fit function and curve fitting work
    def fit_function(x, a, b, c):
        x = np.asarray(x)
        return a*(((x-b)/c)**(1/8)) # m~[(Tc-T)/Tc]^{1/8}
    fit_const, _ = curve_fit(fit_function, x_values, y_means, p0=[-0.5, 1, -2, 1])
    x_fit_values = np.linspace(0.6, 3, 200)

#plot graph
    h = plt.errorbar(x_values, y_means, yerr=y_stddevs, ecolor='red' , elinewidth=1)
    h.get_children()[1].set_linestyle('--')
    # plt.plot(x_values, y_abs, color = 'green')
    plt.plot(x_fit_values, fit_function(x_fit_values, *fit_const), color = 'orange')
    plt.xlabel('T')
    plt.ylabel('<m>')
    plt.suptitle('IsingModel_graph', fontsize = 15)
    plt.title(file_name[6:-4] , fontsize = 10)
    plt.savefig(f'{file_name[:-4]}.jpg', dpi=3000)
    plt.show()