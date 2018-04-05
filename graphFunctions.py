import random

def buildGraph(N, M, bc="periodic"):
    """Creates a graph in the form of a dictionary.
    graph is as graph[site] = [[siteNeighbour0, weight0], [siteNeighbour1, weight1], ...]

    :N: Int - Number of rows
    :M: Int - Number of columns
    :bc: String in ["periodic", "dirichlet"]
    :returns: dictionary 
    """

    graph = {}
    weight = 0
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
                # Must be arrays since passed into a function to be modified
                right = [i, j+1]
                up = [i-1, j]
                left = [i, j-1]
                down = [i+1, j]
                neighbours = [right, up, left, down]

                removeOnBorder(neighbours, N, M)

            # Initialize the site
            graph[(i, j)] = []
            for neighbour in neighbours:
                graph[(i, j)].append([tuple(neighbour), weight])

            # Should now look like:
            # graph[(i, j)] = [ [right, weight], [up, weight], [left, weight], [down, weight] ]

    return graph

def colorLinkBetween(site0, site1, graph):
    """Changes the link weight between site0 and site1

    :site0: 1x2 matrix
    :site1: 1x2 matrix
    :graph: dictionary
    :returns: None
    """
    
    # Convert sites to tuples (tuples can be used as hash values but not arrays)
    site0 = tuple(site0)
    site1 = tuple(site1)

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

    # Iterate over graph to find which weight to change
    for n, siteData in enumerate(graph[site0]):
        # siteData looks like [neighbourSite, weight]
        if siteData[0] == site1:
            # Flip the weight
            graph[site0][n][1] = 1 if siteData[1]==0 else 0

def getLinkWeight(site0, site1, graph):
    """ Link weight between site0 and site1 in graph
    :site0: 1x2 matrix [i, j]
    :site1: 1x2 matrix [i, j]
    :graph: dictionary
    :returns: Int - link weight
    """
    
    # Convert sites to tuples (tuples can be used as hash values but not arrays)
    site0 = tuple(site0)
    site1 = tuple(site1)

    # Iterate over graph to find which weight to change
    for n, siteData in enumerate(graph[site0]):
        # siteData looks like [neighbourSite, weight]
        if siteData[0] == site1:
            # Get the weight
            weight = graph[site0][n][1]
    return weight

def getLinkedNeighbours(site, graph):
    """ All neighbours to site in graph with weight 1
    :site: 1x2 matrix
    :graph: dictionary
    :returns: 1xn matrix of 1x2 matrices
    """

    linkedNeighbours = []
    for neighbour in graph[site]:
        weight = neighbour[1]
        if weight == 1:
            linkedNeighbours.append(list(neighbour[0]))

    # linkedNeighbours is as [[x, y], [z, w], ...]
    return linkedNeighbours

def getRandomNeighbour(site, exceptSite, graph):
    """ Get random neighbour to site that is not exceptSite in graph
    :site: 1x2 matrix - [i, j]
    :exceptSite: 1x2 matrix - [i, j]
    :graph: dictionary
    :returns: 1x2 matrix
    """

    # Convert sites to a tuples (tuples can be used as hash values but not arrays)
    site = tuple(site)

    # graph[site] gives a list of all neighbours on the form [(neighbourIndices), linkWeight]
    neighboursData = graph[site]

    if exceptSite is not None:
        exceptSite = tuple(exceptSite)
        # Remove exceptSite from neighbourData
        for neighbour in neighboursData:
            # neighbour is as [(i, j), weight]
            if neighbour[0] == exceptSite:
                neighboursData.remove(neighbour)

    neighbour = random.choice(neighboursData)[0]

    # Since neighbour will be changed a lot use a list
    return list(neighbour)

def isOnBorder(site, N, M):
    """ True if one of [-1, N, M] can be found in site, False otherwise
    :site: 1x2 matrix
    :N: Int
    :M: Int
    :returns: Boolean
    """
    
    return -1 in site or N in site or M in site

def removeOnBorder(sites, N, M):
    """ Remove site from sites if site is outside of NxM graph
    :sites: 1xn matrix of 1x2 matrices
    :N: int
    :M: int
    :returns: None
    """

    toBeDeleted = []

    # Cannot remove directly in loop since the array is being iterated upon
    for site in sites:
        if isOnBorder(site, N, M):
            toBeDeleted.append(site)

    # Since sites is an array,
    # this will affect the array passed to the function
    for borderSite in toBeDeleted:
        sites.remove(borderSite)

class unsupportedBoundaryCondition(Exception):
    """Unsupported boundary condition called."""

if __name__ == '__main__':
    graphDirichlet = buildGraph(2, 2, "dirichlet")
    graphPeriodic = buildGraph(2, 2, "periodic")
    # buildGraph(3, 3, "dirichlet")
    # print(graphSize)
    # print(graph)
