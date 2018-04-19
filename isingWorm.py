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

def getLastTwoSites(clusterInfo, gitter):
    """Gets the current and previous site by checking the state of the cluster ends
    If one of the ends only has one neighbour, then that neighbour is the previousSite and the end is the currentSite.
    If there are more neighbours, TODO

    :clusterInfo: dictionary - {"ends": listOfEnds, sites: listOfSitesInCluster}
    :gitter: dictionary
    :returns: 2 1x2 matrix - Coordinates of the current and previous site
    """

    for end in clusterInfo["ends"]:
        neighbours = getLinkedNeighbours(end, gitter)
        if len(neighbours) == 1:
            return end, neighbours.pop()

def simulateWormStep(K, corrFunction, currentSite, firstSite, gitter, previousSite):
    """Moves worm head from currentSite to some new site if isAccepted.
    Chooses any neighbour from currentSite except previousSite to avoid moving 180 degrees.
    Updates the correlation function corrFunction after each step.

    :K: TODO
    :corrFunction: TODO
    :currentSite: TODO
    :firstSite: 
    :gitter: TODO
    :previousSite: TODO
    :returns: TODO
    """

    # Get potential next step (choose any neighbour exept previousSite)
    newSite = getRandomNeighbour(site=currentSite, exceptSite=previousSite, graph=gitter)

    if isAccepted(K, currentSite, newSite, gitter):
        colorLinkBetween(currentSite, newSite, gitter)

        # Update correlation function on the fly:
        # add +1 to G(i-i0) for the open path from i0 to i
        # addIfNotExists(sum([pow(firstSite[i] - newSite[i], 2) for i in range(2)]), 1, Gr)
        addIfNotExists(firstSite[0] - newSite[0], 1, corrFunction)

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
    # Track the previous site to avoid that the current turns 180 degrees
    previousSites = {1: firstSite}
    # Get some random neighbour to form the first link
    currentSite = getRandomNeighbour(firstSite, None, gitter)

    # Correlation function for r and x
    # Gr = {}
    Gx = {}

    clusters = {}
    allClustersAreLoops = False
    colorLinkBetween(currentSite, firstSite, gitter)

    # Initialize clusters
    indexClusters(clusters, gitter)
    classifyClusters(clusters, gitter)

    while not allClustersAreLoops:
        # Check if all clusters are loops
        if all(clusters[index]["ends"] == [] for index in clusters):
            allClustersAreLoops = True

        else:
            for index in clusters:
                if clusters[index]["ends"] != []:
                    wormHeads = clusters[index]["ends"]
                    # currentSite = clusters[index]["ends"]
                    # currentSite only has one neighbour (it is an ends of a cluster) NOTE: NOT TRUE CAN HAVE 3 NEIGHBOURS
                    currentSite, previousSite = getLastTwoSites(clusters[index], gitter)
                    print(f"Previous site: {previousSite}")
                    input(f"Current site: {currentSite}")
                    simulateWormStep(K, Gx, currentSite, firstSite, gitter, previousSite[0])
            indexClusters(clusters, gitter)
            classifyClusters(clusters, gitter)

    return gitter, Gx

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
    gitter, Gx = main(K, N, M, boundaryCondition) 
    saveToFile = False
    if saveToFile:
        saveGraph(gitter)
