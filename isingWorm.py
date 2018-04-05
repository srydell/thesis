from graphFunctions import *
from math import tanh, pow

def isAccepted(K, site0, site1, gitter):
    """ Check if the link between site0 and site1 in gitter is accepted

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

def main():
    """ Simulate ising worm algorithm
    :returns: dictionary - gitter
    """

    J = 0.5
    # Temperature
    T = 1

    K = J/T

    # Number of rows in gitter
    N = 300
    # Number of columns in gitter
    M = 300
    gitterSize = (N, M)
    gitter = buildGraph(N, M, bc="periodic")
    
    # Initialize starting site as some random [i, j] within the gitter
    firstSite = [random.randrange(0, N), random.randrange(0, M)]
    currentSite = firstSite
    # Track the previous site to avoid that the current turns 180 degrees
    previousSite = None

    # Initialize the random number generator with current time as seed
    random.seed()

    # Number of iterations
    n = 50

    loop = False
    # Simulation
    # while n > 0:
    print("First site:", firstSite)
    while not loop:
        # n -= 1
        newSite = getRandomNeighbour(site=currentSite, exceptSite=previousSite, graph=gitter)

        if isAccepted(K, currentSite, newSite, gitter):
            colorLinkBetween(currentSite, newSite, gitter)

            previousSite = currentSite
            currentSite = newSite
            
            # TODO: Update correlation function on the fly:
            #       add +1 to G(i-i0) for the open path from i0 to i
            if newSite != firstSite:
                continue
            else:
                # TODO: Update averages: add +1 to G(0) add current loop length to <L>
                print("We found a loop")
                loop = True

    return gitter

if __name__ == '__main__':
    gitter = main()
