'''
Brief:               Plot the winding number against the temperature

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
    system_size = 128
    # {system_size: [avg(D_H), std(D_H)], ...}
    simulation_data = calc.process_file(f"./data/hdims/box_size{system_size}.txt",
            key=r"box_size=(\d*\.?\d*):",
            xy=r"(\d*\.?\d*) (\d*\.?\d*e?-?\d*)")

    ignore_sizes = [64.0, 32.0]
    for size in ignore_sizes:
        simulation_data.pop(size)

    labeled = False
    color = const.COLOR_MAP["black"]
    for size in simulation_data:
        avg_dh = simulation_data[size][1][0]
        std_dh = simulation_data[size][0][0]
        if not labeled:

            plt.errorbar(size / system_size, avg_dh, yerr=std_dh,\
                ecolor='gray', elinewidth=2, fmt=f'{color}', marker='.',\
                linestyle="None", capsize=3, capthick=2,\
                label=r"$\bar{D}_H \pm \sigma_{\bar{D}_H}$")
            labeled = True
        else:
            plt.errorbar(size / system_size, avg_dh, yerr=std_dh,\
                ecolor='gray', elinewidth=2, fmt=f'{color}', marker='.',\
                linestyle="None", capsize=3, capthick=2)

    correct_dh = 1.375
    ranges = [0.7 * min(simulation_data.keys()) / system_size, 1.05 * max(simulation_data.keys()) / system_size]
    plt.plot(ranges,
             [correct_dh, correct_dh],
             c=const.COLOR_MAP["green"],
             label=rf"$D_H^G = {correct_dh}$")

    # Ugly way of getting the [(1, 64), (1, 32), ...]
    fractions = list(reversed(sorted([(k/system_size).as_integer_ratio() for k in simulation_data])))
    labels = []
    for f in fractions:
        ln = f"{f[0]}"
        ld = f"{f[1]}"
        l = r"$\frac{" + ln + r"}{" + ld + r"}$"
        labels.append(l)

    positions = sorted([k / system_size for k in simulation_data])
    plt.xticks(positions,
               labels,
               fontsize=14)

    plt.xlim(ranges)
    plt.xlabel("Relative size of box division")
    plt.ylabel(r"$D_H$")
    plt.title(rf"Geometric Hausdorff dimension on a ${system_size}^2$ Ising lattice")
    plt.legend(loc=2)
    # plt.show()
    illu.save_figure("box_dim_128x128Ising")
