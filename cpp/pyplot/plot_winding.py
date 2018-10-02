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

rc('font', **{'family': 'serif', 'serif': ['DejaVu Sans']})
rc('text', usetex=True)

if __name__ == '__main__':
    simulation_data = calc.process_file("../windingnum_temp.txt", key=r"L=(\d+):", xy=r"(\d*\.?\d*) (\d*\.?\d*)")

    plot_histogram = False
    colors = ['k', 'g', 'b', 'r']
    for size in simulation_data:
        color = colors.pop()

        if plot_histogram:
            plt.hist(simulation_data[size][1], label=rf"${int(size)}$", color=color)
            plt.xlabel(r"$\langle W^2 \rangle$")
            plt.ylabel("Counts")
            plt.title(r"XY lattice")
            plt.legend()
            plt.show()
        else:
            # labels = "${current_x}$"
            # calc.plot_errorbars(simulation_data, labels=labels, via_same_x=True)
            labeled = False
            x = []
            y = []
            for i in [1, 2, 3, 4, 5]:
                T = simulation_data[size][0][((i-1)*100):(i*100)][0]
                current_windings = simulation_data[size][1][((i-1)*100):(i*100)]

                avg_winding = np.mean(current_windings)
                montecarlo_std_winding = np.std(current_windings) / (100 * len(current_windings))

                # print(f"Temperature is: {T}")
                # print(f"Average winding is: {avg_winding}")
                # input()

                if not labeled:
                    # plt.scatter(T,\
                    #         avg_winding,\
                    #         c=color,\
                    #         label=rf"${int(size)}^3$")

                    plt.errorbar(T, avg_winding, yerr=montecarlo_std_winding,\
                        ecolor='gray', elinewidth=2, fmt=f'{color}.', linestyle="None",\
                        capsize=3, capthick=2, label=rf"${int(size)}^3$")

                    labeled = True
                else:
                    # plt.scatter(T,\
                    #         avg_winding,\
                    #         c=color)

                    plt.errorbar(T, avg_winding, yerr=montecarlo_std_winding,\
                        ecolor='gray', elinewidth=2, fmt=f'{color}.', linestyle="None",\
                        capsize=3, capthick=2)

                x.append(T)
                y.append(avg_winding)
            plt.plot(x, y, c=color)
            plt.xlabel("Temperature")
            plt.ylabel(r"$\langle W^2 \rangle$")

            plt.title(r"XY lattice, each $\langle W^2 \rangle$ averaged over 100 points")
            plt.legend(loc=1)
    plt.show()
