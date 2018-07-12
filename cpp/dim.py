'''
Brief:               Plot to see if the worms have expected scaling behaviour

File name:           plottest.py
Author:              Simon Rydell
Date created:        Jun 29, 2018
Date last modified:  Jun 29, 2018
Python Version:      3.6
'''

import matplotlib.pyplot as plt
import numpy as np

from matplotlib import rc
rc('font',**{'family':'sans-serif','sans-serif':['Helvetica']})
## for Palatino and other serif fonts use:
#rc('font',**{'family':'serif','serif':['Palatino']})
rc('text', usetex=True)

box_size_and_num_occ = {}
for i in range(1, 4):
    with open(f"./box_size_occ{i}.txt") as f:
        num_occupied_boxes = []
        one_over_size = []
        for n, line in enumerate(f.readlines()):
            boxes_and_size = [float(i) for i in line.split(" ")]

            if box_size_and_num_occ.get(int(boxes_and_size[0])) is not None:
                box_size_and_num_occ[int(boxes_and_size[0])].append(boxes_and_size[1])
            else:
                box_size_and_num_occ[int(boxes_and_size[0])] = [boxes_and_size[1]]

# Remove the largest one_over_size to avoid dividing by log(1) = 0
full_size = max(list(box_size_and_num_occ.keys()))
box_size_and_num_occ.pop(full_size, None)

avg_dim = []
avg_size = []
for n, key in enumerate(box_size_and_num_occ):
    l = len(box_size_and_num_occ[key])
    one_over_size = [np.power(2, l + 2 - n)]*l
    dimensions = []
    for box in box_size_and_num_occ[key]:
        print(f"taking np.log({box}) / np.log({one_over_size[0]})")
        dimensions.append(np.log(box) / np.log(one_over_size[0]))
    print(f"one_over_size: {one_over_size}")
    print(f"Dimensions: {dimensions}")
    avg_dim.append(np.average(dimensions))
    avg_size.append(1 / one_over_size[0])
    plt.scatter([1 / s for s in one_over_size], dimensions)

eta_ising = 1/4
expected_dim = 2 - eta_ising
plt.plot(avg_size, avg_dim, label="Average dimension")
plt.plot([min(avg_size), max(avg_size)], [expected_dim, expected_dim], label="Expected dimension = 3/4")

plt.xticks([1/32, 1/16, 1/8, 1/4, 1/2], [r"$\frac{1}{32}$", r"$\frac{1}{16}$", r"$\frac{1}{8}$", r"$\frac{1}{4}$", r"$\frac{1}{2}$"])

plt.legend()

plt.xlabel("1 / s")
plt.ylabel("Dimension")
plt.title("Dimension of the largest worm on a 64x64 Ising lattice")

plt.show()
