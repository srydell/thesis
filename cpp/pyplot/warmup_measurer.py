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

rc('font', **{'family': 'serif', 'serif': ['DejaVu Sans']})
rc('text', usetex=True)

def process_file(filename):
    """Go through filename and split it into an array

    :filename: String - Valid path to a file containing the abow
    :returns: 1xn array - [winding_numer0, ...]
    """
    out_data = []
    with open(filename) as data_file:
        for line in data_file.readlines():
            # Check data
            wn_temp_match = re.match(r"(\d+\.?\d+)", line)
            if wn_temp_match:
                winding_number = float(wn_temp_match.group(1))
                # Save the data under the current system size
                out_data.append(winding_number)

    return out_data

if __name__ == '__main__':
    simulation_data = process_file("./data/warmup_windingt2l8xy.txt")

    plt.scatter(list(range(len(simulation_data))), simulation_data, s=1, c='k', label=r"$W$")

    plt.title("XY lattice")
    plt.xlabel("Number of simulations")
    plt.ylabel(r"$W$")
    plt.legend()
    plt.show()
