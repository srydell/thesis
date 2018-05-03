# Ensure that config can be referenced
if "config" not in globals():
    config = {}

import random
from utils import *

def buildGraph(size, boundaryCondition):
    """Creates a graph in the form of a dictionary.
    graph is as graph[site] = {siteNeighbour0: weight0, siteNeighbour1: weight1, ...}

    :size: 1xn matrix - [N, M, ...] largest possible values in each dimension
    :boundaryCondition: String in ["periodic", "dirichlet"]
    :returns: dictionary 
    """

    graph = {}
    startingWeight = 0
    supportedBC = ["periodic", "dirichlet"]

    # I sometimes have fat fingers
    if boundaryCondition not in supportedBC:
        raise unsupportedBoundaryCondition

    # Starting index should be filled with placeholder None
    siteIndex = [None]*len(size)

    # Build the graph recursively depending on how big size is
    recGraph(size, siteIndex, graph, boundaryCondition, startingWeight)

    return graph

def applyBoundaryCondition(boundaryCondition, site, size, startingWeight):
    """Create a list of neighbours to site depending on boundaryCondition

    :boundaryCondition: String - in ["periodic", "dirichlet"]
    :site: 1xn matrix - [i, j, ...] index of site to which neighbours should be calculated
    :size: 1xn matrix - [N, M, ...] largest possible values in each dimension
    :startingWeight: Int - The weight between site and each of its neighbours
    :returns: dictionary of 1xm matrix of 1xn tuples - m = number of neighbours
    """

    neighbours = []
    for i, x_i in enumerate(site):
        if config.get("debug"):
            print(f"Handling {site} on {x_i}")
            print(f"Size is {size}")
            print(f"i is {i}")

        # Create values that are used for all boundary conditions.
        # Creates a cut out so that only the current index (x_i) can be changed.
        # This is then glued back together based on the boundary condition.
        beforeIndex = site[:i]
        afterIndex = site[(i + 1):]
        aboveSite = x_i + 1
        belowSite = x_i - 1

        if boundaryCondition == "periodic":
            # x_i here is the value on the i'th index (dimension) in site
            neighbours.append(tuple(beforeIndex + [aboveSite % size[i]] + afterIndex))
            neighbours.append(tuple(beforeIndex + [belowSite % size[i]] + afterIndex))

            if config.get("debug"):
                input(neighbours)
                print()

        elif boundaryCondition == "dirichlet":
            # x_i here is the value on the i'th index (dimension) in site
            if not aboveSite == size[i]:
                # It is not on the max border so it's safe to add
                neighbours.append(tuple(site[:i] + [(x_i + 1)] + site[(i + 1):]))

            if not belowSite == -1:
                # It is not on the bottom border so it's safe to add
                neighbours.append(tuple(site[:i] + [(x_i - 1)] + site[(i + 1):]))

            if config.get("debug"):
                input(neighbours)
                print()

    # Initialize the site
    outputDict = {}
    for neighbour in neighbours:
        outputDict[neighbour] = startingWeight
    return outputDict

def recGraph(size, currentSite, graph, boundaryCondition, startingWeight, currentForLoop=0):
    """Loop recursively and create graph

    :size: [N, M, ...] where it should be interpreted as NxMx...
    :currentSite: 1xn matrix - Initially [None][i, j, k, ...] index of the current site  to be in the graph. currentSite should start being [None]*len(size)
    :graph: dictionary - Initially {}, in each for loop currentSite will be added to it
    :boundaryCondition: String in ["periodic", "dirichlet"]
    :startingWeight: Int - The weight between site and each of its neighbours
    :currentForLoop: Int - Initially 0. This is then incremented until it size[currentForLoop] is undefined.
    :returns: None
    """

    # This is what stops the recursion
    if currentForLoop <= len(size)-1:
        for x_i in range(size[currentForLoop]):
            # Move the index associated with this for loop to x_i
            currentSite[currentForLoop] = x_i

            # Only start adding sites after all the placeholders are removed
            if None not in currentSite:
                graph[tuple(currentSite)] = applyBoundaryCondition(boundaryCondition, currentSite, size, startingWeight)
            recGraph(size, currentSite, graph, boundaryCondition, startingWeight, currentForLoop + 1)

def switchLinkBetween(site0, site1, graph):
    """Changes the link weight between site0 and site1

    :site0: 1xn matrix
    :site1: 1xn matrix
    :graph: dictionary
    :returns: None
    """
    
    # Flip the two weights
    # NOTE: There is no direction in the graph so we need to update
    #       both link between site0 and site1
    #       and between site1 and site0
    flipSiteWeight(site0, site1, graph)
    flipSiteWeight(site1, site0, graph)

def flipSiteWeight(site0, site1, graph):
    """Flips the site weight between 0 and 1

    :site0: 1xn matrix [i, j, ...]
    :site1: 1xn matrix [i, j, ...]
    :graph: dictionary
    :returns: None
    """

    # Convert sites to tuples (tuples can be used as hash values but not arrays)
    site0 = tuple(site0)
    site1 = tuple(site1)

    currentWeight = graph[site0][site1]

    graph[site0][site1] = 1 if currentWeight == 0 else 0

def getLinkWeight(site0, site1, graph):
    """Link weight between site0 and site1 in graph

    :site0: 1xn matrix [i, j, ...]
    :site1: 1xn matrix [i, j, ...]
    :graph: dictionary
    :returns: Int - link weight
    """
    
    # Convert sites to tuples (tuples can be used as hash values but not arrays)
    site0 = tuple(site0)
    site1 = tuple(site1)

    return graph[site0][site1]

def getLinkedNeighbours(site, graph):
    """All neighbours to site in graph with weight 1
    
    :site: 1xn matrix
    :graph: dictionary
    :returns: 1xm matrix of 1xn matrices
    """

    site = tuple(site)
    linkedNeighbours = []
    for neighbour in graph[site]:
        if graph[site][neighbour] == 1:
            linkedNeighbours.append(list(neighbour))

    # linkedNeighbours is as [[i, j, ...], [a, b, ...], ...]
    return linkedNeighbours

def getRandomNeighbour(site, exceptSite, graph):
    """Get random neighbour to site that is not exceptSite in graph

    :site: 1xn matrix - [i, j, ...]
    :exceptSite: 1xn matrix - [i, j, ...]
    :graph: dictionary
    :returns: 1xn matrix
    """

    # Convert sites to a tuples (tuples can be used as hash keys but not arrays)
    site = tuple(site)

    # Get all the neighbours to site
    # neighbours is as [(i, j+1), ...]
    neighbours = list(graph[site].keys())

    if exceptSite is not None:
        neighbours.remove(tuple(exceptSite))

    neighbour = random.choice(neighbours)

    # Since neighbour will be changed a lot, use a list
    return list(neighbour)

class unsupportedBoundaryCondition(Exception):
    """Unsupported boundary condition called."""

if __name__ == '__main__':
    # Load the configs
    config = loadConfigs("config.ini")

    graphDirichlet = buildGraph([3, 3], "dirichlet")
    # graphPeriodic = buildGraph(3, 3, "periodic")

    # site = [6, 5, 6, 0, 3]
    # size = [9, 6, 7, 9, 4]

    # site = [0, 3]
    # size = [4, 4]

    # boundaryCondition = "periodic"
    # boundaryCondition = "dirichlet"
    # graph = {}
    # startingWeight = 0
    # graph[tuple(site)] = applyBoundaryCondition(boundaryCondition, site, size, graph, startingWeight)
