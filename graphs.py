import random

def buildGraph(N, M, bc="periodic"):
    """Creates a graph in the form of a dictionary.
    graph is as graph[site] = {siteNeighbour0: weight0, siteNeighbour1: weight1, ...}

    :N: Int - Number of rows
    :M: Int - Number of columns
    :bc: String in ["periodic", "dirichlet"]
    :returns: dictionary 
    """

    graph = {}
    startingWeight = 0
    supportedBC = ["periodic", "dirichlet"]

    # I sometimes have fat fingers
    if bc not in supportedBC:
        raise unsupportedBoundaryCondition

    for i in range(N):
        for j in range(M):
            # Create an array of neighbours from (i, j) the graph
            if bc == "periodic":
                right = (i, (j+1)%M)
                up = ((i-1)%N, j)
                left = (i, (j-1)%M)
                down = ((i+1)%N, j)
                neighbours = [right, up, left, down]

            elif bc == "dirichlet":
                # Must be arrays since passed into a function
                right = [i, j+1]
                up = [i-1, j]
                left = [i, j-1]
                down = [i+1, j]
                neighbours = []

                for site in [right, up, left, down]:
                    if not isOnBorder(site, N, M):
                        neighbours.append(site)

            # Initialize the site
            graph[(i, j)] = {}
            for neighbour in neighbours:
                graph[(i, j)][tuple(neighbour)] = startingWeight

            # Should now look like:
            # graph[(i, j)] = {right: startingWeight, up: startingWeight, left: startingWeight, down: startingWeight}

    return graph

def applyBoundaryCondition(boundaryCondition, site, size, graph, startingWeight):
    """Create a list of neighbours to site depending on boundaryCondition

    :boundaryCondition: String - in ["periodic", "dirichlet"]
    :site: 1xn matrix - [i, j, ...] index of site to which neighbours should be calculated
    :size: 1xn matrix - [N, M, ...] largest possible values in each dimension
    :startingWeight: Int - The weight between site and each of its neighbours
    :returns: dictionary of 1xm matrix of 1xn tuples - m = number of neighbours
    """

    neighbours = []
    for i, x_i in enumerate(site):
        if boundaryCondition == "periodic":
            # x_i here is the value on the i'th index (dimension) in site
            print(f"Handling {site} on {x_i}")
            print(f"Size is {size}")
            print(f"i is {i}")
            neighbours.append(tuple(site[:i] + [(x_i + 1) % size[i]] + site[(i + 1):]))
            neighbours.append(tuple(site[:i] + [(x_i - 1) % size[i]] + site[(i + 1):]))
            input(neighbours)
            print()

        elif boundaryCondition == "dirichlet":
            # x_i here is the value on the i'th index (dimension) in site
            print(f"Handling {site} on {x_i}")
            print(f"Size is {size}")
            print(f"i is {i}")
            if not x_i+1 == size[i]:
                # It is not on the max border so it's safe to add
                neighbours.append(tuple(site[:i] + [(x_i + 1)] + site[(i + 1):]))
            if not x_i-1 == -1:
                # It is not on the bottom border so it's safe to add
                neighbours.append(tuple(site[:i] + [(x_i - 1)] + site[(i + 1):]))

            input(neighbours)
            print()

    # Initialize the site
    outputDict = {}
    for neighbour in neighbours:
        outputDict[neighbour] = startingWeight
    return outputDict

def recGraph(size, siteIndex, graph, currentForLoop=0):
    """Loop recursively and create graph

    :size: [N, M, ...] where it should be interpreted as NxMx...
    :siteIndex: 1xn matrix - Initially [None][i, j, k, ...] index of the current site  to be in the graph. siteIndex should start being [None]*len(size)
    :graph: dictionary - Initially {}, in each for loop siteIndex will be added to it
    :currentForLoop: Int - Initially 0. This is then incremented until it size[currentForLoop] is undefined.
    :returns: None
    """

    # This is what stops the recursion
    if currentForLoop <= len(size)-1:
        for x_i in range(size[currentForLoop]):
            siteIndex[currentForLoop] = x_i

            if None not in siteIndex:
                print(siteIndex)
                # TODO: Add applyBoundaryCondition(size, siteIndex) here
                graph[tuple(siteIndex)] = applyBoundaryCondition(boundaryCondition, siteIndex, size, graph)

            print(f"Now on {siteIndex}")
            recGraph(size, siteIndex, graph, currentForLoop + 1)

def switchLinkBetween(site0, site1, graph):
    """Changes the link weight between site0 and site1

    :site0: 1x2 matrix
    :site1: 1x2 matrix
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

    :site0: 1x2 matrix [i, j]
    :site1: 1x2 matrix [i, j]
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

    :site0: 1x2 matrix [i, j]
    :site1: 1x2 matrix [i, j]
    :graph: dictionary
    :returns: Int - link weight
    """
    
    # Convert sites to tuples (tuples can be used as hash values but not arrays)
    site0 = tuple(site0)
    site1 = tuple(site1)

    return graph[site0][site1]

def getLinkedNeighbours(site, graph):
    """All neighbours to site in graph with weight 1
    
    :site: 1x2 matrix
    :graph: dictionary
    :returns: 1xn matrix of 1x2 matrices
    """

    site = tuple(site)
    linkedNeighbours = []
    for neighbour in graph[site]:
        if graph[site][neighbour] == 1:
            linkedNeighbours.append(list(neighbour))

    # linkedNeighbours is as [[x, y], [z, w], ...]
    return linkedNeighbours

def getRandomNeighbour(site, exceptSite, graph):
    """Get random neighbour to site that is not exceptSite in graph

    :site: 1x2 matrix - [i, j]
    :exceptSite: 1x2 matrix - [i, j]
    :graph: dictionary
    :returns: 1x2 matrix
    """

    # Convert sites to a tuples (tuples can be used as hash keys but not arrays)
    site = tuple(site)

    # Get all the neighbours to site
    # neighbours is as [(i, j+1), ...]
    neighbours = list(graph[site].keys())

    if exceptSite is not None:
        neighbours.remove(tuple(exceptSite))

    neighbour = random.choice(neighbours)

    # Since neighbour will be changed a lot use a list
    return list(neighbour)

def isOnBorder(site, size):
    """True if one of [-1 in site, size[0] == site[0], size[1] ==...] is true, False otherwise

    :site: 1xn matrix
    :size: 1xn matrix
    :returns: Boolean
    """
    for i, x_i in enumerate(site):
        if x_i == -1 or x_i == size[i]:
            return True
    return False

class unsupportedBoundaryCondition(Exception):
    """Unsupported boundary condition called."""

if __name__ == '__main__':
    # graphDirichlet = buildGraph(3, 3, "dirichlet")
    # graphPeriodic = buildGraph(3, 3, "periodic")

    # site = [6, 5, 6, 0, 3]
    # size = [9, 6, 7, 9, 4]
    site = [0, 3]
    size = [4, 4]
    boundaryCondition = "periodic"
    # boundaryCondition = "dirichlet"
    graph = {}
    startingWeight = 0
    graph[tuple(site)] = applyBoundaryCondition(boundaryCondition, site, size, graph, startingWeight)
    print(graph)
    # g = {}
    # bc = "dirichlet"
    # size = [1, 2]
    # index = [0]*len(size)
    # numloop = len(size)-1
    # recGraph(size, g, bc, index, numloop)

