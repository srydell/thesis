'''
Brief:               A library for helping with some calculations

File name:           calc.py
Author:              Simon Rydell
Python Version:      3.7
'''

import re
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rc
from mpl_toolkits.axes_grid1.inset_locator import zoomed_inset_axes
from mpl_toolkits.axes_grid1.inset_locator import mark_inset
from mpl_toolkits.axes_grid1.inset_locator import InsetPosition

rc('font', **{'family': 'serif', 'serif': ['DejaVu Sans']})
rc('text', usetex=True)

def process_file(filename, key, xy):
    """Go through filename and split it into arrays
       NOTE: process_file assumes that both the data from key and xy are floats
       Assumes that the file is organized as:

       regex_match(key)
       regex_match(xy)
       regex_match(xy)
       ...

    :filename: String - Valid path to a file containing the abow
    :key: Regex String - Matching keys for the out_dict
    :xy: Regex String - Matching data for the out_dict
    :returns: dict - {<reg_group(1, key)>:
                      [[<reg_group(1, xy)>, ...], [<reg_group(2, xy)>, ...], ...], ...}
    """
    out_dict = {}
    with open(filename) as data_file:
        # Keep track of the key we are on
        current_key = 0
        num_xy_groups = len(re.findall(r'\(.*?\)', xy))

        # print(f"Found the number of groups: {num_xy_groups}")

        for line in data_file.readlines():
            # Check if line is describing system size or box sizes

            # print(f"Examining line: {line}")

            # Check key
            key_match = re.match(key, line)
            # If it matched
            if key_match:
                key_data = float(key_match.group(1))

                # print(f"This is a line with a system size: {key_data}")

                if key_data not in out_dict:

                    # print(f"Key data: {key_data} has not been seen before. Adding new.")

                    # Create a new array for this key data
                    out_dict[key_data] = [[] for i in range(num_xy_groups)]

                current_key = key_data
                # No need to process further, we found a match
                continue

            # Check data
            xy_match = re.match(xy, line)
            if xy_match:
                for i in range(num_xy_groups):
                    x = float(xy_match.group(i + 1))
                    # Save the data under the current system size
                    out_dict[current_key][i].append(x)

                    # print(f"Found the number: {x}")
                # input()

    return out_dict

def plot_errorbars(data_dict, label, color="#202020"):
    """Plot Monte Carlo style error bars where yerr=std(y)/sqrt(number_measurements)

    :data_dict: dict - Assumed to be {x0: [avg(y0), std(y0), num_measurements(y0)], x1: ...}
    :label: string - label for the plot
    :color: string - color for the plot
    :returns: None
    """
    # To only label once
    labeled = False
    for x in data_dict:

        # NOTE: Naive. Breaks on "axe", "flex", ...
        if 'x' in label:
            label = label.format(x=x)

        avg_y = data_dict[x][0]
        std_y = data_dict[x][1]
        num_measurements_y = data_dict[x][2]

        if not labeled:
            plt.errorbar(x, avg_y, yerr=std_y/np.sqrt(num_measurements_y),\
                ecolor='gray', elinewidth=2, fmt=f'{color}', marker='.',\
                linestyle="None", capsize=3, capthick=2, label=rf"{label}")
            labeled = True
        else:
            plt.errorbar(x, avg_y, yerr=std_y/np.sqrt(num_measurements_y),\
                ecolor='gray', elinewidth=2, fmt=f'{color}', marker='.',\
                linestyle="None", capsize=3, capthick=2)

def add_std(std):
    """Calculate the standard deviation of a list of standard deviations

    :std: 1xn Array - [std0, std1, ...]
    :returns: Float - Square root of the average variance
    """

    variance = [np.power(s, 2) for s in std]
    return np.sqrt(sum(variance)/len(variance))

def add_mean(mean):
    """Calculate the mean of a list of means

    :mean: 1xn Array - [mean0, mean1, ...]
    :returns: Float - sum(mean) / len(mean)
    """
    return sum(mean)/len(mean)
