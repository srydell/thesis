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
    simulation_data = calc.process_file("./cleaned_mats_data.txt", key=r"L=(\d+):", xy=r"(\d*\.?\d*) (\d*\.?\d*)")

    colors = const.COLORS
    c_id = 0
    for size in simulation_data:
        c_id += 1
        c = colors[c_id % len(colors)]

        unique_temperatures = sorted(list(set(simulation_data[size][0])))
        winding_number = simulation_data[size][1]

        # Create the data in the format that calc.plot_errorbars wants
        plot_dict = {}
        for temp in unique_temperatures:
            # All indices for t = temp
            indices = [i for i, t in enumerate(simulation_data[size][0]) if t == temp]

            w = [winding_number[i] for i in indices]
            plot_dict[temp] = [np.mean(w), np.std(w), len(w)]

        calc.plot_errorbars(plot_dict, f"${int(size)}^3$", color=c)

        # Plot the lines between the error bars
        avg_windings = []
        for t in sorted(list(unique_temperatures)):
            avg_windings.append(np.mean(plot_dict[t]))
        plt.plot(list(unique_temperatures), avg_windings, color=c)

    plt.xlabel("Temperature")
    plt.ylabel(r"$\langle W^2 \rangle \propto \frac{L}{T} \rho_s$")
    plt.title(r"Superfluid density, $\rho_s$, determined in terms of the winding number on a 3D XY lattice")
    plt.legend(loc=1)
    plt.show()
    # illu.save_figure("winding_number_Tc")
