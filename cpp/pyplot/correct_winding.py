'''
Brief:               Correct an error I made where all winding numbers were multiplied by 3

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
    simulation_data = calc.process_file("./data/windingnum_temp_closetoTc.txt", key=r"L=(\d+):", xy=r"(\d*\.?\d*) (\d*\.?\d*)")

    with open("test.out", 'w') as f:
        for size in simulation_data:
            f.write(f"L={int(size)}:\n")
            for i in range(len(simulation_data[size][0])):
                winding_number = simulation_data[size][0][i]
                temperature = simulation_data[size][1][i]

                f.write(f"{winding_number/3} {temperature}\n")
