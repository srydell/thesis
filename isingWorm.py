import sys
SAVEFRAMES = False
from math import tanh, pow
from indexClusters import *
from graphs import *
from plotGraph import *
from utils import *

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

def saveFrame(obj, typeOfObj, seed, N, M, boundaryCondition, numberOfFrames):
    """ Save obj to ./data/{N}x{M}_{boundaryCondition}/{seed}/{typeOfObj}/frameNumber.pickle

    :obj: Something to save
    :typeOfObj: String - One of ["gitter", "correlation_function", "clusters"]
    :seed: Int
    :N: Int - Number of rows
    :M: Int - Number of columns
    :boundaryCondition: String
    :returns: None
    """
    if typeOfObj not in ["gitter", "correlation_function", "clusters"]:
        raise Exception(f"Type of object to save in saveFrame not supported. Recieved {typeOfObj}.")

    path = f"data/{N}x{M}_{boundaryCondition}/{seed}/{typeOfObj}/{numberOfFrames}"

    # Save the next frame
    saveObject(obj, path)

def updateCorrFunc(firstSite, nextSite, corrFunction):
    """Update the correlation function

    :corrFunction: dictionary
    :nextSite: 1x2 matrix
    :startSite: 1x2 matrix
    :returns: None
    """
    # Update correlation function on the fly:
    # add +1 to G(i-i0) for the open path from i0 to i
    addIfNotExists(firstSite[0] - nextSite[0], 1, corrFunction)

def updateLoopLengths(clusters):
    """Append the number of sites in clusters with index to loopLengths

    :loopLengths: 1xn matrix - list of loop lengths
    :index: Int - cluster index
    :clusters: dictionary - {index: listOfSitesInCluster, ...}
    :returns: None
    """
    loopLengths = []
    for index in clusters:
        loopLengths.append(len(clusters[index]))
    return loopLengths

def simulateWorm(corrFunction, K, N, M, gitter, seed, numberOfFrames):
    """Start a new worm and moves it until a loop is formed
    Chooses any neighbour from currentSite except previousSite to avoid moving 180 degrees.
    Updates the correlation function corrFunction after each step.

    :K: J/T
    :M: Int - Number of columns
    :N: Int - Number of rows
    :currentSite: 1x2 matrix
    :gitter: dictionary
    :previousSite: 1x2 matrix
    :returns: 1xn matrix - loop lengths
    """

    # Initialize starting site as some random [i, j] within the gitter
    firstSite = [random.randrange(0, N), random.randrange(0, M)]

    if DEBUG:
        print(f"First site: {firstSite}")

    # Get some random neighbour to form the first link
    currentSite = getRandomNeighbour(firstSite, None, gitter)
    switchLinkBetween(currentSite, firstSite, gitter)

    if DEBUG:
        print(f"Initial color link between {firstSite} and {currentSite}")

    # Track the previous site to avoid that the current turns 180 degrees
    previousSite = firstSite

    clusters = {}
    indexClusters(clusters, gitter)

    updateCorrFunc(firstSite, currentSite, corrFunction)

    loopFormed = False
    while not loopFormed:
        # Get potential next step (choose any neighbour exept previousSite)
        nextSite = getRandomNeighbour(site=currentSite, exceptSite=previousSite, graph=gitter)

        if isAccepted(K, currentSite, nextSite, gitter):

            if DEBUG:
                print(f"Accepted site {nextSite}")
                print(f"Coloring from {currentSite} to {nextSite}")

            # Flip the weight between currentSite and nextSite
            switchLinkBetween(currentSite, nextSite, gitter)

            previousSite = currentSite
            currentSite = nextSite

            if DEBUG:
                print(f"Updating indexing...")

            # Update indexing
            clusters = {}
            indexClusters(clusters, gitter)

            if DEBUG:
                print(f"The cluster is now: {clusters}")

            if nextSite == firstSite:
                # Found a new loop
                loopFormed = True

                if DEBUG:
                    print(f"Found a looop on index {indexOfLoop}")
                    print(f"Loop lengths before updating: {loopLengths}")

                loopLengths = updateLoopLengths(clusters)

                if DEBUG:
                    print(f"Loop lengths after updating: {loopLengths}")

            # Update the correlation function when new site is accepted
            updateCorrFunc(firstSite, nextSite, corrFunction)

            if SAVEFRAMES:
                # Save the frame
                saveFrame(corrFunction, "gitter", seed, N, M, boundaryCondition, numberOfFrames)
                saveFrame(corrFunction, "correlation_function", seed, N, M, boundaryCondition, numberOfFrames)
                saveFrame(clusters, "clusters", seed, N, M, boundaryCondition, numberOfFrames)
            numberOfFrames += 1

            # Plotting
            plotGraph(clusters, gitter)
            plotCorr(corrFunction, M)

    return loopLengths

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

    # Self-eating loop (resulting in clusters = {})
    seed = 5540102676881230539
    # seed = 1592744071574553462

    # Multiple loops but only one gets indexed
    # seed = 595770392852380573

    # Produces a nice gif. loopLengths = [4, 4, 12, 4]
    # seed = 615885798301355417

    random.seed(seed)
    print(f"The seed is: {seed}")

    gitter = buildGraph(N, M, boundaryCondition)

    # Correlation function for x
    corrFunction = {}

    # How many frames have been played
    numberOfFrames = 0
    for _ in range(2):
        # Move this worm until it forms a loop
        loopLengths = simulateWorm(corrFunction, K, N, M, gitter, seed, numberOfFrames)

    if len(loopLengths) != 0:
        # Update the average loop length
        averageLoopLength = sum(loopLengths)/len(loopLengths)
    else:
        raise Exception(f"There were no loops. Loop lengths: {loopLengths}")

    if DEBUG:
        print(f"Loop lengths: {loopLengths}")
        print(f"Loop averages: {averageLoopLength}")

    return gitter, corrFunction, averageLoopLength, seed

if __name__ == '__main__':
    J = 0.5
    # Temperature
    T = 1

    K = J/T

    # Number of rows in gitter
    N = 10
    # Number of columns in gitter
    M = 10

    boundaryCondition = "dirichlet"
    # boundaryCondition = "periodic"

    # Run the simulation
    gitter, corrFunction, averageLoopLength, seed = main(K, N, M, boundaryCondition) 
    # Keep the window open when done
    plt.show()
    if DEBUG:
        print(f"The seed was: {seed}")
