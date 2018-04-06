import random
from graphFunctions import getLinkedNeighbours, loadGraph
import matplotlib.pyplot as plt

def pause():
    programPause = input("Press <Enter> to continue")

def plotGraph(graph, savePlot=False):
    """
    :graph: dictionary
    :savePlot: Boolean
    :returns: None
    """

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

if __name__ == '__main__':
    N = 300
    M = 300
    bc = "dirichlet"
    graph = loadGraph(f"{N}x{M}{bc}")
    axes = plt.gca()
    axes.set_xlim([198, 212])
    axes.set_ylim([282, 300])
    plotGraph(graph, savePlot=False)

    # Do not close the graph after plotting
    # plt.show()
