'''
Brief:               Plot calculated values of eta from box dimension data

File name:           plottest.py
Author:              Simon Rydell
Date created:        Jul 16, 2018
Date last modified:  Jul 16, 2018
Python Version:      3.7
'''

import matplotlib.pyplot as plt
import math
import numpy as np

from matplotlib import rc
rc('font',**{'family':'serif','serif':['Palatino']})
rc('text', usetex=True)

# NOTE: This is specific to a run where the max size is 64x64 lattice
#       Therefore 32 is 1/2
size_conversions = {32: 1/2, 16: 1/4, 8: 1/8, 4: 1/16, 2: 1/32}

# Will store size : number of occupied boxes
size_and_boxes = {}
with open(f"./box_size_occupied.txt") as f:
    for line in f.readlines():
        # NOTE: size here is the actual length of the side (64, 32, ...)
        size, box = [int(data) for data in line.strip().split()]

        if size_conversions.get(size) is not None:
            # Convert to relative size
            size = size_conversions.get(size)
            if size_and_boxes.get(size) is None:
                # {size : number occupied}
                size_and_boxes[size] = [box]

            else:
                # {size : number occupied}
                size_and_boxes[size].append(box)

# Will store all calulated eta: D_H - 2 = eta
# This will be used to calculate the average
size_and_eta = {}
size_colors = {1/2: "#966842", 1/4: "#f44747", 1/8: "#eedc31", 1/16: "#7fdb6a", 1/32: "#0e68ce"}
# Find the corresponding dimension for each worm
for size in size_and_boxes:
    for num_occupied in size_and_boxes[size]:
        # D_H = log(n(s)) / log(1 / s)
        dimension = -1 * math.log(num_occupied) / math.log(size)
        eta = 2 - dimension

        # Populate size_and_eta
        if size_and_eta.get(size) is not None:
            size_and_eta[size].append(eta)
        else:
            size_and_eta[size] = [eta]

        # Plot the calculated eta
        plt.scatter(size, eta, c=size_colors[size], s=1.4)

average_etas = []
sizes = []
std_average = []
for size in [1/2, 1/4, 1/8, 1/16, 1/32]:
    # Calculate the average eta for each size
    average_etas.append(np.average(size_and_eta[size]))
    sizes.append(size)

    # Calculate the standard deviation
    # std_dev = sqrt(((sample1 - average)^2 + ... + (sampleN - average)^2)/N)
    std_average.append(np.std(size_and_eta[size]))

# Plot the average as a line
# plt.scatter(sizes, average_etas, c=list(size_colors.values()), s=40, label=r"Average calculated $\eta$")
plt.errorbar(sizes, average_etas, yerr=std_average,\
        ecolor='gray', elinewidth=2, fmt='k.', linestyle="None",\
        capsize=3, capthick=2, label=r"$\bar \eta \pm \sigma_\eta$")

# Plot the real eta as a black line
real_eta = 1/4
plt.plot([min(sizes), max(sizes)], [real_eta, real_eta], '#000000', label=r"$\eta_{Ising \ 2D} = 1/4$")

# Ticks
plt.xticks([1/32, 1/16, 1/8, 1/4, 1/2],\
        [r"$\frac{1}{32}$", r"$\frac{1}{16}$", r"$\frac{1}{8}$", r"$\frac{1}{4}$", r"$\frac{1}{2}$"],\
        fontsize=14)

# Set labels
plt.title("Box dimension calculation for decreasing box size on a 64x64 Ising lattice")
plt.xlabel("Relative size of box division")
plt.ylabel(r"$\eta$")

plt.legend()
plt.tight_layout()
plt.savefig("box_dimension_for_64x64Ising.png", bbox_inches='tight')
# plt.show()
