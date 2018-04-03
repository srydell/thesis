import numpy as np
import time

def colorLink(gitter, link):
    """Marks link in gitter as moved to

    :link: 2x1 matrix
    :gitter: NxM matrix
    :returns: link

    """
    if gitter[link[0]][link[1]] == 0:
        # Change the gitter
        print("Should turn to one!")
        gitter[link[0]][link[1]] = 1
    else:
        print("Should turn zero!")
        gitter[link[0]][link[1]] = 0

    return link

def isAccepted(K, linkWeight):
    """Check if the link is accepted

    :K: J/T
    :linkWeight: 1 or 0
    :returns: True if accepted and False otherwise

    """

    # Probability of being accepted
    p = np.power(np.tanh(K), 1 - linkWeight)

    if np.random.random() < p:
        return True
    else:
        return False

def getDirection(pastDirection):
    """

    :pastDirection: 1x2 matrix
    :returns: a random direction

    """

    # Directions = [right, up, left, down]
    directions = [[1, 0], [0, 1], [-1, 0], [0, -1]]

    if pastDirection:
        # Ensure that it cannot turn around instantly
        oppositeDirection = [-x for x in pastDirection]
        directions.remove(oppositeDirection)

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

def getNewLinkWithBoundaryConditions(boundaryCondition, currentLink, direction, sizeOfGitter):
    """

    :boundaryCondition: string with value in ["periodic"]
    :currentLink: 1x2 matrix
    :direction: 1x2 matrix
    :returns: newLink 1x2 matrix

    """

    # Move to currentLink + direction
    newLink = [ currentLink[0] + direction[0], currentLink[1] + direction[1] ]

    # Apply boundary conditions
    newLink = applyBoundaryConditions(newLink, sizeOfGitter, boundaryCondition)

    return newLink

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
    firstLink = [np.random.randint(N), np.random.randint(M)]
    currentLink = firstLink

    # The first link has a weight of 1
    colorLink(gitter, currentLink)

    # Number of iterations
    n = 50

    direction = []
    # Simulation
    while n > 0:
        # Get a new random direction that is not turning 180 degrees
        direction = getDirection(direction)

        newLink = getNewLinkWithBoundaryConditions("periodic", currentLink, direction, sizeOfGitter)

        # Move in the new direction if accepted otherwise stay
        # probability tanh(K) to be accepted if link in direction has weight 1
        # and probability 1 otherwise
        if isAccepted(K, gitter[newLink[0]][newLink[1]]):
            if currentLink == newLink:
                print("We found a loop")
                break

            # Return link after it has been colored ( 0 <-> 1 )
            currentLink = colorLink(gitter, newLink)
        # Else currentLink stays the same

        # TODO: Update correlation function on the fly:
        #       Add +1 to G(i - i0) for the open path from i0 to i

        # Decrement number of iterations
        n -= 1

        print()
        print()
        print()
        print(gitter)
        print()
        time.sleep(1)

if __name__ == '__main__':
    main()
