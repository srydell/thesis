'''
Brief:               Generic plot sizes

File name:           plot_sizes.py
Author:              Simon Rydell
Date created:        Aug 20, 2018
Date last modified:  Aug 20, 2018
Python Version:      3.7
'''

import re
from math import log
import matplotlib.pyplot as plt
from matplotlib import rc

rc('font', **{'family': 'serif', 'serif': ['DejaVu Sans']})
rc('text', usetex=True)

def process_file(filename):
    """Go through filename and split it into arrays
       Assumes that the file is organized as:

       L=<system_linear_size>:
       <box_linear_size> <number_occupied_boxes>
       ...

    :filename: String - Valid path to a file containing the abow
    :returns: dict - {<system_linear_size>:
                      [[<box_linear_size>, <number_occupied_boxes>], ...], ...}
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

            # Check box size
            box_size_match = re.match(r"(\d+) (\d+)", line)
            if box_size_match:
                box_linear_size = float(box_size_match.group(1))
                number_occupied_boxes = float(box_size_match.group(2))
                # Save the data under the current system size
                out_dict[current_system_size].append([box_linear_size, number_occupied_boxes])
    return out_dict

def clean_processed_data(data_dict, exclude_rel_size):
    """Modify data_dict to remove the data points where <box_linear_size> == <system_linear_size> or <box_linear_size> == <system_linear_size> / 2

    :data_dict: dict - {<system_linear_size>:
                       [[<box_linear_size>, <number_occupied_boxes>], ...], ...}
    :exclude_rel_size: 1xn array - Relative box sizes to remove. e.g [1, 1/2, 1/4, ...]
    :returns: None
    """
    for system_linear_size in data_dict:
        for relsize in exclude_rel_size:
            # Remove all boxes with relative size relsize
            data_dict[system_linear_size] = [blsize_numocc for blsize_numocc in data_dict[system_linear_size]\
                                             if blsize_numocc[0] != system_linear_size * relsize]

def calc_hausdorff_dimension(data_dict):
    """Modify data_dict to contain hausdorff dimension
       Assumed input format of data_dict:
           {<system_linear_size>: [[<box_linear_size>, <number_occupied_boxes>], ...], ...}
       After modifying:
           {<system_linear_size>: [[<box_relative_size>, <hausdorff_dimension>], ...], ...}

       NOTE: hausdorff_dim = log(number_occupied_boxes) / log(1 / box_relative_size)
             Where box_relative_size is the relative size to the system_linear_size
                => box_relative_size = box_linear_size / system_linear_size

    :data_dict: dict
    :returns: None
    """
    for system_linear_size in data_dict:
        # Will replace data_dict[system_linear_size]
        haus_dims = []
        # size_and_occ = [box_linear_size, number_occupied_boxes]
        for size_and_occ in data_dict[system_linear_size]:
            number_occupied_boxes = size_and_occ[1]
            box_relative_size = size_and_occ[0] / system_linear_size

            hausdorff_dim = log(number_occupied_boxes) / log(1 / box_relative_size)

            haus_dims.append([box_relative_size, hausdorff_dim])

        data_dict[system_linear_size] = haus_dims

def plot_boxsize_vs_hausdorff(data_dict, savefig=False):
    """Create one plot for each key in data_dict

    :data_dict: dict
    :savefig: Bool - if True, save figures to ./plots
    :returns: None
    """
    colors = ["#966842", "#f44747", "#eedc31", "#7fdb6a", "#0e68ce"]
    for system_linear_size in data_dict:
        color_counter = 0
        # Used for xticks
        found_box_sizes = []
        for boxsize_and_hdim in data_dict[system_linear_size]:
            if not boxsize_and_hdim[0] in found_box_sizes:
                found_box_sizes.append(boxsize_and_hdim[0])

            # Set labels
            plt.title(r"Box dimension calculation on a ${}^3$ Ising lattice"\
                    .format(int(system_linear_size)))
            plt.xlabel("Relative size of box division")
            plt.ylabel(r"$D_H$")

            plt.scatter(boxsize_and_hdim[0], boxsize_and_hdim[1], c="#000000", s=10)

            # New color every box size
            color_counter += 1
            color_counter %= len(colors)

        # labels = [r"$\frac{1}{4}$", ...]
        labels = [bsize.as_integer_ratio() for bsize in found_box_sizes]
        labels = [r"$\frac{" + "{}".format(l[0]) + r"}{" + "{}".format(l[1]) + r"}$" for l in labels]

        plt.xticks(found_box_sizes,\
                labels,\
                fontsize=14)

        if system_linear_size == 128:
            # pass
            plt.show()
        if savefig:
            sls = system_linear_size
            plt.savefig(f"./plots/box_dimension_for_{sls}x{sls}x{sls}Ising.png",\
                        bbox_inches='tight')

if __name__ == '__main__':
    simulation_data = process_file("./data/box_size128x128x128.txt")

    # NOTE: exclude_rel_size must at least contain 1 since x / log(1) is undefined
    clean_processed_data(simulation_data, exclude_rel_size=[1, 1/2])

    calc_hausdorff_dimension(simulation_data)

    plot_boxsize_vs_hausdorff(simulation_data)
