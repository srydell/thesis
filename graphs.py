import random
import pickle

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

def isOnBorder(site, N, M):
    """True if one of [-1, N, M] can be found in site, False otherwise

    :site: 1x2 matrix
    :N: Int
    :M: Int
    :returns: Boolean
    """
    
    return -1 in site or N == site[0] or M == site[1]

class unsupportedBoundaryCondition(Exception):
    """Unsupported boundary condition called."""

if __name__ == '__main__':
    graphDirichlet = buildGraph(3, 3, "dirichlet")
    graphPeriodic = buildGraph(3, 3, "periodic")
