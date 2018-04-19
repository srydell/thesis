from graphs import *
from indexClusters import *
from math import tanh, pow

def isAccepted(K, site0, site1, gitter):
    """Check if the link between site0 and site1 in gitter is accepted

    :K: Float - J/T
    :site0: 1x2 matrix - [i, j]
    :site1: 1x2 matrix - [i, j]
    :gitter: dictionary
    :returns: Boolean - True if accepted and False otherwise
    """

    # Convert sites to a tuples (tuples can be used as hash values but not arrays)
    site0 = tuple(site0)
    site1 = tuple(site1)

    linkWeight = getLinkWeight(site0, site1, gitter)

    # Probability of being accepted
    p = pow(tanh(K), 1 - linkWeight)

    if random.random() < p:
        return True
    else:
        return False

def getLastTwoSites(clusterInfo, gitter, index, startSites):
    """Gets the current and previous site by checking the state of the cluster ends
    If one of the ends only has one neighbour, then that neighbour is the previousSite and the end is the currentSite.
    If there are more neighbours, TODO

    :clusterInfo: dictionary - {"ends": listOfEnds, sites: listOfSitesInCluster}
    :gitter: dictionary
    :returns: 2 1x2 matrix - Coordinates of the current and previous site
    """

    # If we got a new cluster let one of the ends be the start site
    if startSites.get(index) is None:
        startSites[index] = clusters["ends"][0]

    for end in clusterInfo["ends"]:
        if end != startSites[index]:
            neighbours = getLinkedNeighbours(end, gitter)
            if len(neighbours) == 1:
                return end, neighbours.pop()

def updateCorrFunc(firstSite, nextSite, corrFunction):
    """Updates the correlation function

    :corrFunction: dictionary
    :nextSite: 1x2 matrix
    :startSite: 1x2 matrix
    :returns: None
    """
    # Update correlation function on the fly:
    # add +1 to G(i-i0) for the open path from i0 to i
    addIfNotExists(firstSite[0] - nextSite[0], 1, corrFunction)

    # For correlation function in r
    # addIfNotExists(sum([pow(firstSite[i] - nextSite[i], 2) for i in range(2)]), 1, Gr)

def updateLoopLengths(loopLengths, index, clusters):
    """Appends the number of sites in clusters with index to loopLengths

    :loopLengths: 1xn matrix - list of loop lengths
    :index: Int - cluster index
    :clusters: dictionary - {index: {"ends": listOfEndSites, "sites": listOfSitesInCluster}, ...}
    :returns: None
    """
    loopLengths.append(len(clusters[index]["sites"]))

def simulateWormStep(K, currentSite, gitter, previousSite):
    """Moves worm head from currentSite to some new site if isAccepted.
    Chooses any neighbour from currentSite except previousSite to avoid moving 180 degrees.
    Updates the correlation function corrFunction after each step.

    :K: TODO
    :currentSite: TODO
    :gitter: TODO
    :previousSite: TODO
    :returns: TODO
    """

    # Get potential next step (choose any neighbour exept previousSite)
    nextSite = getRandomNeighbour(site=currentSite, exceptSite=previousSite, graph=gitter)

    if isAccepted(K, currentSite, nextSite, gitter):
        colorLinkBetween(currentSite, nextSite, gitter)

        return nextSite

    return currentSite

def main(K, N, M, boundaryCondition):
    """Simulate ising worm algorithm

    :K: Float - J/T
    :N: Int - Number of rows
    :M: Int - Number of columns
    :boundaryCondition: String - Supported: ["periodic", "dirichlet"]
    :returns: dictionaries - gitter, correlation function
    """

    # Initialize the random number generator with current time as seed
    random.seed()

    gitter = buildGraph(N, M, boundaryCondition)

    # Initialize starting site as some random [i, j] within the gitter
    firstSite = [random.randrange(0, N), random.randrange(0, M)]
    print(f"First site: {firstSite}")

    # Get some random neighbour to form the first link
    currentSite = getRandomNeighbour(firstSite, None, gitter)
    colorLinkBetween(currentSite, firstSite, gitter)

    # Track the previous site to avoid that the current turns 180 degrees
    previousSite = firstSite

    # Correlation function for r and x
    # Gr = {}
    Gx = {}

    # Loop lengths
    loopLengths = []

    # Initialize clusters
    clusters = {}
    indexClusters(clusters, gitter)
    classifyClusters(clusters, gitter)

    allClustersAreLoops = False
    while not allClustersAreLoops:
        # Get potential next step (choose any neighbour exept previousSite)
        nextSite = getRandomNeighbour(site=currentSite, exceptSite=previousSite, graph=gitter)

        if isAccepted(K, currentSite, nextSite, gitter):
            # Flip the weight between currentSite and nextSite
            colorLinkBetween(currentSite, nextSite, gitter)

            previousSite = currentSite
            currentSite = nextSite

        if nextSite == firstSite:
            # Found a new loop
            allClustersAreLoops = True
            updateLoopLengths(loopLengths, gitter[tuple(firstSite)]["index"], clusters)

        # NOTE: This should always runs, even when not accepted
        updateCorrFunc(firstSite, nextSite, Gx)

        # Update indexing
        indexClusters(clusters, gitter)
        # Update ends in clusters (classify clusters as loops and open clusters)
        classifyClusters(clusters, gitter)

    averageLoopLength = [l/len(loopLengths) for l in loopLengths]

    return gitter, Gx, clusters, averageLoopLength

if __name__ == '__main__':
    J = 0.5
    # Temperature
    T = 1

    K = J/T

    # Number of rows in gitter
    N = 4
    # Number of columns in gitter
    M = 4

    boundaryCondition = "dirichlet"

    # Run the simulation
    gitter, Gx, clusters, averageLoopLength = main(K, N, M, boundaryCondition) 
    print(f"Found a loop: {clusters}")
    if len(clusters[1]["ends"]) != 0:
        site1 = clusters[1]["ends"][0]
        site2 = clusters[1]["ends"][1]
        print(f"First weird site: {site1} - {gitter[site1]}")
        print(f"Second weird site: {site2} - {gitter[site2]}")
    saveToFile = False
    if saveToFile:
        saveGraph(gitter)
