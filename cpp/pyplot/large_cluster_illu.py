'''
Brief:               Plot a large 2d cluster

File name:           large_cluster_illu.py
Author:              Simon Rydell
Python Version:      3.7
'''


import helpers.calc as calc
import helpers.constants as const
import helpers.illustrations as illu
import helpers.animate as ani
import matplotlib.pyplot as plt
import numpy as np

def plot_setup(size, pltlattice=True):
    """Setup 2d ising plot

    :size: Int - Size of graph
    :returns: None
    """
    # Remove surrounding border
    ax.axis('off')
    ax.set_xlim([-1, size])
    ax.set_ylim([-1, size])

    if pltlattice:
        # Redraw lattice
        ani.plot_lattice(size, const.COLOR_MAP["black"], ax, size=20, d=2)

def min_max_sites_span(sites):
    """Return the {min,max}_x(sites) and {min,max}_y(sites)

    :sites: 1xn array of Site Objects
    :returns: [[min_x, max_x], [min_y, max_y]]
    """
    pass

if __name__ == '__main__':
    fig, ax = plt.subplots()
    system_size = 128
    dimension = 2

    # Color of flux
    c = const.COLOR_MAP["black"]
    # plot_setup(system_size)

    # Load graph
    sites = ani.process_graph_file(f"ising/large_cluster/largest_cluster", dimension)
    ax.axis('off')

    # movegraph = 128 * 50
    movegraph = 90
    # Plot all flux
    for site in sites:
        site.weights = [w / 2 for w in site.weights]
        site.i = site.i + movegraph
        site.neighbours = [n + movegraph for n in site.neighbours]
        site.plot_arrows_to_neighbours(ax, system_size, color=c, style='-')

    illu.save_figure(f"largest_cluster_testing_nolattice")
