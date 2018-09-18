'''
Brief:               Plot susceptibility

File name:           plot_sizes.py
Author:              Simon Rydell
Date created:        Aug 24, 2018
Date last modified:  Aug 25, 2018
Python Version:      3.7
'''

import random
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rc
from scipy.optimize import curve_fit

rc('font', **{'family': 'serif', 'serif': ['DejaVu Sans']})
rc('text', usetex=True)

def process_file(filename):
    """Go through filename and split it into arrays
       Assumes that the file is organized as:

       <susceptibility00> <max_loop_length01> ...
       <susceptibility10> <max_loop_length11> ...
       ...

       Where the system_linear_size is 2, 4, 8, ... on each line

    :filename: String - Valid path to a file containing the abow
    :returns: dict - {<system_linear_size0>:
                      [<susceptibility00>, <max_loop_length10, ...], ...}
    """
    out_dict = {}
    with open(filename) as data_file:
        for line in data_file.readlines():
            # Split the data into an array
            data = [float(d) for d in line.split()]

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

    :data_dict: dict - {<system_linear_size>: [<susceptibility00>, <susceptibility01>, ...], ...}
    :exclude_system_size: 1xn array - System linear size to remove from data_dict. e.g [4, 8, ...]
    :returns: None
    """
    for system_linear_size in exclude_system_size:
        data_dict.pop(system_linear_size)

def plot_hist_of_error(data_dict):
    """Plot histogram for each system_size in data_dict

    :data_dict: dict
    :returns: None
    """
    for system_size in data_dict:
        plt.hist(data_dict[system_size])
        plt.title(rf"Error distribution for ${system_size}^2$ Ising lattice")
        plt.xlabel(r"$\chi$")
        plt.ylabel("Number of counts")
        plt.show()
        # Clear figure
        plt.clf()

def plot_syssize_vs_susc(data_dict):
    """Plot system size (key) vs loop lengths (values) from data_dict

    :data_dict: dict
    :returns: None
    """
    colors = ["#966842", "#f44747", "#eedc31", "#7fdb6a", "#0e68ce"]
    for system_linear_size in data_dict:
        color_counter = 0

        plt.scatter([system_linear_size]*len(data_dict[system_linear_size]),\
                data_dict[system_linear_size],\
                c="#000000", s=10)

        # New color every box size
        color_counter += 1
        color_counter %= len(colors)

    plt.xticks(list(data_dict.keys()),\
            list(data_dict.keys()),)

def fit_function(x, a, eta):
    """Function to fit against
    y = a * x^(2 - eta) + c

    :x: Float - Known data
    :a: Float - Unknown data
    :eta: Float - Unknown data
    :returns: Float
    """
    return a * np.power(x, 2 - eta)

def plot_setup():
    """Set titles, {x,y}labels etc.
    :returns: None
    """
    # Set labels
    plt.title("Susceptibility on a 2D Ising lattice")
    plt.xlabel("Linear system size")
    plt.ylabel(r"$\chi$")

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
    """Fit against the function fit_function(x, *opt_parameters)
       Use bootstrap to calculate the error

    :data_dict: dict
    :returns: None
    """

    system_sizes = []
    susceptibility = []
    # avg_syssize = []
    # avg_susc = []
    for system_size in data_dict:
        # if system_size not in avg_syssize:
        #     avg_syssize.append(system_size)
        #     avg_susc.append(np.average(data_dict[system_size]))

        # Ensure there are as many system_size points as susceptibility
        ss = [system_size]*len(data_dict[system_size])
        for size in ss:
            system_sizes.append(size)

        for susc in data_dict[system_size]:
            susceptibility.append(susc)

    # xdata
    system_sizes = np.array(system_sizes)
    # ydata
    susceptibility = np.array(susceptibility)

    stderr_pars = bootstrap(fit_function, system_sizes, susceptibility, 100)
    opt_pars = list(stderr_pars.keys())

    # Used for plotting against the fitted function
    xdata = np.linspace(min(system_sizes), max(system_sizes), 50)
    # colors = ["#966842", "#f44747", "#eedc31", "#7fdb6a", "#0e68ce"]
    plt.loglog(xdata, fit_function(xdata, *opt_pars),\
            c="#0e68ce", label=fr"$\propto L^{{ 2 - {opt_pars[1]:.2f} \pm {stderr_pars[opt_pars[1]]:.3f}  }}$")
    plt.loglog(xdata, fit_function(xdata, *[0.51, .25]),\
            c="#7fdb6a", label=r"$\propto L^{{ 2 - \eta_{Ising} }}$")
    plt.loglog(xdata, fit_function(xdata, *[0.35, 0]),\
            c="#f44747", label=r"$\propto L^{{ 2 }}$")

    print(f"eta = {opt_pars[1]:.6} +- {stderr_pars[opt_pars[1]]:.4}")

if __name__ == '__main__':
    simulation_data = process_file("./data/susceptibility.txt")
    clean_processed_data(simulation_data, exclude_system_size=[])

    # plot_hist_of_error(simulation_data)

    plot_setup()
    plot_syssize_vs_susc(simulation_data)
    plot_syssize_vs_fit(simulation_data)

    plt.legend()

    savefig = False
    if savefig:
        plt.savefig(f"./plots/susceptibility128x128Ising.png",\
                    bbox_inches='tight')
    plt.show()
