'''
Brief:               Plot the winding number against the temperature

File name:           plot_winding.py
Author:              Simon Rydell
Python Version:      3.7
'''

import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rc
import helpers.calc as calc
import helpers.constants as const
import helpers.illustrations as illu

rc('font', **{'family': 'serif', 'serif': ['DejaVu Sans']})
rc('text', usetex=True)

if __name__ == '__main__':
    mats_simulation_data = calc.process_file("./cleaned_mats_data.txt", key=r"L=(\d+):", xy=r"(\d*\.?\d*) (\d*\.?\d*)")
    # The good big data
    simon_simulation_data = calc.process_file("./data/windingnum_tempXY", key=r"L=(\d+):", xy=r"(\d*\.?\d*) (\d*\.?\d*)")

    colors = {4: "#b2b2b2", 8: "#999999", 16: "#7f7f7f", 30: "#666666", 40: "#4c4c4c"}
    for size in [4, 8, 16, 30, 40]:
    # for size in [4, 8, 16]:
        c = colors[size]
        unique_temperatures = sorted(list(set(mats_simulation_data[size][1])))
        winding_number = mats_simulation_data[size][0]
        # Create the data in the format that calc.plot_errorbars wants
        plot_dict = {}
        for temp in unique_temperatures:
            # All indices for t = temp
            indices = [i for i, t in enumerate(mats_simulation_data[size][1]) if t == temp]
            plot_dict[temp] = [winding_number[i] for i in indices]
        # Plot the lines between the error bars
        avg_windings = []
        for t in sorted(list(unique_temperatures)):
            avg_windings.append(np.mean(plot_dict[t]))
        plt.plot(list(unique_temperatures), avg_windings, color=colors[size], label=f"M {int(size)}")
        plt.scatter(list(unique_temperatures), avg_windings, color=colors[size], s=3)
        plt.legend(loc=2)

    colors = {4: "#9999ff", 8: "#4c4cff", 16: "#0000e5", 32: "#000066", 64: "#000066"}
    for size in simon_simulation_data:
        unique_temperatures = sorted(list(set(simon_simulation_data[size][1])))
        winding_number = simon_simulation_data[size][0]
        # Create the data in the format that calc.plot_errorbars wants
        plot_dict = {}
        for temp in unique_temperatures:
            # All indices for t = temp
            indices = [i for i, t in enumerate(simon_simulation_data[size][1]) if t == temp]
            plot_dict[temp] = [winding_number[i] for i in indices]
        # Plot the lines between the error bars
        avg_windings = []
        for t in sorted(list(unique_temperatures)):
            avg_windings.append(np.mean(plot_dict[t]))
        plt.plot(list(unique_temperatures), avg_windings, color=colors[size], label=f"S {int(size)}")
        plt.scatter(list(unique_temperatures), avg_windings, color=colors[size], s=3)
        plt.legend(loc=1)

    plt.xlabel("Temperature")
    plt.ylabel(r"$\langle W^2 \rangle \propto \frac{L}{T} \rho_s$")
    plt.title(r"Mean square winding number on a 3D XY lattice")
    # plt.xlim([.05, .7])
    # plt.ylim([-.05, .3])
    plt.show()
    # illu.save_figure("winding_number_Tc")
