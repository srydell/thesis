'''
Brief:               A library for helping with some calculations

File name:           calc.py
Author:              Simon Rydell
Python Version:      3.7
'''

import re
import numpy as np
import matplotlib.pyplot as plt

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
    :xy: Regex String - Matching x and y data for the out_dict
    :returns: dict - {<reg_group(1, key)>:
                      [[<reg_group(1, xy)>, ...], [<reg_group(2, xy)>, ...]], ...}
    """
    out_dict = {}
    with open(filename) as data_file:
        # Keep track of the key we are on
        current_key = 0
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
                    out_dict[key_data] = [[], []]

                current_key = key_data
                # No need to process further, we found a match
                continue

            # Check data
            xy_match = re.match(xy, line)
            if xy_match:
                x = float(xy_match.group(1))
                y = float(xy_match.group(2))
                # Save the data under the current system size
                out_dict[current_key][0].append(y)
                out_dict[current_key][1].append(x)

                # print(f"Found winding number: {x}")
                # print(f"Found y: {y}")
            # input()

    return out_dict

def plot_errorbars(data_dict, labels, colors=None):
    """Plot error bars using numpy.std() and numpy.mean() according to input bools
       Let xi = [x0, x1, ...]
       Let yi = [y0, y1, ...]

    :data_dict: dict - Assumed to be {x0: [[y0, y1, ...]], x1: ...}
    :labels: array of strings or a single string - All labels for the plots
    :colors: array of strings - All colors for the plots
    :returns: None
    """
    if colors is None:
        colors = ["#966842", "#f44747", "#eedc31", "#7fdb6a", "#0e68ce"]

    for x in data_dict:

        c_id += 1
        color = colors[c_id % len(colors)]
        if type(labels) == str:
            label = labels.format(x = x)
        else:
            l = labels[0]
            labels = labels[1:]

        y = data_dict[x]
        plt.errorbar(x, np.mean(y), yerr=np.std(y)/len(y),\
            ecolor='gray', elinewidth=2, fmt=f'{color}', linestyle="None",\
            capsize=3, capthick=2, label=rf"{label}")
