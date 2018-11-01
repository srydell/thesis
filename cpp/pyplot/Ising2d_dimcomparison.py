'''
Brief:               Plot of a comparison between the results of the 2d Ising dimensions

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

    # Self avoiding walk
    d_saw = 4/3
    # Random walk
    d_rw = 2
    # Geometric Hausdorff dimension (exact)
    d_dg = 1.375
    # scaling result
    d_sc = 1.38
    d_sc_std = 0.02
    # box counting result
    d_bc = 1.35193
    d_bc_std = 5 * 10 ** (-4)
    labels = ["Box dimension", "Scaling dimension", "Geometric $D_H$ $2D$ Ising", "Self avoiding walk", "Random walk"]

    plt.scatter(0, d_rw, c=const.COLOR_MAP["black"], label="Random walk")
    plt.scatter(1, d_saw, c=const.COLOR_MAP["yellow"], label="Self avoiding walk")
    plt.scatter(2, d_dg, c=const.COLOR_MAP["green"], label="Geometric $D_H$ Ising")
    calc.plot_errorbars({3: [d_bc, d_bc_std, 1]}, label="Box dimension, Ising", color=const.COLOR_MAP["red"])
    calc.plot_errorbars({4: [d_sc, d_sc_std, 1]}, label="Scaling dimension, Ising", color=const.COLOR_MAP["blue"])


    plt.xticks([0, 1, 2, 3, 4], [], rotation=45)
    plt.legend(loc=1)
    plt.ylabel(r"$D$")
    plt.title(rf"Comparison of fractal dimensions on a $2D$ lattice")
    # illu.save_figure("dimenson_comparison")
    plt.show()
