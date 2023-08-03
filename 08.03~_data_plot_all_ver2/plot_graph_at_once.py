import numpy as np
import matplotlib.pyplot as plt
import os
folder_path = os.getcwd()
file_list = os.listdir(folder_path)

#open files that end with ".txt"
for file_name in file_list:
    if ( (not file_name.startswith("Ising_")) or (not file_name.endswith(".txt"))):
        continue
    last_real_filename = file_name
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
#plot graph
    h = plt.errorbar(x_values, y_means, yerr=y_stddevs, ecolor='red' , elinewidth=1, label = file_name[6:].split("_")[0])
    h.get_children()[1].set_linestyle('--')

    plt.xlabel('T')
    plt.ylabel('<m>')
    plt.suptitle('ClusterIsingModel_graph', fontsize = 15)
    plt.title(file_name[10:-4] , fontsize = 10)#[6:-4]
    plt.ylim(0, 1.1)
    plt.legend(loc = 'lower left')
    
    
plt.savefig(f'{last_real_filename[:-4]}.jpg', dpi=3000)
plt.show()