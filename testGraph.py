def buildNonRecursiveGraph(N, M, bc="periodic"):
    """Creates a graph in the form of a dictionary.
    graph is as graph[site] = {siteNeighbour0: weight0, siteNeighbour1: weight1, ...}
    :N: Int - Number of rows
    :M: Int - Number of columns
    :bc: String in ["periodic", "dirichlet"]
    :returns: dictionary 
    """

    graph = {}
    startingWeight = 0
    supportedBC = ["periodic", "dirichlet"]

    # I sometimes have fat fingers
    if bc not in supportedBC:
        raise unsupportedBoundaryCondition
    
    for i in range(N):
        for j in range(M):
            # Create an array of neighbours from (i, j) the graph
            if bc == "periodic":
                right = (i, (j+1)%M)
                up = ((i-1)%N, j)
                left = (i, (j-1)%M)
                down = ((i+1)%N, j)
                neighbours = [right, up, left, down]

            elif bc == "dirichlet":
                # Must be arrays since passed into a function
                right = [i, j+1]
                up = [i-1, j]
                left = [i, j-1]
                down = [i+1, j]
                neighbours = []

                for site in [right, up, left, down]:
                    if not isOnBorder(site, N, M):
                        neighbours.append(site)

            # Initialize the site
            graph[(i, j)] = {}
            for neighbour in neighbours:
                graph[(i, j)][tuple(neighbour)] = startingWeight

            # Should now look like:
			# graph[(i, j)] = {right: startingWeight, up: startingWeight, left: startingWeight, down: startingWeight}

    return graph
