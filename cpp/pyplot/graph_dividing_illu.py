'''
Brief:               Illustration of the Graph Dividing algorithm

File name:           graph_dividing_illu.py
Author:              Simon Rydell
Python Version:      3.7
'''

import helpers.illustrations as illu
from matplotlib import pyplot as plt
from matplotlib import rc

rc('font', **{'family': 'serif', 'serif': ['DejaVu Sans']})
rc('text', usetex=True)

if __name__ == '__main__':
    x, y = 4, 4
    s = 3
    figname = "graphDividing"
    form = "pdf"

    illu.plot_lattice(x, y, size=s)

    # Lines dividing next set of boxes
    color = "grey"
    plt.plot([1.5, 1.5], [-.5, y + .5], linestyle="--", color=color)
    plt.plot([-.5, x + .5], [y/2 - .5, y/2 - .5], linestyle="--", color=color)

    # Plot e_i^0 and e_i^1
    illu.plot_arrow(0, 0, 2, 0, size=s/2)
    illu.plot_arrow(0, 0, 0, 2, size=s/2)

    # Label s_i, e_i^0 and e_i^1
    plt.text(1.8, .2, r"$e_i^0$", fontsize=16, horizontalalignment='center', verticalalignment='center')
    plt.text(0.2, 1.8, r"$e_i^1$", fontsize=16, horizontalalignment='center', verticalalignment='center')
    plt.text(0.2, 0.2, r"$s_i$", fontsize=16, horizontalalignment='center', verticalalignment='center')

    # plt.show()

    illu.save_figure(figname)
    # plt.savefig(f"./plots/{figname}.{form}",\
    #             bbox_inches='tight', format=form, dpi=1200)
