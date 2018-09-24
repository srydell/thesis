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

rc('font', **{'family': 'serif', 'serif': ['DejaVu Sans'], 'size': 14})
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
    simulation_data = process_file("./data/warmup_winding.txt")

    plt.scatter(list(range(len(simulation_data))), simulation_data, s=1, c='k', label=r"$\frac{W}{3}$")

    rolling_avg = []
    gathering_avg = []
    num_points = 1000
    for i, wn in enumerate(simulation_data):
        if i % num_points == 0:
            current_avg = np.mean(gathering_avg)
            for n in range(num_points):
                rolling_avg.append(current_avg)
            gathering_avg = []
        gathering_avg.append(wn)

    plt.scatter(range(len(rolling_avg)), rolling_avg, s=2, c='b', label=f"Rolling average every {num_points} points")

    plt.title("XY lattice")
    plt.xlabel("Number of simulations")
    plt.ylabel(r"$\frac{W_{xyz}}{3}$")
    plt.legend()
    plt.show()
