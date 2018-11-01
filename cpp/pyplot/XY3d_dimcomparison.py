'''
Brief:               Plot of a comparison between the results of the 3d XY dimensions

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

    # Sudbo
    d_s = 2.287 
    d_s_std = 2 * 10 ** (-3)
    # Prokof'ev
    d_p = 1.7655
    d_p_std = 2 * 10 ** (-3)
    # box counting result
    d_bc = 1.77468
    d_bc_std = 4 * 10 ** (-6)

    calc.plot_errorbars({0: [d_s, d_s_std, 1]}, label="Hove, Mo and Sudbo", color=const.COLOR_MAP["red"])
    calc.plot_errorbars({1: [d_p, d_p_std, 1]}, label="Prokof'ev and Svistunov", color=const.COLOR_MAP["blue"])
    calc.plot_errorbars({2: [d_bc, d_bc_std, 1]}, label="Box dimension", color=const.COLOR_MAP["black"])


    plt.xticks([0, 0.5, 1], [], rotation=45)
    plt.legend(loc=1)
    plt.ylabel(r"$D_H$")
    plt.title(rf"Comparison of fractal dimensions on a $3D$ XY lattice")
    # illu.save_figure("dimenson_comparison_XY")
    plt.show()
