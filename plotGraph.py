import random
from graphFunctions import getLinkedNeighbours
import matplotlib.pyplot as plt

def pause():
    programPause = input("Press <Enter> to continue")

def plotGraph(graph, saveGraph=False):
    """
    :graph: dictionary
    :saveGraph: Boolean
    :returns: None
    """

    for site in graph:
        # None here is just a placeholder
        x = [site[0], None]
        y = [site[1], None]

        linkedNeighbours = getLinkedNeighbours(site, graph)

        # NOTE: Here site is a 1x2 tuple
        site = list(site)

        print(f"Linked neighbours to {site} is {linkedNeighbours}")

        for neighbour in linkedNeighbours:
            print(f"Plotting {site} to {neighbour}\n")
            x[1] = neighbour[0]
            y[1] = neighbour[1]

            pause()
            plt.plot(x, y)
            # In seconds
            plt.pause(1)

    if saveGraph:
        plt.savefig("./figures/graph.png", bbox_inches="tight")

if __name__ == '__main__':
    testGraph = {(0, 0): [[(0, 1), 0], [(2, 0), 0], [(0, 2), 0]], (0, 1): [[(0, 2), 0], [(1, 1), 1]], (0, 2): [[(0, 0), 0], [(2, 2), 1], [(1, 2), 1]], (1, 0): [[(0, 0), 0], [(2, 0), 1]], (1, 1): [[(1, 0), 1], [(2, 1), 1]], (1, 2): [[(1, 0), 0], [(1, 1), 1]], (2, 0): [[(2, 1), 0], [(1, 0), 1], [(2, 2), 1], [(0, 0), 0]], (2, 1): [[(2, 2), 0], [(2, 0), 0], [(0, 1), 1]], (2, 2): [[(1, 2), 0], [(2, 1), 0]]}

    N = 3
    M = 3
    axes = plt.gca()
    axes.set_xlim([-1, 3])
    axes.set_ylim([-1, 3])
    plotGraph(testGraph, saveGraph=True)

    # Do not close the graph after plotting
    plt.show()
