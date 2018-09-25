'''
Brief:               Spin loop illustration of in an Ising model

File name:           ising_loop_illu.py
Author:              Simon Rydell
Python Version:      3.7
'''

import helpers.illustrations as illu
from matplotlib import pyplot as plt
import random
import numpy as np

if __name__ == '__main__':
    x = 2
    y = 2
    form = "pdf"
    # Color of the links
    color = "#d64d4d"

    # [[x0, y0], ...]
    list_of_links = [[[0, 1], [0, 0]],\
                     [[0, 1, 1], [0, 0, 1]],\
                     [[0, 1, 1, 0], [0, 0, 1, 1]],\
                     [[0, 1, 1, 0, 0], [0, 0, 1, 1, 0]],\
                     ]

    fignames = ["ising_loop_one_link", "ising_loop_two_link", "ising_loop_three_link", "ising_loop_four_link"]
    for links, figname in zip(list_of_links, fignames):
        plt.clf()
        illu.plot_lattice(x, y)

        # Label s_i, e_i^0 and e_i^1
        plt.text(1.1, 1.1, r"$S_4$", fontsize=16, horizontalalignment='center', verticalalignment='center')
        plt.text(0.1, 1.1, r"$S_3$", fontsize=16, horizontalalignment='center', verticalalignment='center')
        plt.text(1.1, .1, r"$S_2$", fontsize=16, horizontalalignment='center', verticalalignment='center')
        plt.text(0.1, 0.1, r"$S_1$", fontsize=16, horizontalalignment='center', verticalalignment='center')

        plt.plot(links[0], links[1], c=color, linewidth=2)

        # plt.show()

        ans = input(f"Save figure as: {figname}? ")

        if ans in 'yY':
            plt.savefig(f"./plots/{figname}.{form}",\
                        bbox_inches='tight', format=form, dpi=1200)
