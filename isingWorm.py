import sys
from math import tanh, pow
# from indexClusters import *
from removeends import *
from graphs import *

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
    :clusters: dictionary - {index: listOfSitesInCluster, ...}
    :returns: None
    """
    loopLengths.append(len(clusters[index]))

def simulateWorm(corrFunction, loopLengths, K, clusters, gitter):
    """Starts a new worm and moves it until a loop is formed
    Chooses any neighbour from currentSite except previousSite to avoid moving 180 degrees.
    Updates the correlation function corrFunction after each step.

    :K: TODO
    :currentSite: TODO
    :gitter: TODO
    :previousSite: TODO
    :returns: TODO
    """
    # Initialize starting site as some random [i, j] within the gitter
    firstSite = [random.randrange(0, N), random.randrange(0, M)]
    print(f"First site: {firstSite}")

    # Get some random neighbour to form the first link
    currentSite = getRandomNeighbour(firstSite, None, gitter)
    colorLinkBetween(currentSite, firstSite, gitter)
    print(f"Initial color link between {firstSite} and {currentSite}")

    # Track the previous site to avoid that the current turns 180 degrees
    previousSite = firstSite

    indexClusters(clusters, gitter)

    loopFormed = False
    while not loopFormed:
        # Get potential next step (choose any neighbour exept previousSite)
        nextSite = getRandomNeighbour(site=currentSite, exceptSite=previousSite, graph=gitter)

        if isAccepted(K, currentSite, nextSite, gitter):
            print(f"Accepted site {nextSite}")
            print(f"Coloring from {currentSite} to {nextSite}")
            # Flip the weight between currentSite and nextSite
            colorLinkBetween(currentSite, nextSite, gitter)

            previousSite = currentSite
            currentSite = nextSite

            if nextSite == firstSite:
                # Found a new loop
                loopFormed = True
                indexOfLoop = getIndex(firstSite, clusters)
                print(f"Found a looop on index {indexOfLoop}")
                print(f"Loop lengths before updating: {loopLengths}")
                updateLoopLengths(loopLengths, indexOfLoop, clusters)
                print(f"Loop lengths after updating: {loopLengths}")

            # Update indexing
            print(f"Updating indexing...")
            indexClusters(clusters, gitter)
            input(f"The cluster is now: {clusters}")

        # NOTE: This should always runs, even when not accepted
        updateCorrFunc(firstSite, nextSite, corrFunction)

def main(K, N, M, boundaryCondition):
    """Simulate ising worm algorithm

    :K: Float - J/T
    :N: Int - Number of rows
    :M: Int - Number of columns
    :boundaryCondition: String - Supported: ["periodic", "dirichlet"]
    :returns: dictionaries - gitter, correlation function
    """

    # Initialize the random number generator with current time as seed
    seed = random.randrange(sys.maxsize)
    # This seed produces a self-eating loop (resulting in clusters = {})
    # seed = 5540102676881230539
    seed = 1592744071574553462
    random.seed(seed)

    gitter = buildGraph(N, M, boundaryCondition)

    # Correlation function for r and x
    # Gr = {}
    corrFunction = {}

    # Loop lengths
    loopLengths = []

    # Initialize clusters
    clusters = {}

    for _ in range(2):
        # Move this worm until it forms a loop
        simulateWorm(corrFunction, loopLengths, K, clusters, gitter)

    # Update the average loop length
    averageLoopLength = sum(loopLengths)/len(loopLengths)

    print(f"Loop lengths: {loopLengths}")
    print(f"Loop averages: {averageLoopLength}")

    return gitter, corrFunction, clusters, averageLoopLength, seed

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
    gitter, corrFunction, clusters, averageLoopLength, seed = main(K, N, M, boundaryCondition) 
    print(f"Found a loop: {clusters}")
    print(f"The seed was: {seed}")

    SAVETOFILE = False
    if SAVETOFILE:
        saveGraph(gitter)
