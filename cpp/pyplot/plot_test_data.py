'''
Brief:               Plot to see if the worms have expected scaling behaviour

File name:           plottest.py
Author:              Simon Rydell
Date created:        Jun 29, 2018
Date last modified:  Jun 29, 2018
Python Version:      3.6
'''
import matplotlib.pyplot as plt
import numpy as np
from gauss_newton import GN_temp

from matplotlib import rc
rc('font',**{'family':'sans-serif','sans-serif':['Helvetica']})
## for Palatino and other serif fonts use:
#rc('font',**{'family':'serif','serif':['Palatino']})
rc('text', usetex=True)

files = ["./avg_sizes.txt", "./avg_tanh_sizes.txt", "./final_sizes.txt"]
files = ["./final_sizestesting.txt"]
for n, filename in enumerate(files):
    with open(filename) as f:
        max_size = 0
        min_size = 0
        num_simulations = 0
        # sizes: 8, 16, 32, 64
        sizes = [[], [], [], []]
        for line in f.readlines():
            # Each line is a simulation
            num_simulations += 1
            loop_lengths = [float(i) for i in line.split(" ")]
            for i, l in enumerate(loop_lengths):
                sizes[i].append(l)

    system_sizes = [8*8, 16*16, 32*32, 64*64]
    # avg_loop_sizes = []
    for i, ll in enumerate(sizes):
        plt.scatter([system_sizes[i]]*len(ll), ll)
        # avg_loop_sizes.append(np.mean(ll))
    # plt.plot(system_sizes, avg_loop_sizes, label="Average final size")

    # x = []
    # y = []
    # for k, size in enumerate(sizes):
    #     for s in size:
    #         y.append(s)
    #         x.append(system_sizes[k])

    x = []
    y = []
    for k, size in enumerate(sizes):
        print(np.mean(size))
        print(system_sizes[k])
        y.append(np.mean(size))
        x.append(system_sizes[k])


    
    gn = GN_temp()
    gn.setup(y, x, np.array([10.0, 1.0]))
    parameters = gn.optimize(100)
    # print(x)
    # print(y)
    # p = np.polyfit(x, y, 2)
    x = np.array(x)
    print(parameters)
    # quit()
    # plt.plot(x, np.polyval(p, x))
    plt.plot(x, parameters[0] + x ** parameters[1], label=f"\propto N^{{ 1 / {1/parameters[1]:.2f} }}")
    # plt.plot(*p.linspace())
    plt.legend()

    titles  = ["Ising lattice after 10000 worms started on each lattice size", "Average size as avg(L) = sum(L * tanh^L(K)) / sum(tanh^L(K))", "Largest loop"]
    yaxises = ["Size of the largest loop", "Loop size", "Size of the largest loop"]
    plt.xlabel("System size")
    plt.ylabel(yaxises[n])
    plt.title(titles[n])
    xt = [f"${np.sqrt(size):.0f}^2$" for size in system_sizes]
    plt.xticks(system_sizes, [r"$8^2$", r"$16^2$", r"$32^2$", r"$64^2$"])

    # plt.show()
    plt.tight_layout()
    plt.savefig("largest_worm_sizes_scaling.png", bbox_inches='tight')
