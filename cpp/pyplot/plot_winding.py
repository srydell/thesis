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
    # simulation_data = calc.process_file("./data/windingnum_tempXY", key=r"L=(\d+):", xy=r"(\d*\.?\d*) (\d*\.?\d*) (\d*\.?\d*)")
    simulation_data = calc.process_file("./winding", key=r"L=(\d+):", xy=r"(\d*\.?\d*) (\d*\.?\d*) (\d*\.?\d*) (\d*\.?\d*)")

    # Necessary for the zoomed inset
    fig, ax = plt.subplots()

    colors = const.GRADIENT["blue"]
    c_id = 0
    plot_dict = {}
    zoomed_data = {}
    for size in simulation_data:
        c_id += 1
        c = colors[c_id % len(colors)]

        temperatures = simulation_data[size][0]
        avg_winding_number = simulation_data[size][1]
        std_winding_number = simulation_data[size][2]
        number_measurements = simulation_data[size][3]

        # Old way of calculating winding number
        # avg_winding_number = simulation_data[size][0]
        # temperatures = simulation_data[size][1]
        # number_measurements = simulation_data[size][2]

        unique_temperatures = sorted(list(set(temperatures)))

        # Create the data in the format that calc.plot_errorbars wants
        avg_ws = []
        errorbars_dict = {}
        for temp in unique_temperatures:
            # All indices for t = temp
            # NOTE: Done since the order of the data is random
            indices = [i for i, t in enumerate(temperatures) if t == temp]

            avg_w = calc.add_mean([avg_winding_number[i] for i in indices])
            std_w = calc.add_mean([std_winding_number[i] for i in indices])
            n = [number_measurements[i] for i in indices]
            errorbars_dict[temp] = [avg_w, std_w, sum(n)]

            # For plotting lines between averages
            avg_ws.append(avg_w)


        calc.plot_errorbars(errorbars_dict, f"${int(size)}^3$", color=c, axis=ax)
        ax.plot(unique_temperatures, avg_ws, color=c, linewidth=1.2)

        zoomed_data[size] = [unique_temperatures, avg_ws]
        plot_dict[size] = errorbars_dict

    # [x0, x1, y0, y1, zoom_factor]
    zoomed_size = [0.330, 0.335, -0.1, 1.0, 3]
    ticks = [[0.330, 0.333, 0.335], [0.0, 0.5, 1.0]]

    # rel_{x,y} refers to the lower left corner of the inset
    # rel_{width,height} is relative the whole plot
    # [rel_x, rel_y, rel_width, rel_height]
    inset_position = [0.1, 0.5, 0.4, 0.4]

    calc.plot_zoomed_inset(zoomed_data,
                           zoomed_size,
                           ax,
                           colors,
                           # Uncomment to plot with error bars in inset
                           # plot_dict,
                           ticks=ticks,
                           inset_position=inset_position)

    ax.set_xlabel("Temperature")
    ax.set_ylabel(r"$\langle W^2 \rangle \propto \frac{L}{T} \rho_s$")
    ax.set_title(r"Average winding square on a 3D XY lattice")
    ax.legend(loc=1)
    # illu.save_figure("winding_number_Tc")
    plt.show()
