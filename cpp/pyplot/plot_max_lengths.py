'''
Brief:               Generic plot sizes

File name:           plot_sizes.py
Author:              Simon Rydell
Date created:        Aug 20, 2018
Date last modified:  Aug 20, 2018
Python Version:      3.7
'''

import numpy as np
import random
import matplotlib.pyplot as plt
from matplotlib import rc
from scipy.optimize import curve_fit
import helpers.illustrations as illu
import helpers.calc as calc
import helpers.constants as const

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
                c=const.COLOR_MAP["blue"], s=10)

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

def correct_function(x, a, b):
    """Function to fit against
    y = a * x^(1.375) + b

    :x: Float - Known data
    :a: Float - Unknown data
    :b: Float - Unknown data
    :returns: Float
    """
    return a * np.power(x, 1.375) + b

def wrong_function(x, a, b):
    """Function to fit against
    y = a * x^(2) + b

    :x: Float - Known data
    :a: Float - Unknown data
    :b: Float - Unknown data
    :returns: Float
    """
    return a * np.power(x, 2) + b

def bootstrap(fit_func, xdata, ydata, iterations=100):
    """Perform bootstrap resampling and get the average and standard deviation from the optimal parameters

    :fit_func: Function Object - To fit against
    :xdata: 1xn array
    :ydata: 1xn array
    :returns: dict of size m - {average: std, ...}, m is the number of parameters to fit against in fit_func
    """
    opt_parameters, _ = curve_fit(fit_func, xdata, ydata)
    # NOTE: Do not use [[]]*len(...) since this creates three references to the same []
    samples = [[] for i in range(len(opt_parameters))]

    new_xdata = [0 for i in range(len(xdata))]
    new_ydata = [0 for i in range(len(ydata))]

    for _ in range(iterations):
        # Resample
        for i in range(len(ydata)):
            index_choice = random.randrange(0, len(xdata))
            new_xdata[i] = xdata[index_choice]
            new_ydata[i] = ydata[index_choice]

        # Curve fit and store the samples
        sampled_pars, _spcov = curve_fit(fit_func, new_xdata, new_ydata)

        for i, sp in enumerate(sampled_pars):
            samples[i].append(sp)

    return {np.average(sample): np.std(sample) for sample in samples}

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

    stderr_pars = bootstrap(fit_function, system_sizes, loop_lengths, 100)
    dh_error = stderr_pars[list(stderr_pars.keys())[1]]

    opt_parameters, _pcov = curve_fit(fit_function, system_sizes, loop_lengths)
    correct_opt_parameters, _pcov = curve_fit(correct_function, system_sizes, loop_lengths)
    wrong_opt_parameters, _pcov = curve_fit(wrong_function, system_sizes, loop_lengths)

    # Used for plotting against the fitted function
    xdata = np.linspace(min(system_sizes), max(system_sizes), 50)
    # colors = ["#966842", "#f44747", "#eedc31", "#7fdb6a", "#0e68ce"]
    plt.loglog(xdata, fit_function(xdata, *opt_parameters),
               c=const.COLOR_MAP["blue"],
               label=fr"$\propto L^{{ {opt_parameters[1]:.3f} \pm {dh_error:.2f} }}$")
    plt.loglog(xdata, correct_function(xdata, *correct_opt_parameters),
               c=const.COLOR_MAP["green"],
               label=r"$\propto L^{1.375}$")
    plt.loglog(xdata, wrong_function(xdata, *wrong_opt_parameters),
               c=const.COLOR_MAP["red"],
               label=r"$\propto L^{2}$")

    print(f"D_H = {opt_parameters[1]:.6} +- {dh_error:.2f}")

if __name__ == '__main__':
    simulation_data = process_file("./data/loop_lengths128x128.txt")

    clean_processed_data(simulation_data, exclude_system_size=[])

    # plot_syssize_vs_looplength(simulation_data)

    errorbar_data = {}
    for size in simulation_data:
        errorbar_data[size] = [np.average(simulation_data[size]),
                               np.std(simulation_data[size]),
                               len(simulation_data[size])]

    calc.plot_errorbars(errorbar_data,
                        # label=r"$\bar{D}_H \pm \sigma_{\bar{D}_H}$",
                        color=const.COLOR_MAP["black"])

    plot_syssize_vs_fit(simulation_data)

    plt.title("Maximum loop length on a 2D Ising lattice")
    plt.xlabel("Linear system size")
    plt.ylabel("Loop length")
    plt.legend()

    savefig=False
    if savefig:
        illu.save_figure(f"maximum_loop_length_for_2D_Ising")
    plt.show()
