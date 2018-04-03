from graphFunctions import *
from math import tanh, pow

def isAccepted(K, site0, site1, gitter):
    """Check if the link is accepted

    :K: J/T
    :site0: 1x2 matrix [i, j]
    :site1: 1x2 matrix [i, j]
    :gitter: dictionary
    :returns: True if accepted and False otherwise

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
    """Main simulation function
    :returns: gitter

    """

    J = 0.5
    # Temperature
    T = 1

    K = J/T

    # Number of rows in gitter
    N = 3
    # Number of columns in gitter
    M = 3
    gitter, gitterSize = buildGraph(N, M)
    
    # Initialize starting site as some random [i, j] within the gitter
    firstSite = [random.randrange(0, N), random.randrange(0, M)]
    currentSite = firstSite

    # Initialize the random number generator with current time as seed
    random.seed()

    # Number of iterations
    n = 50

    previousDirection = None

    loop = False
    # Simulation
    # while n > 0:
    print("First site:", firstSite)
    while not loop:
        n -= 1

        newDirection, newSite = getRandomNeighbour(currentSite, gitter, previousDirection)

        if isAccepted(K, currentSite, newSite, gitter):
            colorLinkBetween(currentSite, newSite, gitter)

            currentSite = newSite
            print("current site:", currentSite)
            
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
