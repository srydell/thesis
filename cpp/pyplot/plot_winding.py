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
    """Go through filename and split it into arrays
       Assumes that the file is organized as:

       L=<system_linear_size>:
       <avg_winding_num^2> <temperature>
       ...

    :filename: String - Valid path to a file containing the abow
    :returns: dict - {<system_linear_size>:
                      [[<temperature>], [<avg_winding_num^2>, ...]], ...}
    """
    out_dict = {}
    with open(filename) as data_file:
        # Keep track of the system size we are on
        current_system_size = 0
        for line in data_file.readlines():
            # Check if line is describing system size or box sizes

            # print(f"Examining line: {line}")

            # Check system size
            # Match 'L=<one or more digits>:'
            system_size_match = re.match(r"L=(\d+):", line)
            # If it matched
            if system_size_match:
                system_linear_size = float(system_size_match.group(1))

                # print(f"This is a line with a system size: {system_linear_size}")

                if system_linear_size not in out_dict:

                    # print(f"System size: {system_linear_size} has not been seen before. Adding new.")

                    # Create a new array for this system size
                    out_dict[system_linear_size] = [[], []]

                current_system_size = system_linear_size
                # No need to process further, we found a match
                continue

            # Check data
            wn_temp_match = re.match(r"(\d*\.?\d*) (\d*\.?\d*)", line)
            if wn_temp_match:
                avg_windingnum_sqred = float(wn_temp_match.group(1))
                temperature = float(wn_temp_match.group(2))
                # Save the data under the current system size
                out_dict[current_system_size][0].append(temperature)
                out_dict[current_system_size][1].append(avg_windingnum_sqred)

                # print(f"Found winding number: {avg_windingnum_sqred}")
                # print(f"Found temperature: {temperature}")
            # input()

    return out_dict

if __name__ == '__main__':
    simulation_data = process_file("./windingnum_tempXY.txt")

    plot_histogram = False
    colors = ['k', 'g', 'b', 'r']
    for size in simulation_data:
        color = colors.pop()

        if plot_histogram:
            plt.hist(simulation_data[size][1], label=rf"${int(size)}$", color=color)
            plt.xlabel(r"$\langle W^2 \rangle$")
            plt.ylabel("Counts")
            plt.title(r"XY lattice, each $\langle W^2 \rangle$ averaged over 100 points")
            plt.legend()
            plt.show()
        else:
            labeled = False
            x = []
            y = []
            for i in [1, 2, 3]:
            # for i in [1]:
                # NOTE: 100 is the number of points measured
                #       (they are all the same temperature, but different winding)
                if i == 3:
                    T = simulation_data[size][0][((i-1)*100):][0]
                    current_windings = simulation_data[size][1][((i-1)*100):]
                else:
                    T = simulation_data[size][0][((i-1)*100):(i*100)][0]
                    current_windings = simulation_data[size][1][((i-1)*100):(i*100)]

                avg_winding = np.mean(current_windings)
                montecarlo_std_winding = np.std(current_windings) / len(current_windings)

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
