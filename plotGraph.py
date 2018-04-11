import random
from graphs import getLinkedNeighbours, loadGraph
import matplotlib.pyplot as plt

def pause():
    programPause = input("Press <Enter> to continue")

def plotGraph(graph, N, M, savePlot=False):
    """
    :graph: dictionary
    :N: Int - Number of rows
    :M: Int - Number of columns
    :savePlot: Boolean
    :returns: None
    """

    axes = plt.gca()
    axes.set_xlim([N-N/10, N+N/10])
    axes.set_ylim([M-M/10, M+M/10])

    for site in graph:
        # None here is just a placeholder for the neighbours x and y
        x = [site[0], None]
        y = [site[1], None]

        linkedNeighbours = getLinkedNeighbours(site, graph)

        for neighbour in linkedNeighbours:
            # print(f"Plotting from {site} to {neighbour}\n")
            x[1] = neighbour[0]
            y[1] = neighbour[1]

            plt.plot(x, y)

    # Pause after every frame. Time argument is in seconds
    plt.pause(0.1)

    if savePlot:
        plt.savefig("./figures/graph.png", bbox_inches="tight")

def plotCorr(corr):
    """Plot correlation function corr in the r direction as a histogram
    NOTE: The values in the correlation function is squared for computational purposes
        Should decay as r^(1/4) for 2D Ising

    :corr: dictionary - Correlation function
    :returns: None
    """
    # Width of one bar
    width = 1.0

    keys = [sqrt(key) for key in corr.keys()]
    plt.bar(keys, corr.values(), width, color='g')

def plotXCorr(corr):
    """Plot correlation function corr in the x direction as a histogram
    NOTE: The values in the correlation function is squared for computational purposes
        Should decay as r^(1/4) for 2D Ising

    :corr: dictionary - Correlation function
    :returns: None
    """
    # Width of one bar
    width = 1.0

    # Normalize to g(i) = G(i) / G(0)
    keys = [key/corr[0] for key in corr.keys()]
    plt.bar(keys, corr.values(), width, color='g')

if __name__ == '__main__':
    N = 300
    M = 300
    bc = "dirichlet"
    # graph = loadGraph(f"{N}x{M}{bc}")
    # plotGraph(graph, N, M, savePlot=False)

    # Do not close the graph after plotting
    # plt.show()
