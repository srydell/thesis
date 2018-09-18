'''
Brief:               Generic plot sizes

File name:           plot_sizes.py
Author:              Simon Rydell
Date created:        Aug 20, 2018
Date last modified:  Aug 20, 2018
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

def clean_processed_data(data_dict, size_of_interest, exclude_rel_size=[]):
    """Modify data_dict to remove the data points where
       <system_linear_size> != size_of_interest
       <box_linear_size> == <system_linear_size> * exclude_rel_size

    :data_dict: dict - {<system_linear_size>:
                       [[<box_linear_size>, <number_occupied_boxes>], ...], ...}
    :size_of_interest: Int - Remove every system_linear_size except this one
    :exclude_rel_size: 1xn array - Relative box sizes to remove. e.g [1, 1/2, 1/4, ...]
    :returns: None
    """
    sizes_to_remove = [size for size in data_dict if size != size_of_interest]
    for size in sizes_to_remove:
        data_dict.pop(size)

    # Ensure that the box containing the whole system is never in the data
    # Since x / log(1) is undefined for all x
    if 1 not in exclude_rel_size:
        exclude_rel_size.append(1)

    for system_linear_size in data_dict:
        for relsize in exclude_rel_size:
            # Remove all boxes with relative size relsize
            data_dict[system_linear_size] =\
                     [blsize_numocc for blsize_numocc in data_dict[system_linear_size]\
                                    if blsize_numocc[0] != system_linear_size * relsize]

def calc_hausdorff_dimension(data_dict):
    """Modify data_dict to contain hausdorff dimension
       Assumed input format of data_dict:
           {<system_linear_size>: [[<box_linear_size>, <number_occupied_boxes>], ...]}
       After modifying:
           {<system_linear_size>: [[<box_relative_size>, <hausdorff_dimension>], ...]}

       NOTE: hausdorff_dim = log(number_occupied_boxes) / log(1 / box_relative_size)
             Where box_relative_size is the relative size to the system_linear_size
                => box_relative_size = box_linear_size / system_linear_size

    :data_dict: dict
    :returns: None
    """
    for system_linear_size in data_dict:
        # Will replace data_dict[system_linear_size]
        box_and_haus_dims = []
        # size_and_occ = [box_linear_size, number_occupied_boxes]
        for size_and_occ in data_dict[system_linear_size]:
            number_occupied_boxes = size_and_occ[1]
            box_relative_size = size_and_occ[0] / system_linear_size

            hausdorff_dim = np.log(number_occupied_boxes) / np.log(1 / box_relative_size)

            box_and_haus_dims.append([box_relative_size, hausdorff_dim])

        data_dict[system_linear_size] = box_and_haus_dims

def plot_error_bars(data_dict):
    """Plot error bars from data_dict[system_linear_size]

    :data_dict: dict -
           {<system_linear_size>: [[<box_relative_size>, <hausdorff_dimension>], ...]}
    :returns: None
    """
    # NOTE: There should only be one system_linear_size here
    for system_linear_size in data_dict:
        # {box_relative_size: [D_H0, D_H1, ...], ...}
        brs_dhs = {}
        for brs_and_dh in data_dict[system_linear_size]:
            # If new relative box size
            if brs_and_dh[0] not in brs_dhs:
                brs_dhs[brs_and_dh[0]] = []

            # Append the D_H to the correct relative box size
            brs_dhs[brs_and_dh[0]].append(brs_and_dh[1])

        min_rel_box_size = min(brs_dhs.keys())
        average_dh = []
        std_average = []
        for rel_box in brs_dhs:
            # NOTE: The best approximation in the box model is always the smallest box
            if rel_box == min_rel_box_size:
                best_avg_hdim = np.average(brs_dhs[rel_box])
                best_std_hdim = np.std(brs_dhs[rel_box])

            average_dh.append(np.average(brs_dhs[rel_box]))

            # Calculate the standard deviation
            # std_dev = sqrt(((sample1 - average)^2 + ... + (sampleN - average)^2)/N)
            std_average.append(np.std(brs_dhs[rel_box]) / np.sqrt(len(data_dict[system_linear_size])))

        best_std_hdim = best_std_hdim / len(data_dict[system_linear_size])
        print(f"D_H = {best_avg_hdim:.5} +- {best_std_hdim:.2}")

        plt.errorbar(list(brs_dhs.keys()), average_dh, yerr=std_average,\
            ecolor='gray', elinewidth=2, fmt='k.', linestyle="None",\
            capsize=3, capthick=2, label=r"$\bar D_H \pm \frac{\sigma_{D_H}}{\sqrt{N}}$")

def plot_boxsize_vs_hausdorff(data_dict, plot_scatter=False, savefig=False):
    """Plot data from data_dict[system_linear_size]

    :data_dict: dict -
           {<system_linear_size>: [[<box_relative_size>, <hausdorff_dimension>], ...]}
    :plot_scatter: Bool - if True, plot all data points with plt.scatter
    :savefig: Bool - if True, save figures to ./plots
    :returns: None
    """
    colors = ["#966842", "#f44747", "#eedc31", "#7fdb6a", "#0e68ce"]
    color_counter = 0

    # NOTE: There should only be one system_linear_size here
    for system_linear_size in data_dict:
        # Used for xticks
        found_box_sizes = []
        for boxsize_and_hdim in data_dict[system_linear_size]:
            if not boxsize_and_hdim[0] in found_box_sizes:
                found_box_sizes.append(boxsize_and_hdim[0])

            if plot_scatter:
                plt.scatter(boxsize_and_hdim[0], boxsize_and_hdim[1], c="#555555", s=2)

            # New color every box size
            # color_counter += 1
            # color_counter %= len(colors)

        # Set labels
        plt.title(r"Box dimension calculation on a ${}^2$ Ising lattice"\
                .format(int(system_linear_size)))
        plt.xlabel("Relative size of box division")
        plt.ylabel(r"$D_H$")

        # labels = [r"$\frac{1}{4}$", ...]
        labels = [bsize.as_integer_ratio() for bsize in found_box_sizes]
        labels = [r"$\frac{" + "{}".format(l[0]) + r"}{" + "{}".format(l[1]) + r"}$"\
                 for l in labels]

        plt.xticks(found_box_sizes,\
                labels,\
                fontsize=14)

        plt.legend()

        if savefig:
            sls = system_linear_size
            plt.savefig(f"./plots/box_dimension_for_{int(sls)}x{int(sls)}Ising.png",\
                        bbox_inches='tight')
        plt.show()

if __name__ == '__main__':
    simulation_data = process_file("./data/box_size128x128.txt")

    size_to_plot = 128
    clean_processed_data(simulation_data, size_to_plot, exclude_rel_size=[1/2, 1/4])

    calc_hausdorff_dimension(simulation_data)

    plot_error_bars(simulation_data)

    # plt.xlim([1/64 - 1/64 * .2, 1/8 + 1/64 * .2])
    plt.plot([1/64 - 1/64 * .2, 1/8 + 1/64 * .2], [1.375, 1.375], linewidth=1, c="#0e68ce", label=r"D_H = 1.375")

    # plt.show() called here
    plot_boxsize_vs_hausdorff(simulation_data, savefig=True)
