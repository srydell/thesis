'''
Brief:               Plot the benchmark results from the Graph dividing algorithm

File name:           plot_winding.py
Author:              Simon Rydell
Python Version:      3.7
'''

import re
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rc
import helpers.calc as calc
import helpers.constants as const
import helpers.illustrations as illu
from scipy.optimize import curve_fit

rc('font', **{'family': 'serif', 'serif': ['DejaVu Sans']})
rc('text', usetex=True)

def fit_func_linear(x, a, b, c):
    """y = a * x^b + c

    :x: Know input
    :a: Unknown fit data
    :b: Unknown fit data
    :c: Unknown fit data
    :returns: a * x^b + c
    """
    return a * np.power(x, b) + c

def fit_func_log(x, a, b, c):
    """y = a * log2(x) * x^b + c

    :x: Know input
    :a: Unknown fit data
    :b: Unknown fit data
    :c: Unknown fit data
    :returns: a * log2(x) * x^b + c
    """
    return a * np.log2(x) * np.power(x, b) + c


if __name__ == '__main__':
    dimension = 2
    # {box_size: [avg(D_H), std(D_H)], ...}
    # BM_DivideGraph/4        12078 ns      12064 ns      58434
    simulation_data = calc.process_file(f"./data/benchmark_graphdiv.txt",
                                        key=r"xxxx",
                                        xy=r"BM_DivideGraph/(\d*\.?\d*)\s*(\d*\.?\d*) ns\s*(\d*\.?\d*) ns \s* (\d*\.?\d*)")

    sizes, time, cpu_time, num_measurents = simulation_data[0]

    # Normalize cpu_time
    cpu_time = [t / cpu_time[0] for t in cpu_time]

    # Get total number of sites
    sites = [np.power(s, dimension) for s in sizes]

    cpu_time = np.array(cpu_time)
    sites = np.array(sites)

    # fit_data_linear = calc.bootstrap(fit_func_linear, sites, cpu_time, 400)
    # fit_data_log = calc.bootstrap(fit_func_log, sites, cpu_time, 400)
    # print(fit_data)
    # quit()

    ydata_linear = fit_func_linear(sites, 0.07, 1.004, 0.01)
    plt.loglog(sites, ydata_linear,
             c=const.COLOR_MAP["green"],
             # linestyle="--",
             linewidth=2,
             label=r"$\propto n^{1.01 \pm 0.05}$")

    # # a * np.log2(x) * np.power(x, b) + c
    # ydata_log = fit_func_log(sites, 0.02, 0.88, 0.26)
    # plt.loglog(sites, ydata_linear,
    #          c=const.COLOR_MAP["blue"],
    #          linestyle=":",
    #          linewidth=2,
    #          label=r"$\propto \log_2(n) n^{0.88 \pm 0.05}$")

    plt.scatter(sites, cpu_time,
                c=const.COLOR_MAP["black"],
                s=15,
                label=r"$\bar t$")

    # How many of the smallest points should be excluded from the plot
    i = 0
    plt.xticks(sites[i:], [rf"${int(s)}^{dimension}$" for s in sizes[i:]])
    plt.yticks([], [])

    plt.xlabel("Number of sites")
    plt.ylabel("Time")
    plt.title("Benchmark of Graph Dividing Algorithm")
    plt.legend(loc=2)

    # illu.save_figure(f"bench_graph_div")

    plt.show()
