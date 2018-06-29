'''
Brief:               Plot to see if the worms are converging in size on a fixed lattice

File name:           plottest.py
Author:              Simon Rydell
Date created:        Jun 29, 2018
Date last modified:  Jun 29, 2018
Python Version:      3.6
'''

import matplotlib.pyplot as plt

with open("test.txt") as f:
    max_size = 0
    min_size = 0
    num_simulations = 0
    for line in f.readlines():
        # Each line is a simulation
        num_simulations += 1
        loop_lengths = [float(i) for i in line.split(" ")]
        worm_number = list(range(len(loop_lengths)))

        # These are just for y ticks in pyplot
        min_size = min_size if min_size < float(min(loop_lengths)) else float(min(loop_lengths))
        max_size = max_size if max_size > float(max(loop_lengths)) else float(max(loop_lengths))

        plt.plot(worm_number, loop_lengths)

plt.xlabel("Worm number")
plt.ylabel("Size of the largest loop")
plt.title(f"16x16 Ising lattice with {num_simulations} simulations")

plt.yticks([int(min_size), int((max_size - min_size)/3),\
        int(2 * (max_size - min_size)/3), int(max_size)])
# plt.show()
# plt.tight_layout()
plt.savefig("test.png", bbox_inches='tight')
