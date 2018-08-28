'''
Brief:               Generic plot sizes

File name:           plot_sizes.py
Author:              Simon Rydell
Date created:        Aug 20, 2018
Date last modified:  Aug 20, 2018
Python Version:      3.7
'''

import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rc
from scipy.optimize import curve_fit

rc('font', **{'family': 'serif', 'serif': ['DejaVu Sans']})
rc('text', usetex=True)

def process_file(filename):
    """Go through filename and split it into arrays
       Assumes that the file is organized as:

       <max_loop_length00> <max_loop_length01> ...
       <max_loop_length10> <max_loop_length11> ...
       ...

       Where the system_linear_size is 2, 4, 8, ... on each line

    :filename: String - Valid path to a file containing the abow
    :returns: dict - {<system_linear_size0>:
                      [<max_loop_length00>, <max_loop_length10, ...], ...}
    """
    out_dict = {}
    with open(filename) as data_file:
        for line in data_file.readlines():
            # Split the data into an array
            data = [int(d) for d in line.split()]

            # Initial size of the system
            current_system_size = 4

            # Split the loop lengths from data into out_dict
            for loop_length in data:
                if current_system_size not in out_dict:
                    out_dict[current_system_size] = []

                # Add the data to the appropriate system_size
                out_dict[current_system_size].append(loop_length)
                current_system_size *= 2

    return out_dict

def clean_processed_data(data_dict, exclude_system_size):
    """Modify data_dict to remove the each <system_linear_size> in exclude_system_size

    :data_dict: dict - {<system_linear_size>: [<max_loop_length00>, <max_loop_length10>, ...], ...}
    :exclude_system_size: 1xn array - System linear size to remove from data_dict. e.g [4, 8, ...]
    :returns: None
    """
    for system_linear_size in exclude_system_size:
        data_dict.pop(system_linear_size)

def plot_syssize_vs_looplength(data_dict):
    """Plot system size (key) vs loop lengths (values) from data_dict

    :data_dict: dict
    :returns: None
    """
    colors = ["#966842", "#f44747", "#eedc31", "#7fdb6a", "#0e68ce"]
    for system_linear_size in data_dict:
        color_counter = 0
        # Set labels
        plt.title("Maximum loop length on a 2D Ising lattice")
        plt.xlabel("Linear system size")
        plt.ylabel("Loop length")

        plt.scatter([system_linear_size]*len(data_dict[system_linear_size]),\
                data_dict[system_linear_size],\
                c="#000000", s=10)

        # New color every box size
        color_counter += 1
        color_counter %= len(colors)

    plt.xticks(list(data_dict.keys()),\
            list(data_dict.keys()),)

def fit_function(x, a, b, c):
    """Function to fit against
    y = a * x^(b) + c

    :x: Float - Known data
    :a: Float - Unknown data
    :b: Float - Unknown data
    :c: Float - Unknown data
    :returns: Float
    """
    return a * np.power(x, b) + c

def plot_syssize_vs_fit(data_dict):
    """Fit against the function loop_length = system_size^(D_H)

    :data_dict: dict
    :returns: None
    """

    system_sizes = []
    loop_lengths = []
    for system_size in data_dict:
        # Ensure there are as many system_size points as loop_lengths
        ss = [system_size]*len(data_dict[system_size])
        for size in ss:
            system_sizes.append(size)

        for ll in data_dict[system_size]:
            loop_lengths.append(ll)

    # xdata
    system_sizes = np.array(system_sizes)
    # ydata
    loop_lengths = np.array(loop_lengths)

    opt_parameters, _pcov = curve_fit(fit_function, system_sizes, loop_lengths)

    # Used for plotting against the fitted function
    xdata = np.linspace(min(system_sizes), max(system_sizes), 50)
    # colors = ["#966842", "#f44747", "#eedc31", "#7fdb6a", "#0e68ce"]
    plt.loglog(xdata, fit_function(xdata, *opt_parameters), c="#0e68ce", label=fr"$\propto L^{{ {opt_parameters[1]:.4f} }}$")

    print(f"D_H = {opt_parameters[1]:.6}")

if __name__ == '__main__':
    simulation_data = process_file("./data/loop_lengths128x128.txt")

    clean_processed_data(simulation_data, exclude_system_size=[4, 8])

    plot_syssize_vs_looplength(simulation_data)

    plot_syssize_vs_fit(simulation_data)

    plt.legend()

    savefig=False
    if savefig:
        plt.savefig(f"./plots/maximum_loop_length_for_2D_Ising.png",\
                    bbox_inches='tight')
    plt.show()
