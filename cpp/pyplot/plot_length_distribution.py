'''
Brief:               Plot length distribution

File name:           plot_sizes.py
Author:              Simon Rydell
Date created:        Aug 30, 2018
Python Version:      3.7
'''

import re
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rc

rc('font', **{'family': 'serif', 'serif': ['DejaVu Sans']})
rc('text', usetex=True)

def process_file(filename):
    """Go through filename and split it into arrays
       Assumes that the file is organized as:

       L=<system_linear_size>:
       <worm_size0> <worm_size1> ...
       ...

    :filename: String - Valid path to a file containing the abow
    :returns: dict - {<system_linear_size>:
                      [<worm_size0>, <worm_size1>, ...], ...}
    """
    out_dict = {}
    with open(filename) as data_file:
        # Keep track of the system size we are on
        current_system_size = 0
        for line in data_file.readlines():
            # Check if line is describing system size or box sizes

            # Check system size
            # Match 'L=<one or more digits>:'
            system_size_match = re.match(r"L=(\d+):", line)
            # If it matched
            if system_size_match:
                system_linear_size = float(system_size_match.group(1))
                if system_linear_size not in out_dict:
                    # Create a new array for this system size
                    out_dict[system_linear_size] = []

                current_system_size = system_linear_size
                # No need to process further, we found a match
                continue

            # Check if there are loop sizes
            # At least one loop size
            loop_size_match = re.match(r"(\d+)", line)
            if loop_size_match:
                loops = [float(l) for l in line.split()]
                # Save the data under the current system size
                for l in loops:
                    out_dict[current_system_size].append(l)
    return out_dict

def clean_processed_data(data_dict, size_of_interest, exclude_first):
    """Modify data_dict to remove the data points where
       <system_linear_size> != size_of_interest
       <worm_length> < exclude_first

    :data_dict: dict - {<system_linear_size>:
                       [<worm_length0>, <worm_length0>, ...], ...}
    :size_of_interest: Int - Remove every system_linear_size except this one
    :returns: None
    """
    sizes_to_remove = [size for size in data_dict if size != size_of_interest]
    for size in sizes_to_remove:
        data_dict.pop(size)

    data_dict[size_of_interest] = [wl for wl in data_dict[size_of_interest] if wl > exclude_first]

def plot_setup():
    """Set axis labels and such
    :returns: TODO
    """
    plt.xlabel("Loop size")
    plt.ylabel("Counts")
    plt.title(rf"Worm distribution for ${128}^2$ Ising lattice")

def plot_histogram(data_dict, bin_size=16):
    """Plot histogram over worm distribution

    :data_dict: dict
    :bin_size: int
    :returns: None
    """
    for system_linear_size in data_dict:
        bins = []
        for i in range(0, int(max(data_dict[system_linear_size])), bin_size):
            bins.append(i)
        plt.hist(data_dict[system_linear_size], log=True, bins=bins, label="Loop distribution")

def plot_line(system_size, dimension, height):
    """Plot a line at where no loop lengths can be longer than

    :system_size: int
    :dimension: int
    :returns: None
    """
    num_sites = pow(system_size, dimension)
    plt.plot([num_sites, num_sites], [0, height])

if __name__ == '__main__':
    simulation_data = process_file("./data/worm_distribution.txt")

    size_to_plot = 128
    clean_processed_data(simulation_data, size_to_plot, exclude_first=150)

    plot_setup()
    plot_histogram(simulation_data, bin_size=16)
    # plot_line(size_to_plot, dimension=2,\
    #         height=simulation_data[size_to_plot].count(min(simulation_data[size_to_plot])))
    plt.show()
