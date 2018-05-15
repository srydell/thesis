# Ensure that isingWormConfig can be referenced
if "isingWormConfig" not in globals():
    isingWormConfig = {}

import sys
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

    linkWeight = gitter[site0][site1]

    # Probability of being accepted
    p = pow(tanh(K), 1 - linkWeight)

    if random.random() < p:
        return True
    else:
        return False

def saveFrame(obj, typeOfObj, seed, size, boundaryCondition, numberOfFrames):
    """ Save obj to ./data/{NxMx...}_{boundaryCondition}/{seed}/{typeOfObj}/frameNumber.pickle
    Wrapper around saveObject

    :obj: Something to save
    :typeOfObj: String - One of ["gitter", "correlation_function", "clusters"]
    :seed: Int
    :size: 1xn matrix - [N, M, ...]
    :boundaryCondition: String
    :returns: None
    """
    if typeOfObj not in ["gitter", "correlation_function", "clusters"]:
        raise Exception(f"Type of object to save in saveFrame not supported. Recieved {typeOfObj}.")

    # sizeSeparatedByX = NxMx...
    sizeSeparatedByX = 'x'.join(str(s) for s in size)
    path = f"data/{sizeSeparatedByX}_{boundaryCondition}/{seed}/{typeOfObj}/{numberOfFrames}"

    # Save the next frame
    saveObject(obj, path)

def updateCorrFunc(firstSite, nextSite, corrFunction):
    """Update the correlation function

    :corrFunction: dictionary
    :nextSite: 1xn matrix
    :startSite: 1xn matrix
    :returns: None
    """
    # add +1 to G(i-i0) for the open path from i0 to i
    # NOTE: This has to be the absolute value,
    #       otherwise it will be skewed toward the side with the largest number of sites.
    addIfNotExists(abs(firstSite[0] - nextSite[0]), 1, corrFunction)

def updateLoopLengths(clusters, graph):
    """Append the number of sites in clusters with index to loopLengths

    :loopLengths: 1xn matrix - list of loop lengths
    :index: Int - cluster index
    :clusters: dictionary - {index: listOfSitesInCluster, ...}
    :clusters: dictionary
    :returns: None
    """
    loopLengths = []
    for index in clusters:
        currentLength = 0
        for site in clusters[index]:
            # Get all links from each site in that cluster
            currentLength += len(getLinkedNeighbours(site, graph))
        # Have to divide by 2 to avoid double counting
        loopLengths.append(currentLength/2)
    return loopLengths

def simulateWorm(corrFunction, K, size, gitter, seed, numberOfFrames):
    """Start a new worm and moves it until a loop is formed
    Chooses any neighbour from currentSite except previousSite to avoid moving 180 degrees.
    Updates the correlation function corrFunction after each step.

    :K: J/T
    :size: 1xn matrix - [N, M, ...]
    :currentSite: 1x2 matrix
    :gitter: dictionary
    :seed: Int
    :numberOfFrames: Int - The number of frames that have been saved
    :returns: 1xn matrix - loop lengths
    """

    # Initialize starting site as some random [i, j, ...] within the gitter
    firstSite = [random.randrange(0, maxX) for maxX in size]

    if isingWormConfig.get("debug"):
        print(f"First site: {firstSite}")

    # Get some random neighbour to form the first link
    currentSite = getRandomNeighbour(firstSite, None, gitter)
    switchLinkBetween(currentSite, firstSite, gitter)

    if isingWormConfig.get("debug"):
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

            if isingWormConfig.get("debug"):
                print(f"Accepted site {nextSite}")
                print(f"Coloring from {currentSite} to {nextSite}")

            # Flip the weight between currentSite and nextSite
            switchLinkBetween(currentSite, nextSite, gitter)

            previousSite = currentSite
            currentSite = nextSite

            if isingWormConfig.get("debug"):
                print(f"Updating indexing...")

            if isingWormConfig.get("plotting"):
                # Update indexing
                # By updating the cluster every frame,
                # we can use the indices to put nice colors on the plot
                # pass
                clusters = {}
                indexClusters(clusters, gitter)

            if isingWormConfig.get("debug"):
                print(f"The cluster is now: {clusters}")

            if nextSite == firstSite:
                # Found a new loop
                loopFormed = True
                # Update indexing
                clusters = {}
                indexClusters(clusters, gitter)

                loopLengths = updateLoopLengths(clusters, gitter)

                if isingWormConfig.get("debug"):
                    print(f"Loop lengths after updating: {loopLengths}")

            # Update the correlation function when new site is accepted
            updateCorrFunc(firstSite, nextSite, corrFunction)

            if isingWormConfig.get("save data"):
                # Save the frame
                saveFrame(corrFunction, "gitter", seed, size, boundaryCondition, numberOfFrames)
                saveFrame(corrFunction, "correlation_function", seed, size, boundaryCondition, numberOfFrames)
                saveFrame(clusters, "clusters", seed, size, boundaryCondition, numberOfFrames)
            numberOfFrames += 1

            if isingWormConfig.get("plotting"):
                # Plotting
                plotGraph(clusters, gitter)
                # Correlation function is normalized
                # by the number of sites in the x direction = size[0]
                plotCorr(corrFunction, size[0])

    return loopLengths

def main():
    """Simulate ising worm algorithm

    :returns: gitter, corrFunction, averageLoopLength, seed
    """

    # Initialize the random number generator with current time as seed
    seed = random.randrange(sys.maxsize)

    # Self-eating loop (resulting in clusters = {})
    # seed = 5540102676881230539
    # seed = 1592744071574553462

    # 4 small clusters, ~ 20 frames
    seed = 595770392852380573

    # Produces a nice gif. loopLengths = [4, 4, 12, 4] for 10x10 graph
    # seed = 615885798301355417

    random.seed(seed)
    # print(f"The seed is: {seed}")

    # Current the isingWormConfig
    # print(isingWormConfig)

    J = 0.5
    # Temperature
    T = 1

    K = J/T

    # Number of rows in gitter
    N = 10
    # Number of columns in gitter
    M = 10
    size = [N, M]

    boundaryCondition = isingWormConfig.get("boundary condition")
    gitter = buildGraph(size, boundaryCondition)

    # Correlation function for x
    corrFunction = {}

    # How many frames have been played
    numberOfFrames = 0
    for _ in range(4):
        # Move this worm until it forms a loop
        loopLengths = simulateWorm(corrFunction, K, size, gitter, seed, numberOfFrames)

    if len(loopLengths) != 0:
        # Update the average loop length
        averageLoopLength = sum(loopLengths)/len(loopLengths)
    else:
        raise Exception(f"There were no loops. Loop lengths: {loopLengths}")

    if isingWormConfig.get("debug"):
        print(f"Loop lengths: {loopLengths}")
        print(f"Loop averages: {averageLoopLength}")

    return gitter, corrFunction, averageLoopLength, seed

if __name__ == '__main__':
    # Load the configs
    isingWormConfig = loadConfigs("config.ini")

    # Run the simulation
    gitter, corrFunction, averageLoopLength, seed = main() 
    # Keep the window open when done
    plt.show()

    if isingWormConfig.get("debug"):
        print(f"The seed was: {seed}")
