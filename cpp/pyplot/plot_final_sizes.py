'''
Brief:               Plot to see if the worms have expected scaling behaviour

File name:           plottest.py
Author:              Simon Rydell
Date created:        Jun 29, 2018
Date last modified:  Jun 29, 2018
Python Version:      3.6
'''

import numpy as np
import matplotlib.pyplot as plt

with open("./final_sizes.txt") as f:
    max_size = 0
    min_size = 0
    num_simulations = 0
    # sizes: 4,  8,  16, 32
    sizes = [[], [], [], []]
    for line in f.readlines():
        # Each line is a simulation
        num_simulations += 1
        loop_lengths = [float(i) for i in line.split(" ")]
        for i, l in enumerate(loop_lengths):
            sizes[i].append(l)

avg_loop_sizes = []
system_sizes = [4, 8, 16, 32]
for i, ll in enumerate(sizes):
    plt.scatter([system_sizes[i]]*len(ll), ll)
    avg_loop_sizes.append(np.mean(ll))
plt.plot(system_sizes, avg_loop_sizes, label="Average final size")

# # These are just for y ticks in pyplot
# min_size = min_size if min_size < float(min(loop_lengths)) else float(min(loop_lengths))
# max_size = max_size if max_size > float(max(loop_lengths)) else float(max(loop_lengths))

plt.xlabel("System size")
plt.ylabel("Size of the largest loop")
plt.title(f"Ising lattice after {num_simulations} simulations on each lattice size")

plt.xticks(system_sizes, system_sizes)

plt.legend()

# plt.show()
# plt.tight_layout()
plt.savefig("largest_worm_sizes.png", bbox_inches='tight')
