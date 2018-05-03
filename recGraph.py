# Loop recursively and create graph
# size is [N, M, K, ...] where it should be interpreted as NxMxKx...
# n is what stops the recursion. It should always start at 0. This is then incremented until it size[n] is undefined.
# index is [i, j, k, ...] the index of the current site that will be handled in the graph. index should start being [None]*len(size).
# graph is a dictionary that should be {} at start. In each loop the site from index will be added to it.
def recGraph(size, index, graph, n):
    if n <= len(size)-1:
        for x in range(size[n]):
            index[n] = x

            if None not in index:
                print(index)
                # TODO: Add applyBoundaryCondition(size, index) here
                graph[tuple(index)] = 0

            print(f"Now on {index}")
            recGraph(size, index, graph, n + 1)

# Example input
size = [2, 3]
index = [None]*len(size)
graph = {}
recGraph(size, index, graph, 0)

print(graph)
print(len(graph))

# TODO: Add this to graphs.py
