'''
Brief:               Plot the benchmark results from the Lattice implementation benchmark

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

if __name__ == '__main__':
    dimension = 2
    # BM_singlevec/128      41051 ns      41025 ns      16924
    singlevec_data = calc.process_file(f"./data/benchmark_latticeimpl.txt",
                                       key=r"xxxx",
                                       xy=r"BM_singlevec/(\d*\.?\d*)\s*(\d*\.?\d*) ns\s*(\d*\.?\d*) ns \s* (\d*\.?\d*)")

    multivec_data = calc.process_file(f"./data/benchmark_latticeimpl.txt",
                                      key=r"xxxx",
                                      xy=r"BM_vecofvecs/(\d*\.?\d*)\s*(\d*\.?\d*) ns\s*(\d*\.?\d*) ns \s* (\d*\.?\d*)")

    sizes_single, _time, cpu_time_single, _num_measurents = singlevec_data[0]
    sizes_multi, _time, cpu_time_multi, _num_measurents = multivec_data[0]

    # Normalize cpu_time_single
    cpu_time_single = [t / max([cpu_time_single[0], cpu_time_multi[0]]) for t in cpu_time_single]
    cpu_time_multi = [t / max([cpu_time_single[0], cpu_time_multi[0]]) for t in cpu_time_multi]

    # Get total number of sites
    # Assume same number of sites
    sites = [np.power(s, dimension) for s in sizes_single]

    cpu_time_single = np.array(cpu_time_single)
    cpu_time_multi = np.array(cpu_time_multi)
    sites = np.array(sites)

    plt.scatter(sites, cpu_time_multi,
                c=const.COLOR_MAP["red"],
                s=25,
                label=r"$\bar t_{multi}$")
    plt.scatter(sites, cpu_time_single,
                c=const.COLOR_MAP["green"],
                s=25,
                label=r"$\bar t_{single}$")

    plt.xscale("log", nonposx='clip')
    # How many of the smallest points should be excluded from the plot
    i = 0
    plt.xticks(sites[i:], [rf"${int(s)}^{dimension}$" for s in sizes_single[i:]])
    plt.yticks([], [])

    plt.xlabel("Number of sites of 4 bytes")
    plt.ylabel("Time")
    plt.title("Benchmark of Lattice Implementation")
    plt.legend(loc=2)

    # illu.save_figure(f"bench_latticeimpl")

    plt.show()
