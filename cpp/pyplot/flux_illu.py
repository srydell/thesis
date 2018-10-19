'''
Brief:               Illustration of the flux and resulting winding in a lattice

File name:           flux_illu.py
Author:              Simon Rydell
Python Version:      3.7
'''

import helpers.illustrations as illu
from matplotlib import pyplot as plt

if __name__ == '__main__':
    x, y = 6, 6
    s = 2.5
    color = "#d64d4d"
    figname = "percolatingFlux"

    illu.plot_lattice(x, y, size=1)

    # Simple loop cluster
    illu.fill_with_arrows([2, 3], [5, 3], size=s, color=color)
    illu.fill_with_arrows([5, 3], [5, 5], size=s, color=color)
    illu.fill_with_arrows([5, 5], [4, 5], size=s, color=color)
    illu.fill_with_arrows([4, 5], [4, 4], size=s, color=color)
    illu.fill_with_arrows([4, 4], [2, 4], size=s, color=color)
    illu.fill_with_arrows([2, 4], [2, 3], size=s, color=color)

    # Loop cluster with a link with 2 flux quanta
    illu.fill_with_arrows([0, 2], [1, 2], size=s, color=color)
    illu.fill_with_arrows([1, 2], [1, 3], size=s, color=color)
    illu.fill_with_arrows([1, 3], [0, 3], size=2.5 * s, color=color)
    illu.fill_with_arrows([0, 3], [0, 4], size=s, color=color)
    illu.fill_with_arrows([0, 4], [1, 4], size=s, color=color)
    illu.fill_with_arrows([1, 4], [1, 3], size=s, color=color)
    illu.fill_with_arrows([0, 3], [0, 2], size=s, color=color)

    # Percolating cluster
    illu.fill_with_arrows([-1, 1], [x + 1, 1], size=s, color=color)
    # plt.show()
    illu.save_figure(figname)
    # plt.savefig(f"./plots/{figname}.{form}",\
    #             bbox_inches='tight', format=form, dpi=1200)
