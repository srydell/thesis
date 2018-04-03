import random

def buildGraph(N, M):
    """Creates a graph in the form of a dictionary

    :N: Number of rows
    :M: Number of columns
    :returns: graph dictionary in the form graph[site] = [[siteNeighbour0, weight0], [siteNeighbour1, weight1], ...] and graph size

    """
    graphSize = (N, M)
    graph = {}
    weight = 0
    
    for i in range(N):
        for j in range(M):
            # Directions in the gitter
            right = (i, (j+1)%M)
            up = ((i-1)%N, j)
            left = (i, (j-1)%M)
            down = ((i+1)%N, j)

            graph[(i, j)] = [ [right, weight], [up, weight], [left, weight], [down, weight] ]

    return graph, graphSize

def getLinkWeight(site0, site1, graph):
    """

    :site0: 1x2 matrix [i, j]
    :site1: 1x2 matrix [i, j]
    :graph: dictionary
    :returns: link weight between site0 and site1

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

def getRandomNeighbour(site, graph):
    """

    :site: 1x2 matrix [i, j]
    :graph: dictionary
    :returns: 1x2 matrix, neighbour from site in graph [i, j]

    """

    # Convert site to a tuple (tuples can be used as hash values but not arrays)
    site = tuple(site)

    # graph[site] gives a list of all neighbours on the form [[neighbourIndices], linkWeight]
    neighbour = random.choice(graph[site])[0]

    # Since neighbour will be changed a lot use a list
    return list(neighbour)

if __name__ == '__main__':
    graph, graphSize = buildGraph(3, 3)
    # print(graphSize)
    # print(graph)
