'''
Brief:               Plot the Hausdorff dim on a 3D XY lattice

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

rc('font', **{'family': 'serif', 'serif': ['DejaVu Sans']})
rc('text', usetex=True)

def fit_function(x, a, b):
    """Function to fit against
    y = a * x + b

    :x: Float - Known data
    :a: Float - Unknown data
    :b: Float - Unknown data
    :returns: Float
    """
    return a * x + b

if __name__ == '__main__':
    # Format: Relative_box_size avg(D_H) std(D_H) Num_measurements
    simulation_data = calc.process_file("./h_dims",
            key=r"L=(\d+):", xy=r"(\d*\.?\d*) (\d*\.?\d*) (\d*\.?\d*) (\d*\.?\d*)")

    # Necessary for the zoomed inset
    # fig, ax = plt.subplots()
    system_size = 128
    rel_box_sizes, avg_dhs, std_dhs, num_measurements = simulation_data[system_size]
    skip_boxes = [1/2, 1/4]
    # skip_boxes = []

    c = const.COLOR_MAP["black"]
    unique_rel_boxes = list(reversed(sorted(list(set(rel_box_sizes)))))
    for sb in skip_boxes:
        unique_rel_boxes.remove(sb)

    x_relbox = []
    y_lnnumboxes = []

    errorbars_dict = {}
    min_relbox = min(unique_rel_boxes)
    for u_relbox in unique_rel_boxes:
        # All indices for t = u_relbox
        # NOTE: Done since the order of the data is random
        indices = [i for i, ub in enumerate(rel_box_sizes) if ub == u_relbox]

        avg_dh = calc.add_mean([avg_dhs[i] for i in indices])
        std_dh = calc.add_std([std_dhs[i] for i in indices])
        n = [num_measurements[i] for i in indices]
        errorbars_dict[u_relbox] = [avg_dh, std_dh, sum(n)]
        if u_relbox == min_relbox:
            print(f"D_H = {avg_dh:.5f} += {std_dh/np.sqrt(sum(n)):.0e}")
            # quit()
        x_relbox.append(u_relbox)
        y_lnnumboxes.append(avg_dh * np.log(1 / u_relbox))

    print(y_lnnumboxes)
    print(x_relbox)
    print(calc.bootstrap(fit_function, x_relbox, y_lnnumboxes))
    quit()
    # :data_dict: dict - Assumed to be {x0: [avg(y0), std(y0), num_measurements(y0)], x1: ...}
    calc.plot_errorbars(errorbars_dict, r"$\bar D_H \pm \sigma_{\bar D_H}$", color=c, axis=plt)

    # Ugly way of getting the [(1, 64), (1, 32), ...]
    fractions = list(reversed(sorted([(ub).as_integer_ratio() for ub in unique_rel_boxes])))
    labels = []
    for f in fractions:
        ln = f"{f[0]}"
        ld = f"{f[1]}"
        l = r"$\frac{" + ln + r"}{" + ld + r"}$"
        labels.append(l)

    plt.xscale("log")
    positions = sorted([ub for ub in unique_rel_boxes])
    plt.xticks(positions,
               labels,
               fontsize=14)

    comment_dh = 1.7655
    sudbo_dh = 2.287
    dhs_to_compare = [comment_dh, sudbo_dh]
    labels_to_compare = [r"$D_H^{PS}" + rf" = {comment_dh}$", r"$D_H^{HMS}" + rf" = {sudbo_dh}$"]
    colors = [const.COLOR_MAP["green"], const.COLOR_MAP["red"]]
    ranges = [0.7 * min(unique_rel_boxes), 1.05 * max(unique_rel_boxes)]

    for dh, lbl, clr in zip(dhs_to_compare, labels_to_compare, colors):
        plt.plot(ranges,
                 [dh, dh],
                 c=clr,
                 label=lbl)

    plt.legend(loc=1)
    plt.xlim(ranges)
    plt.xlabel("Relative size of box division")
    plt.ylabel(r"$D_H$")
    plt.title(rf"Geometric Hausdorff dimension, $D_H$, on a ${system_size}^3$ XY lattice")
    # illu.save_figure("box_dimension_xy_128x3")
    plt.show()
