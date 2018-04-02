import numpy as np

def colorLink(gitter, link):
    """Marks link in gitter as moved to

    :link: 2x1 matrix
    :gitter: NxM matrix
    :returns: link

    """
    # Change the gitter
    gitter[link[0]][link[1]] = 1 if gitter[link[0]][link[1]] == 0 else 0

    return link

def isAccepted(K, linkWeight):
    """Check if the link is accepted

    :K: J/T
    :linkWeight: 1 or 0
    :returns: True if accepted and False otherwise

    """

    # Probability of being accepted
    p = np.power(np.tanh(K), 1 - linkWeight)

    if np.random.random() > p:
        return True
    else:
        return False

def getDirection():
    """
    :returns: a random new direction

    """

    # Directions = [right, up, left, down]
    directions = [(1, 0), (0, 1), (-1, 0), (0, -1)]

    return directions[np.random.randint(len(directions))]

def applyBoundaryConditions(link, sizeOfGitter, boundaryCondition):
    """Applies a boundary condition specified by boundaryCondition

    :link: 1x2 matrix
    :sizeOfGitter: 1x2 matrix
    :boundaryCondition: String with possible values "periodic"
    :returns: link after boundary condition is applied

    """

    if boundaryCondition == "periodic":
        newLink = [ link[0] % sizeOfGitter[0], link[1] % sizeOfGitter[1] ]
        return newLink

def moveDirectionIfAccepted(currentLink, direction, gitter, K, sizeOfGitter):
    """Move current in currentLink in the direction direction

    :currentLink: 1x2 matrix
    :direction: 1x2 matrix
    :gitter: NxM matrix
    :K: J/T number
    :sizeOfGitter: 1x2 tuple
    :returns: old link if rejected, link in direction if accepted

    """

    # Move to currentLink + direction
    newLink = [ currentLink[0] + direction[0], currentLink[1] + direction[1] ]

    # Apply boundary conditions
    newLink = applyBoundaryConditions(newLink, sizeOfGitter, "periodic")

    if isAccepted(K, gitter[newLink[0]][newLink[1]]):
        # Return link after it has been colored ( 0 <-> 1 )
        return colorLink(gitter, newLink)
    else:
        # Return link without doing anything
        return currentLink

def main():
    """Main simulation function
    """

    J = 0.5
    # Temperature
    T = 1

    K = J/T

    # Number of rows in gitter
    N = 10
    # Number of columns in gitter
    M = 8
    sizeOfGitter = (N, M)
    
    # Initialize the gitter as NxM matrix
    gitter = np.zeros(sizeOfGitter)

    # Initialize starting link as some random [x, y] within the gitter
    currentLink = [np.random.randint(N), np.random.randint(M)]

    # The first link has a weight of 1
    colorLink(gitter, currentLink)

    # Number of iterations
    n = 2

    # Simulation
    while n > 0:
        print("The current link is:")
        print(currentLink)
        print("The gitter is:")
        print(gitter)
        print()

        # Get a new random direction
        direction = getDirection()

        # Move in the new direction if accepted otherwise stay
        currentLink = moveDirectionIfAccepted(currentLink, direction, gitter, K, sizeOfGitter)

        # Decrement number of iterations
        n -= 1

if __name__ == '__main__':
    main()
