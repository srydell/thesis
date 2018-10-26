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
    # simulation_data = calc.process_file("./data/xy_winding.txt",
    #         key=r"L=(\d+):", xy=r"(\d*\.?\d*) (\d*\.?\d*) (\d*\.?\d*) (\d*\.?\d*)")
    simulation_data = calc.process_file("../winding",
            key=r"L=(\d+):", xy=r"(\d*\.?\d*) (\d*\.?\d*) (\d*\.?\d*) (\d*\.?\d*)")

    # Should the zoomed inset be plotted?
    zoomed = True

    # Necessary for the zoomed inset
    fig, ax = plt.subplots()

    colors = const.GRADIENT["blue"]
    c_id = -1
    plot_dict = {}
    zoomed_data = {}
    # Approximations of Tc
    closest_temps_to_tc = [0.333, 0.334]
    closest_ws_to_tc = []
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
        # Plotting very {low,high} temperatures makes the zoomed_inset look weird
        if zoomed:
            # temp_to_remove = [0.1, 0.31, 0.35, 0.6]
            # t = [0.332, 0.333, 0.334]
            temp_to_keep = [0.31, 0.32, 0.33, 0.331, 0.332, 0.333, 0.334, 0.335, 0.34, .35]
            unique_temperatures = [ut for ut in unique_temperatures if ut in temp_to_keep]

        # Create the data in the format that calc.plot_errorbars wants
        avg_ws = []
        errorbars_dict = {}
        for temp in unique_temperatures:
            # All indices for t = temp
            # NOTE: Done since the order of the data is random
            indices = [i for i, t in enumerate(temperatures) if t == temp]

            avg_w = calc.add_mean([avg_winding_number[i] for i in indices])
            std_w = calc.add_std([std_winding_number[i] for i in indices])
            n = [number_measurements[i] for i in indices]
            errorbars_dict[temp] = [avg_w, std_w, sum(n)]

            # For plotting lines between averages
            avg_ws.append(avg_w)

            if temp in closest_temps_to_tc:
                closest_ws_to_tc.append(avg_w)

        calc.plot_errorbars(errorbars_dict, f"${int(size)}^3$", color=c, axis=ax)
        ax.plot(unique_temperatures, avg_ws, color=c, linewidth=1.2)

        zoomed_data[size] = [unique_temperatures, avg_ws]
        plot_dict[size] = errorbars_dict

    # Get all permutations of Tc
    # Use the same thinking as distribution law for multiplication
    tcs = []
    perm_indices = [2 * l for l in range(len(closest_ws_to_tc) // 2)]
    sizes = list(simulation_data.keys())
    # will be size0 * size1 corresponding to each intersection
    weights = []
    for i in perm_indices:

        # print(i)
        # print(f"Size0: {sizes[perm_indices.index(i)]}")

        size0 = sizes[perm_indices.index(i)]
        for j in perm_indices[(i//2):][:-1]:

            # print("\t", j+2)
            # print(f"\tSize1: {sizes[perm_indices.index(j+2)]}")

            size1 = sizes[perm_indices.index(j+2)]
            # NOTE: calc.get_intersection returns [x0, y0] of intersection (therefore [0])
            tcs.append(calc.get_intersection([*closest_temps_to_tc, *closest_ws_to_tc[i:(i+2)]],
                                             [*closest_temps_to_tc, *closest_ws_to_tc[(j+2):(j+4)]])\
                                             [0])
            weights.append(size0 * size1)

    avg_tc = calc.weighted_average(tcs, weights)
    error_tc = np.std(tcs)
    print(f"T_c = {avg_tc:.4f} +- {error_tc:.0e}")
    # format the error nicely
    error_form = list(re.match(r"(\d*)e?(-?\d*)", f"{error_tc:.0e}").groups())
    error_form = [float(e) for e in error_form]

    # [x0, x1, y0, y1, zoom_factor]
    zoomed_size = [0.330, 0.335, -0.1, 1.0, 3]
    # ticks = [[0.330, 0.333, 0.335], [0.0, 0.5, 1.0]]
    # ticks = [[0.333], []]
    ticks = None

    # rel_{x,y} refers to the lower left corner of the inset
    # rel_{width,height} is relative the whole plot
    # [rel_x, rel_y, rel_width, rel_height]
    inset_position = [0.1, 0.5, 0.4, 0.4]

    if zoomed:
        zoomed_axis = calc.plot_zoomed_inset(zoomed_data,
                                             zoomed_size,
                                             ax,
                                             colors,
                                             # Uncomment to plot with error bars in inset
                                             # plot_dict,
                                             ticks=ticks,
                                             inset_position=inset_position)
        for axis in [ax, zoomed_axis]:
            axis.plot([avg_tc, avg_tc],
                      [zoomed_size[2], zoomed_size[3]],
                      linewidth=1,
                      c=const.COLOR_MAP["red"],
                      label=rf"$T_c$")
                      # label=rf"$T_c \approx {avg_tc:.4f} \pm {error_tc:.0e}$")

    ax.set_xlabel("Temperature")
    ax.set_ylabel(r"$\langle W^2 \rangle \propto \frac{L}{T} \rho_s$")
    if zoomed:
        ax.set_title(rf"Average winding squared on a 3D XY lattice, $T_c \approx {avg_tc:.4f} \pm {error_form[0]:.0f} \cdot 10^" + r"{" + str(int(error_form[1])) + r"}$")
    else:
        ax.set_title(rf"Average winding squared on a 3D XY lattice")
    ax.legend(loc=1)

    filename = "winding_number_Tc"
    if zoomed:
        filename += "_zoomed"
        ax.set_xticks([0.32, 0.33, 0.34])
    illu.save_figure(filename)
    # plt.show()
