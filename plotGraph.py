import random
from graphs import getLinkedNeighbours, loadGraph
import matplotlib.pyplot as plt

PLOT_CONFIG_LOADED = False
GRAPH_AXIS = None
CORR_AXIS = None
FIGURE = None

def plotConfigs():
    """Sets the configs for plotting
    :returns: None
    """
    global PLOT_CONFIG_LOADED, GRAPH_AXIS, CORR_AXIS, FIGURE

    fig = plt.figure()
    ax1 = fig.add_subplot(211)
    ax2 = fig.add_subplot(212)

    # ax1 clears itself every run so this needs to be reset in the function
    # This is only so that fig.tight_layout() looks nice
    ax1.set_title("Ising Worm")
    ax2.set_title("Correlation function")

    fig.tight_layout()

    FIGURE = fig
    GRAPH_AXIS = ax1
    CORR_AXIS = ax2

    PLOT_CONFIG_LOADED = True

def plotGraph(clusters, graph, savePlot=False):
    """
    :clusters: dictionary
    :graph: dictionary
    :N: Int - Number of rows
    :M: Int - Number of columns
    :savePlot: Boolean
    :returns: None
    """

    if not PLOT_CONFIG_LOADED:
        plotConfigs()

    GRAPH_AXIS.cla()

    colors = ["#d11141", "#00b159", "#00aedb", "#f37735", "#c425ff", "#5900b1"]

    for index in clusters:
        for site in clusters[index]:
            # None here is just a placeholder for the neighbours x and y
            x = [site[0], None]
            y = [site[1], None]

            linkedNeighbours = getLinkedNeighbours(site, graph)

            for neighbour in linkedNeighbours:
                # print(f"Plotting from {site} to {neighbour}\n")
                x[1] = neighbour[0]
                y[1] = neighbour[1]

                # print(f"Plot configs loaded: {PLOT_CONFIG_LOADED}")
                # input(f"Graph axis:\n{GRAPH_AXIS}")
                GRAPH_AXIS.plot(x, y, color=colors[index%len(colors)])

    # Set title. (It is cleared in the beginning of each function call)
    GRAPH_AXIS.set_title("Ising Worm")

    # Pause after every frame. Time argument is in seconds
    plt.pause(0.01)

    if savePlot:
        plt.savefig("./figures/graph.png", bbox_inches="tight")

def plotCorr(corr):
    """Plot correlation function corr as a histogram

    :corr: dictionary - Correlation function
    :returns: None
    """

    if not PLOT_CONFIG_LOADED:
        plotConfigs()

    # Width of one bar
    width = 1.0

    # Normalize to g(i) = G(i) / G(0)
    if corr.get(0) is not None:
        keys = [key/corr[0] for key in corr.keys()]
    else:
        # No need to normalize, corr[0] = 1
        keys = list(corr.keys())

    CORR_AXIS.bar(keys, corr.values(), width, color='g')

    # Pause after every frame. Time argument is in seconds
    plt.pause(0.01)

if __name__ == '__main__':
    N = 300
    M = 300
    bc = "dirichlet"
    # graph = loadGraph(f"{N}x{M}{bc}")
    # plotGraph(graph, N, M, savePlot=False)

    # Do not close the graph after plotting
    # plt.show()
