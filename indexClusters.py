from graphs import getLinkedNeighbours, buildGraph, switchLinkBetween
DEBUG=False

def removeDeprecated(clusters):
    """Goes through each index in clusters and check if it has any sites

    :clusters: dictionary
    :returns: None
    """

    emptyClusterIndices = []
    for index in clusters:
        if clusters[index] == []:
            emptyClusterIndices.append(index)

    # Cleanup empty clusters
    for index in emptyClusterIndices:
        clusters.pop(index)

def getIndex(sites, clusters):
    """Gets a list of indices for each site in sites
    If there is only one site, it returns an int

    :sites: TODO
    :clusters: TODO
    :returns: TODO
    """

    onlyOneSite = all(type(entry) == int for entry in sites)
    indices = []
    smallestIndex = None
    for index in clusters:
        if onlyOneSite:
            if sites in clusters[index]:
                return index
            else:
                continue

        else:
            for site in sites:
                if site in clusters[index]:
                    indices.append(index)

    # If could'nt find site
    if onlyOneSite:
        return None

    return indices

def moveToIndex(newIndex, listOfSites, clusters):
    """Moves all sites in listOfSites from their previous index to newIndex

    :newIndex: TODO
    :listOfSites: TODO
    :clusters: TODO
    :returns: TODO
    """

    removeFromCluster = {}
    for index in clusters:
        for site in listOfSites:
            if site in clusters[index]:
                # If site in the wrong index
                if index != newIndex:
                    clusters[index].remove(site)

            # Append to the new cluster
            if index == newIndex:
                if site not in clusters[index]:
                    clusters[index].append(site)

def notIndexed(listOfSites, clusters):
    """Checks if any of the sites in listOfSites has an index clusters

    :listOfSites: TODO
    :clusters: TODO
    :returns: TODO
    """

    for site in listOfSites:
        for index in clusters:
            if site in clusters[index]:
                return False
    return True

def indexClusters(clusters, graph):
    """Loop through the graph and index the clusters,
    also adds the site where every new cluster is found to clusters
    In the ends it removes all deprecated sites in clusters (with indices that have been overwritten)

    :clusters: dictionary
    :graph: dictionary
    :returns: None
    """

    largestIndex = 0
    indexHasChanged = True

    while indexHasChanged:
        indexHasChanged = False
        for site in graph:
            neighbours = getLinkedNeighbours(site, graph)

            # If there are any links
            if neighbours != []:
                # Make each neighbour hashable
                localCluster = [list(site), *neighbours]
                if DEBUG:
                    print(f"On site {site}, will process: {localCluster}")

                # None of the sites in the local cluster have been indexed
                if notIndexed(localCluster, clusters):
                    indexHasChanged = True
                    largestIndex += 1
                    # Add the sites to the new index
                    if DEBUG:
                        print(f"Setting index {largestIndex} on {localCluster}")
                        print(f"Cluster before setting:\n{clusters}")
                    clusters[largestIndex] = localCluster

                    if DEBUG:
                        input(f"Cluster after setting:\n{clusters}")

                    # Go to next site
                    continue
                    
                # Only gets indices that are set in clusters
                localClusterIndices = getIndex(localCluster, clusters)
                # Get the smallest of them that is not 0
                smallestIndex = min(localClusterIndices)
                if DEBUG:
                    print(f"Indices found: {localClusterIndices}")
                    print(f"Smallest index: {smallestIndex}")

                numSitesNotIndexed = len(localCluster) - len(localClusterIndices)

                # Pad indices with zeros for each site not indexed
                localClusterIndices += [0]*numSitesNotIndexed

                allHaveMinIndex = all(index == smallestIndex for index in localClusterIndices)
                if DEBUG:
                    print(f"The indices are: {localClusterIndices}")
                    print(f"The smallest index is: {smallestIndex}")
                    input(f"All have this index: {allHaveMinIndex}")

                if not allHaveMinIndex:
                    # Not all neighbours have the same index
                    indexHasChanged = True

                    if DEBUG:
                        print(f"Move sites {localCluster} to index {smallestIndex}")
                        print(f"Cluster before moving:\n{clusters}")
                    moveToIndex(smallestIndex, localCluster, clusters)
                    if DEBUG:
                        input(f"Cluster after moving:\n{clusters}")
            else:
                # If it has no neighbours it should not exist in clusters.
                # This can happen if it is overridden by itself or another cluster.
                for index in clusters:
                    if site in clusters[index]:
                        clusters[index].remove(site)

    # Remove deprecated sites from clusters
    if DEBUG:
        print(f"Removing empty indices. Cluster is now:\n{clusters}")
    removeDeprecated(clusters)
    if DEBUG:
        print(f"Done. Cluster is now:\n{clusters}")

def testDataToMats(N, M, fileIndex):
    """Get test data to mats. Write to file test_data_i.txt, where i is an integer.
    x = [random values 0 and 1 in range(N*M)]
    y = [random values 0 and 1 in range(N*M)]
    Order clusters in number of sites in cluster
    example:
    clusters: 
        1: 40
        2: 37
        3: 28
        ...
        
    :N: Int - Number of rows
    :M: Int - Number of columns
    :returns: None
    """
    import random
    import operator

    # Link in +x direction
    x = [random.randint(0, 1) for i in range(N*M)]
    # Link in +y direction
    y = [random.randint(0, 1) for j in range(N*M)]

    g = buildGraph(N, M, "periodic")

    for n, site in enumerate(g):
        if x[n]:
            switchLinkBetween(site, (site[0], (site[1]+1)%M), g)
        if y[n]:
            switchLinkBetween(site, ((site[0]+1)%N, site[1]), g)
    clusters = {}
    indexClusters(clusters, g)

    for index in clusters:
        clusters[index] = len(clusters[index])

    # [indexWithLargestValue, indexWithNextToLargestKey, ...]
    sortedIndices = sorted(clusters, key=clusters.get, reverse=True)

    with open(f"./dataToMats/index_cluster_data_{fileIndex}_size_{N}x{M}.txt", "w") as f:
        f.write(f"x={x}\n")
        f.write(f"y={y}\n")

        f.write("\n")
        for index in sortedIndices:
            f.write(f"{index}: {clusters[index]}\n")

if __name__ == '__main__':
    numRows = 20
    numCols = 20

    # Test data to mats for checking indexClusters function
    # for n in range(10):
    #     testDataToMats(numRows, numCols, n)

    # g1 = buildGraph(numRows, numCols, "dirichlet")
    # g2 = buildGraph(numRows, numCols, "dirichlet")

    # # One cluster for g1
    # switchLinkBetween([0, 0], [1, 0], g1)
    # switchLinkBetween([1, 0], [1, 1], g1)
    # switchLinkBetween([1, 1], [1, 2], g1)
    # switchLinkBetween([1, 2], [0, 2], g1)
    # switchLinkBetween([0, 2], [0, 3], g1)

    # # Another cluster for g1
    # switchLinkBetween([2, 0], [2, 1], g1)
    # switchLinkBetween([2, 1], [2, 2], g1)

    # # Cluster for g2
    # switchLinkBetween([0, 0], [0, 1], g2)
    # switchLinkBetween([0, 1], [1, 1], g2)
    # switchLinkBetween([1, 1], [1, 0], g2)
    # switchLinkBetween([1, 0], [0, 0], g2)

    # clusters = {}
    # clusterWithLoop = {}

    # indexClusters(clusters, g1)

    # # indexClusters(clusterWithLoop, g2)

    # # Close the loop in g1
    # switchLinkBetween([0, 3], [1, 3], g1)
    # indexClusters(clusters, g1)

    # switchLinkBetween([2, 3], [1, 3], g1)
    # indexClusters(clusters, g1)

    # switchLinkBetween([2, 3], [2, 2], g1)
    # indexClusters(clusters, g1)

    # switchLinkBetween([1, 0], [2, 0], g1)
    # indexClusters(clusters, g1)

    # switchLinkBetween([1, 0], [0, 0], g1)
    # indexClusters(clusters, g1)
