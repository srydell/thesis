'''
Brief:               Plot the energy scaling behaviour for XY 3D lattice

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

def fit_function(x, a, exponent, b):
    """Function to fit against
    y = a * x^(exponent)

    :x: Float - Known data
    :a: Float - Unknown data
    :exponent: Float - Unknown data
    :returns: Float
    """
    return a * np.power(x, exponent) + b

if __name__ == '__main__':
    # Format: avg(E) std(E) Num_measurements
    # {system_size: [avg(E), std(E), num_measurements], ...}
    simulation_data = calc.process_file(f"./energy_sizeXY",
                                        key=r"L=(\d*\.?\d*):",
                                        xy=r"(\d*\.?\d*) (\d*\.?\d*) (\d*\.?\d*)")

    # Necessary for the zoomed inset
    fig, ax = plt.subplots()

    color = const.COLOR_MAP["black"]
    avg_es = []
    for size in simulation_data:
        avg_es.append(calc.add_mean(simulation_data[size][0]))

        simulation_data[size][0] = calc.add_mean(simulation_data[size][0])
        simulation_data[size][1] = calc.add_std(simulation_data[size][1])
        simulation_data[size][2] = sum(simulation_data[size][2])


    calc.plot_errorbars(simulation_data, r"$\bar{E} \pm \sigma_{\bar{E}}$", color=color, axis=ax)

    # xdata
    system_sizes = np.array(list(simulation_data.keys()))
    # ydata
    average_energy = np.array(avg_es)

    stderr_pars = calc.bootstrap(fit_function, system_sizes, average_energy, 100)
    opt_pars = list(stderr_pars.keys())

    # Used for plotting against the fitted function
    xdata = np.linspace(min(system_sizes), max(system_sizes), 50)
    # colors = ["#966842", "#f44747", "#eedc31", "#7fdb6a", "#0e68ce"]
    plt.loglog(xdata, fit_function(xdata, *opt_pars),\
            c="#0e68ce",\
            label=fr"$\propto L^{{ {opt_pars[1]:.2f} \pm {stderr_pars[opt_pars[1]]:.2f}  }}$")

    plt.xlabel("Linear system size")
    plt.ylabel(r"$E$")
    plt.title(rf"Scaling behaviour of energy in the Villain approximation on a 3D XY lattice")
    plt.legend(loc=2)
    # illu.save_figure(f"energy_scaling_xy")
    plt.show()
