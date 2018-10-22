'''
Brief:               Plot the Hausdorff dimension for XY 3D lattice

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

if __name__ == '__main__':
    system_size = 64
    # {system_size: [rel_box_size, avg(D_H), std(D_H), num_measurements], ...}
    simulation_data = calc.process_file(f"./data/h_dimensionXY",
                                        key=r"L=(\d*\.?\d*):",
                                        xy=r"(\d*\.?\d*) (\d*\.?\d*) (\d*\.?\d*) (\d*\.?\d*)")

    # Necessary for the zoomed inset
    fig, ax = plt.subplots()

    # Clear simulation data except system_size
    data = simulation_data[system_size]
    simulation_data.clear()
    simulation_data[system_size] = data

    ignore_relbox_sizes = [1/2]

    labeled = False
    color = const.COLOR_MAP["black"]
    for size in simulation_data:
        rel_box_sizes = simulation_data[size][0]
        avg_dhs = simulation_data[size][1]
        std_dhs = simulation_data[size][2]
        num_measurements = simulation_data[size][3]

        unique_relboxes = sorted(list(set(rel_box_sizes)))
        for s in ignore_relbox_sizes:
            unique_relboxes.remove(s)

        # Create the data in the format that calc.plot_errorbars wants
        errorbars_dict = {}
        for relbox in unique_relboxes:
            # NOTE: Done since the order of the data is random
            indices = [i for i, rb in enumerate(rel_box_sizes) if rb == relbox]

            avg_dh = calc.add_mean([avg_dhs[i] for i in indices])
            std_dh = calc.add_std([std_dhs[i] for i in indices])
            n = [num_measurements[i] for i in indices]

            errorbars_dict[relbox] = [avg_dh, std_dh, sum(n)]

            # Assume that the best approximation is for the smallest box size
            if relbox == min(unique_relboxes):
                print(f"D_H = {avg_dh:.6f} +- {std_dh / sum(n):.0e}")
                best_dh = avg_dh
                best_error_dh = std_dh / sum(n)

        calc.plot_errorbars(errorbars_dict, r"$\bar{D}_H \pm \sigma_{\bar{D}_H}$", color=color, axis=ax)

    ranges = [0.7 * min(unique_relboxes), 1.05 * max(unique_relboxes)]

    # correct_dh = 1.375
    # plt.plot(ranges,
    #          [correct_dh, correct_dh],
    #          c=const.COLOR_MAP["green"],
    #          label=rf"$D_H^G = {correct_dh}$")

    # Ugly way of getting the [(1, 64), (1, 32), ...]
    fractions = list(reversed(sorted([ur.as_integer_ratio() for ur in unique_relboxes])))
    labels = []
    for f in fractions:
        ln = f"{f[0]}"
        ld = f"{f[1]}"
        l = r"$\frac{" + ln + r"}{" + ld + r"}$"
        labels.append(l)

    positions = sorted(unique_relboxes)
    plt.xticks(positions,
               labels,
               fontsize=14)

    plt.xlim(ranges)
    plt.xlabel("Relative size of box division")
    plt.ylabel(r"$D_H$")
    plt.title(rf"Geometric Hausdorff dimension, $D_H$, on a ${system_size}^3$ XY lattice")
    plt.legend(loc=2)
    # illu.save_figure(f"box_dim_{system_size}x{system_size}x{system_size}xy")
    plt.show()
