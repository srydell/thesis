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

def findClusterLoopsAndEnds(clusters, graph):
    """Find the cluster end and loops in graph in clusters
    An end is a site that is found an odd number of times in all linked neighbours in the cluster
    A loop is where every site in the cluster is found an even amount of times in all linked neighbours
    Modifies clusters so that it is as: {index: {'end': endSite, 'sites': listOfSites}, ...}
    If enSite is None, then that cluster is a loop

    :clusters: dictionary - {index: listOfSites, ...}
    :graph: dictionary
    :returns: None
    """


def indexClusters(clusters, graph):
    """Loop through the graph and index the clusters,
    also adds the site where every new cluster is found to clusters
    In the end it removes all deprecated sites in clusters (with indices that have been overwritten)

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
            # input(f"{site} has {neighbours} to go through.")

            # If there are any links
            if neighbours != []:
                # Get a list of all the indices from the neighbours and the current site
                clusterIndices = getIndex([site, *neighbours], graph)
                # input(f"Got indices {clusterIndices} from sites {[site, *neighbours]}")

                # If we have found a totally new cluster (all indices are 0)
                if compareList(0, clusterIndices):
                    indexHasChanged = True
                    largestIndex += 1
                    # print(f"Setting {largestIndex} on {[site, *neighbours]}")
                    # print(f"Creating new entry into clusters: {largestIndex}: 'end': None, 'sites': {[site]}")
                    # Add a new cluster list to clusters
                    addIfNotExists(largestIndex, {"end": None, "sites": [site]}, clusters)
                    setIndex(largestIndex, [site, *neighbours], graph)
                    # print(f"Now clusters look like: {clusters}")
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
                    # print(f"Not all clusters have the smallest index {minClusterIndex}")
                    # print(f"Setting {minClusterIndex} on {[site, *neighbours]}")
                    # print(f"Adding site {site} to dict {clusters[minClusterIndex]}")
                    # Add a new cluster list to clusters
                    setIndex(minClusterIndex, [site, *neighbours], graph)

                # Always add the site to the cluster dictionary
                addIfNotExists("sites", site, clusters[minClusterIndex])
                # print(f"Now clusters look like: {clusters}")

    # Remove deprecated sites from clusters
    print(clusters)
    input(" ")
    removeDeprecated(clusters, graph)
    print(clusters)
    input(" ")

if __name__ == '__main__':
    numRows = 3
    numCols = 4
    clusterGraph = buildGraph(numRows, numCols, "dirichlet")
    # One cluster
    colorLinkBetween([0, 0], [1, 0], clusterGraph)
    colorLinkBetween([1, 0], [1, 1], clusterGraph)
    colorLinkBetween([1, 1], [1, 2], clusterGraph)
    colorLinkBetween([1, 2], [0, 2], clusterGraph)
    colorLinkBetween([0, 2], [0, 3], clusterGraph)

    # Another cluster
    colorLinkBetween([2, 0], [2, 1], clusterGraph)
    colorLinkBetween([2, 1], [2, 2], clusterGraph)

    clusters = {}
    before = copy.deepcopy(clusterGraph)
    indexClusters(clusters, clusterGraph)
    after = clusterGraph

    # index* is a matrix showing only the indices of each site
    indexBefore = visualizeIndex(before, numRows, numCols)
    indexAfter = visualizeIndex(after, numRows, numCols)

