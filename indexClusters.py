from graphs import getLinkedNeighbours, buildGraph, colorLinkBetween
import copy

def getIndex(listOfSites, graph):
    """Get the "index" property in graph for each site in listOfSites

    :listOfSites: 1xn matrix - On the form [[i, j], [i+1, j], ...]
    :graph: dictionary
    :returns: 1xn matrix - Indices on the form [1, 3, ...]
    """

    indices = []
    for site in listOfSites:
        indices.append(graph[tuple(site)]["index"])

    return indices

def setIndex(index, listOfSites, graph):
    """Get the "index" property in graph for each site in listOfSites

    :index: Int - The index that will be set
    :listOfSites: 1xn matrix - On the form [[i, j], [i+1, j], ...]
    :graph: dictionary
    :returns: None
    """
    
    for site in listOfSites:
        graph[tuple(site)]["index"] = index

def addIfNotExists(key, value, dictionary):
    """Adds the entry (key: value) to dictionary if not exists
    If it dictionary[key] exists and is a list, append value to it

    :key: Something hashable
    :value: Some value to be stored
    :dictionary: dictionary
    :returns: None
    """

    if dictionary.get(key) is None:
        dictionary[key] = value
        return

    elif type(dictionary[key]) == int or type(dictionary[key]) == float:
        dictionary[key] += value
        return

    elif type(dictionary[key]) == list:
        # Only append if it is not already there
        if value not in dictionary[key]:
            dictionary[key].append(value)

def compareList(value, cList):
    """Checks if every entry in cList is equal to value

    :value: Int
    :cList: 1xn matrix - list to compare on the form [2, 1, ...]
    :returns: Boolean
    """

    return all(listVal == value for listVal in cList)

def removeDeprecated(clusters, graph):
    """Goes through each key in clusters and check if the site still has that index in the graph

    :clusters: dictionary
    :graph: dictionary
    :returns: None
    """

    toBeRemoved = []
    emptyClusterIndices = []
    # clusters is as: {index0: [site0, site1, ...], index1: ...}
    for index in clusters:
        for site in clusters[index]["sites"]:
            # If the index has changed (e.g. it was overwritten for something smaller)
            if graph[site]["index"] != index:
                toBeRemoved.append(site)

        # No longer looping over the list so it's safe to remove the sites
        for site in toBeRemoved:
            clusters[index]["sites"].remove(site)

        if clusters[index]["sites"] == []:
            emptyClusterIndices.append(index)
        # For the next cluster to be inspected
        toBeRemoved = []

    # Cleanup empty clusters
    for index in emptyClusterIndices:
        clusters.pop(index)

def visualizeIndex(graph, N, M):
    """Creates a matrix to visualize the indices

    :graph: dictionary
    :N: numRows
    :M: numCols
    :returns: NxM matrix
    """

    matrix = []
    for i in range(N):
        row = []
        for j in range(M):
            row.append(graph[(i, j)]["index"])
        matrix.append(row)

    return matrix

def classifyClusters(clusters, graph):
    """Find the cluster ends and loops in graph in clusters
    An ends is a site that has an odd number of linked neighbours
    A loop is where every site in the cluster has an even number of linked neighbours
    Finds endSite and modifies clusters as: {index: {'ends': endSite, 'sites': listOfSites}, ...}
    If no ends has been found, it stays as None

    :clusters: dictionary - {index: {"ends": None, "sites": listOfSites}, ...}
    :graph: dictionary
    :returns: None
    """

    for index in clusters:
        # Will be {site0: numOfTimesFound, site1: ...}
        checked = {}
        for site in clusters[index]["sites"]:
            neighbours = getLinkedNeighbours(site, graph)
            for neighbour in neighbours:
                neighbour = tuple(neighbour)
                if checked.get(neighbour) is None:
                    # If it has not been found before
                    checked[neighbour] = 1
                else:
                    # It has been found before, add +1
                    checked[neighbour] += 1

        for site in checked:
            # If we found an ends (site with odd number of neighbours)
            # append it to the list of ends
            if checked[site] % 2 == 1:
                addIfNotExists("ends", site, clusters[index])

        # Lastly, if any ends are now part of a loop - remove them
        endsToRemove = []
        for site in clusters[index]["ends"]:
            if len(getLinkedNeighbours(site, graph)) % 2 == 0:
                endsToRemove.append(site)

        # Remove the sites
        for site in endsToRemove:
            clusters[index]["ends"].remove(site)

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
                # Get a list of all the indices from the neighbours and the current site
                clusterIndices = getIndex([site, *neighbours], graph)

                # If we have found a totally new cluster (all indices are 0)
                if compareList(0, clusterIndices):
                    indexHasChanged = True
                    largestIndex += 1
                    # Add a new cluster list to clusters
                    addIfNotExists(largestIndex, {"ends": [], "sites": [site]}, clusters)
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
                    # Add a new cluster list to clusters
                    setIndex(minClusterIndex, [site, *neighbours], graph)

                # Always add the site to the cluster dictionary
                addIfNotExists("sites", site, clusters[minClusterIndex])

            # If it has no linked neighbours it should always have index 0
            else:
                graph[site]["index"] = 0

    # Remove deprecated sites from clusters
    removeDeprecated(clusters, graph)

if __name__ == '__main__':
    numRows = 3
    numCols = 4

    g1 = buildGraph(numRows, numCols, "dirichlet")
    g2 = buildGraph(numRows, numCols, "dirichlet")

    # One cluster
    colorLinkBetween([0, 0], [1, 0], g1)
    colorLinkBetween([1, 0], [1, 1], g1)
    colorLinkBetween([1, 1], [1, 2], g1)
    colorLinkBetween([1, 2], [0, 2], g1)
    colorLinkBetween([0, 2], [0, 3], g1)

    # Another cluster
    colorLinkBetween([2, 0], [2, 1], g1)
    colorLinkBetween([2, 1], [2, 2], g1)

    # Another cluster
    colorLinkBetween([0, 0], [0, 1], g2)
    colorLinkBetween([0, 1], [1, 1], g2)
    colorLinkBetween([1, 1], [1, 0], g2)
    colorLinkBetween([1, 0], [0, 0], g2)

    clusters = {}
    clusterWithLoop = {}

    indexClusters(clusters, g1)
    classifyClusters(clusters, g1)

    indexClusters(clusterWithLoop, g2)
    classifyClusters(clusterWithLoop, g2)

    # matrix* is a matrix showing only the indices of each site
    matrixg1 = visualizeIndex(g1, numRows, numCols)
    matrixg2 = visualizeIndex(g2, numRows, numCols)

    # Close the loop in g1
    colorLinkBetween([0, 3], [1, 3], g1)
    indexClusters(clusters, g1)
    classifyClusters(clusters, g1)

    colorLinkBetween([2, 3], [1, 3], g1)
    indexClusters(clusters, g1)
    classifyClusters(clusters, g1)

    colorLinkBetween([2, 3], [2, 2], g1)
    indexClusters(clusters, g1)
    classifyClusters(clusters, g1)

    colorLinkBetween([1, 0], [2, 0], g1)
    indexClusters(clusters, g1)
    classifyClusters(clusters, g1)

    colorLinkBetween([1, 0], [0, 0], g1)
    indexClusters(clusters, g1)
    classifyClusters(clusters, g1)
    matrixg1 = visualizeIndex(g1, numRows, numCols)
