'''
Brief:               A library for helping with some calculations

File name:           calc.py
Author:              Simon Rydell
Python Version:      3.7
'''

import re
import numpy as np
import random
import matplotlib.pyplot as plt
from matplotlib import rc
from scipy.optimize import curve_fit
from mpl_toolkits.axes_grid1.inset_locator import zoomed_inset_axes
from mpl_toolkits.axes_grid1.inset_locator import mark_inset
from mpl_toolkits.axes_grid1.inset_locator import InsetPosition

rc('font', **{'family': 'serif', 'serif': ['DejaVu Sans']})
rc('text', usetex=True)

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

                    if current_key not in out_dict:
                        out_dict[current_key] = [[] for i in range(num_xy_groups)]

                    # Save the data under the current system size
                    out_dict[current_key][i].append(x)

                    # print(f"Found the number: {x}")
                # input()

    return out_dict

def plot_errorbars(data_dict, label=None, color="#202020", axis=None):
    """Plot Monte Carlo style error bars where yerr=std(y)/sqrt(number_measurements)

    :data_dict: dict - Assumed to be {x0: [avg(y0), std(y0), num_measurements(y0)], x1: ...}
    :label: string - label for the plot
    :color: string - color for the plot
    :axis: Plot Object - Optional axis to plot on
    :returns: None
    """
    # To only label once
    labeled = False if label is not None else True
    # print(label)
    # print(labeled)
    for x in data_dict:

        # NOTE: Naive. Breaks on "axe", "flex", ...
        if label is not None:
            if 'x' in label:
                label = label.format(x=x)

        avg_y = data_dict[x][0]
        std_y = data_dict[x][1]
        num_measurements_y = data_dict[x][2]

        if axis is None:
            axis = plt

        if not labeled:
            axis.errorbar(x, avg_y, yerr=std_y/np.sqrt(num_measurements_y),\
                ecolor='gray', elinewidth=2, fmt=f'{color}', marker='.',\
                linestyle="None", capsize=3, capthick=2, label=rf"{label}")
            labeled = True
        else:
            axis.errorbar(x, avg_y, yerr=std_y/np.sqrt(num_measurements_y),\
                ecolor='gray', elinewidth=2, fmt=f'{color}', marker='.',\
                linestyle="None", capsize=3, capthick=2)

def plot_zoomed_inset(data, zoomed_size, axis, colors, plot_errorbar=None, ticks=None, inset_position=None):
    """Use a subplot to show an inset

    :data: dict - {key0: [x0, y0], ...} will be called plot() upon each key
    :zoomed_size: 1x5 array - [xlim0, xlim1, ylim0, ylim1, zoom_factor], where to zoom in and how much
    :axis: Plot Object - The original axis to plot the insert in
    :colors: 1xn array - Strings of hex values, will be used on data and plot_errorbar
    :plot_errorbar: 1x2 array - (OPTIONAL) [plot_dict, colors],
                                will be called plot_errorbars upon each key
    :plot_errorbar: 1x2 array - (OPTIONAL) [plot_dict, colors],
                                will be called plot_errorbars upon each key
    :ticks: 1x2 array - (OPTIONAL) [xticks, yticks],
                        Set ticks, if None, hide all ticks
    :inset_position: 1x2 array - (OPTIONAL) [relpos_lowerleftcorner_x, relpos_lowerleftcorner_y,
                                 rel_width, rel_height],
                                 Position of the inset, if None, loc=2
    :returns: Plot Object - Inset axis
    """
    # Get the axis
    # Parameters: [original_axis, zoom_factor, location]
    zoomed_axis = zoomed_inset_axes(axis, zoomed_size[-1], loc=2)

    # If got an inset_position override default position
    if inset_position is not None:
        ip = InsetPosition(axis, inset_position)
        zoomed_axis.set_axes_locator(ip)

    # data is plotted using ax.plot
    if data:
        c_id = -1
        for key in data:
            c_id += 1
            c = colors[c_id % len(colors)]
            zoomed_axis.plot(data[key][0], data[key][1], c=c, linewidth=1)

    # plot_errorbar is plotted using the function plot_errorbars
    if plot_errorbar is not None:
        c_id = 0
        error_data = plot_errorbar
        for key in error_data:
            c_id += 1
            c = colors[c_id % len(colors)]
            plot_errorbars(error_data[key], color=c, axis=zoomed_axis)

    # Apply limits
    zoomed_axis.set_xlim(zoomed_size[0], zoomed_size[1])
    zoomed_axis.set_ylim(zoomed_size[2], zoomed_size[3])

    # Default to no ticks on the inset
    if ticks is None:
        plt.yticks(visible=False)
        plt.xticks(visible=False)
    else:
        zoomed_axis.set_xticks(ticks[0])
        zoomed_axis.set_yticks(ticks[1])

    # This draws the lines between the inset and the zoomed in box of the original axis
    mark_inset(axis, zoomed_axis, loc1=3, loc2=1, fc="none", ec="0.5")
    return zoomed_axis

def get_intersection(l0, l1):
    """Return intersection between line l0 and line l1

    :l0: 1x4 array of numbers - [x0, x1, y0, y1]
    :l1: 1x4 array of numbers - [x2, x3, y2, y3]
    :returns: 1x2 array of numbers - [x_intersect, y_intersect
    """
    # Source: https://en.wikipedia.org/wiki/Line–line_intersection

    denominator = (l0[0] - l0[1]) * (l1[2] - l1[3]) -\
                  (l0[2] - l0[3]) * (l1[0] - l1[1])

    x_nominator = (l0[0] * l0[3] - l0[2] * l0[1]) * (l1[0] - l1[1]) -\
                 (l1[0] * l1[3] - l1[2] * l1[1]) * (l0[0] - l0[1])
    y_nominator = (l0[0] * l0[3] - l0[2] * l0[1]) * (l1[2] - l1[3]) -\
                 (l1[0] * l1[3] - l1[2] * l1[1]) * (l0[2] - l0[3])

    return [x_nominator / denominator, y_nominator / denominator]

def weighted_average(array, weights):
    """Calculate the weighted average of array

    weighted_average = sum(w_i * x_i) / sum(w_i)

    :array: 1xn array
    :weights: 1xn array
    :returns: Number - The weighted average of array
    """
    assert len(array) == len(weights)
    return sum([x * w for x, w in zip(array, weights)]) / sum(weights)

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
