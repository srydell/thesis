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
    simulation_data = calc.process_file("./data/windingnum_tempXY", key=r"L=(\d+):", xy=r"(\d*\.?\d*) (\d*\.?\d*) (\d*\.?\d*)")

    colors = {4: "#9999ff", 8: "#4c4cff", 16: "#0000e5", 32: "#000066", 64: "#000066"}
    # colors = const.COLORS
    c_id = 0
    for size in simulation_data:
        c_id += 1
        c = colors[c_id % len(colors)]

        avg_winding_number = simulation_data[size][0]
        temperatures = simulation_data[size][1]
        number_measurements = simulation_data[size][2]

        unique_temperatures = sorted(list(set(temperatures)))

        # Create the data in the format that calc.plot_errorbars wants
        plot_dict = {}
        for temp in unique_temperatures:
            # All indices for t = temp
            # NOTE: Done since the order of the data is random
            indices = [i for i, t in enumerate(temperatures) if t == temp]

            avg_w = calc.add_mean([avg_winding_number[i] for i in indices])
            n = [number_measurements[i] for i in indices]
            plot_dict[temp] = [calc.add_mean(avg_w), calc.add_std(avg_w), sum(n)]

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
