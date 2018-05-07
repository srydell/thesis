def union(i, j):
    """TODO: Docstring for union.

    :i: TODO
    :j: TODO
    :returns: TODO

    """
    pass

def find(i):
    """TODO: Docstring for find.

    :i: TODO
    :returns: TODO

    """
    pass

largest_label = 0
for i in range(N):
    for j in range(M):
        # TODO: Look so these actually exists
        leftNode = (i-1, j)
        aboveNode = (i, j-1)
        # .get() return None if not in dictionary
        leftLabel = 0 if [i, j] in [[0, 0], [N, 0]] else graph.get(leftNode).get("label")
        aboveLabel = 0 if [i, j] in [[0, 0], [0, M]] else graph.get(aboveNode).get("label")

        # Neither a label above nor to the left.
        if (leftLabel == 0) and (aboveLabel == 0)
            # Make a new, as-yet-unused cluster label.
            largest_label = largest_label + 1
            graph[(i,j)]["label"] = largest_label;

        elif (leftLabel != 0) and (aboveLabel == 0)
            # One neighbor, to the left.
            graph[(i,j)]["label"] = find(leftLabel);

        elif (leftLabel == 0) and (aboveLabel != 0)
            # One neighbor, above.
            graph[(i,j)]["label"] = find(aboveLabel);

        else
            # Neighbors BOTH to the left and above.
            union(leftLabel,aboveLabel)
            # Link the left and above clusters.
            graph[(i,j)]["label"] = find(leftLabel);
