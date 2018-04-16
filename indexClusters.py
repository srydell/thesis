def compareList(value, cList):
    """Checks if every entry in cList is equal to value

    :value: Int
    :cList: 1xn matrix - list to compare on the form [2, 1, ...]
    :returns: Boolean
    """

    for listValue in cList:
        isAllSame = listValue == value

    return isAllSame

def indexClusters(graph):
    """Loop through the graph and index the clusters

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
                # Get a list of all the indices from the neighbours and the current site
                clusterIndices = getIndex([site, *neighbours], graph)

                # If we have found a totally new cluster (all indices are 0)
                if compareList(0, clusterIndices):
                    indexHasChanged = True
                    largestIndex += 1
                    setIndex(largestIndex, [site, *neighbours], graph)
                    # Go to next in the for loop through sites
                    continue

                # Get the smallest of them that is not 0
                minClusterIndex = min([index for index in clusterIndices if index != 0])

                # allHaveMinIndex = True if all values in clusterIndices are minClusterIndex, False otherwise
                allHaveMinIndex = compareList(minClusterIndex, clusterIndices)

                # Here we know that minClusterIndex is not 0
                # but not all neighbours have the same index, so change them 
                if not allHaveMinIndex:
                    indexHasChanged = True
                    setIndex(minClusterIndex, [site, *neighbours], graph)
                    # Go to next in the for loop through sites
                    continue
