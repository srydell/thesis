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

def buildCorr(firstSite, N, M):
    """Brute force loop through every site value and add r^2 as a key to correlation function

    :firstSite: 1x2 matrix - Starting site, [x, y]
    :N: Int - Number of rows
    :M: Int - Number of columns
    :returns: dictionary - correlation function for r^2
    """
    x0 = firstSite[0]
    x0 = firstSite[1]

    G = {}
    for i in range(list(range(x0-N, N-x0))):
        for j in range(list(range(y0-M, M-y0))):
            G[j * j + i * i] = 0

    return G

def addXToCorr(firstSite, newSite, value, corr):
    """TODO: Docstring for addXToCorr.

    :firstSite: 1x2 matrix - site to calculate corr from
    :newSite: 1x2 matrix - site to calculate corr to
    :value: Int - value to add to the key
    :corr: dictionary - Correlation function
    :returns: None
    """

    # x value
    key = firstSite[0] - newSite[0]

    if corr.get(key) == None:
        # key have not been hashed before, use 0 as starting value and then add value
        corr[key] = value
    else:
        # key has been hashed before, add value
        corr[key] += value

def addToCorr(firstSite, newSite, value, corr):
    """Add value to corr on the radius from firstSite to newSite

    :firstSite: 1x2 matrix - site to calculate corr from
    :newSite: 1x2 matrix - site to calculate corr to
    :value: Int - value to add to the key
    :corr: dictionary - Correlation function
    :returns: None
    """

    # r^2 = x^2 + y^2
    key = pow(firstSite[0] - newSite[0], 2) + pow(firstSite[1] - newSite[1], 2)

    # Faster to hash ints
    key = int(key)

    if corr.get(key) == None:
        # key have not been hashed before, use 0 as starting value and then add value
        corr[key] = value
    else:
        # key has been hashed before, add value
        corr[key] += value

def simulateWormStep(gitter, K, currentSite, previousSite, corrFunction):
    """TODO: Docstring for simulationStep.

    :gitter: TODO
    :K: TODO
    :currentSite: TODO
    :previousSite: TODO
    :returns: TODO
    """

    # Get potential next step (choose any neighbour exept previousSite)
    newSite = getRandomNeighbour(site=currentSite, exceptSite=previousSite, graph=gitter)

    if isAccepted(K, currentSite, newSite, gitter):
        colorLinkBetween(currentSite, newSite, gitter)

        previousSite = currentSite
        currentSite = newSite
        
        # Update correlation function on the fly:
        # add +1 to G(i-i0) for the open path from i0 to i
        # addIfNotExists(sum([pow(firstSite[i] - newSite[i], 2) for i in range(2)]), 1, Gr)
        addIfNotExists(firstSite[0] - newSite[0], 1, corrFunction)

    return gitter, Gr, Gx

def main(K, N, M, bc):
    """Simulate ising worm algorithm

    :K: Float - J/T
    :N: Int - Number of rows
    :M: Int - Number of columns
    :bc: String - Boundary condition in ["periodic", "dirichlet"]
    :returns: dictionary - gitter
    """

    # Initialize the random number generator with current time as seed
    random.seed()

    gitter = buildGraph(N, M, bc)

    # Initialize starting site as some random [i, j] within the gitter
    # Track the previous site to avoid that the current turns 180 degrees
    firstSite = [random.randrange(0, N), random.randrange(0, M)]
    currentSite = firstSite
    previousSite = None

    # Correlation function for r and x
    Gr = {}
    Gx = {}

    # ===========

    # clusters = {}
    # allLoop = False
    # simulateWormStep()
    # indexClusters()
    # classifyClusters()
    # while not allLoop:
    #     # Check if all clusters are loops
    #     if all(clusters[index]["end"] is None for index in clusters):
    #         allLoop = True
    #         continue

    #     else:
    #         for index in clusters:
    #             if clusters[index]["end"] is not None:
    #                 simulateWormStep(clusters[index]["end"])
    #         indexClusters()
    #         classifyClusters()
        
    # ==========

    allLoop = False
    # Simulation
    print("First site:", firstSite)
    while not allLoop:
        newSite = getRandomNeighbour(site=currentSite, exceptSite=previousSite, graph=gitter)

        if isAccepted(K, currentSite, newSite, gitter):
            colorLinkBetween(currentSite, newSite, gitter)

            previousSite = currentSite
            currentSite = newSite
            
            # Update correlation function on the fly:
            # add +1 to G(i-i0) for the open path from i0 to i
            addIfNotExists(sum([pow(firstSite[i] - newSite[i], 2) for i in range(2)]), 1, Gr)
            addIfNotExists(firstSite[0] - newSite[0], 1, Gx)
            if newSite != firstSite:
                continue
            else:
                # TODO: Update averages: add +1 to G(0), add current loop length to <L>
                # corr[site][site] += 1
                print("We found a loop")
                allLoop = True

    return gitter, Gr, Gx

if __name__ == '__main__':
    J = 0.5
    # Temperature
    T = 1

    K = J/T

    # Number of rows in gitter
    N = 300
    # Number of columns in gitter
    M = 300

    bc = "periodic"

    # Run the simulation
    gitter, G, Gx = main(K, N, M, bc)

    saveToFile = False
    if saveToFile:
        saveGraph(gitter)

