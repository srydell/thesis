# Master thesis of Simon Rydell

## Todo list for exwork ##

### Questions for Mats ###

### Paper ###

* Create a directory for latex sources.
* Read one of the given example papers.
* Write up the overall section structure.

### isingWorm ###

* Write test code for isingWorm
* Correlation function should only take absolute values. Otherwise it will be skewed for Dirichlet boundary contitions. This is because if the first site is chosen, for example, left of the middle, there will be more sites to the right, and the correlation function will have a bias in this direction.
* Create data saving mechanism:
```
data
\-- sizeXxsizeY_boundaryCondition
	+-- clusters
		\-- frame1
	+-- correlation_function
		\-- frame1
	\-- graph
		\-- frame1
	\-- plot
		\-- lastFrame.png
```

### graphs ###

* Write test code for graphs
* Rewrite save graphs to saving SQL table. (Call each table NxMboundaryCondition and add a row for each data point)
* Rewrite load graphs to load from SQL table
* buildGraph should take a 1xn matrix of size and then use recursion to add the number of for loops
* Make a separate function called applyBoundaryCondition that gets used in buildGraph.

### plotGraph ###

* Write test code for plotGraph
* plot graph (option for file or live)
* Add functionality to plot from file.
* Add functionality to choose boundary conditions (now only supports dirichlet).

### indexClusters ###

* Some clusters seem to get the same index even though they are not part of the same cluster. This has showed up on some plots via the coloring (taken from the indices in clusters) being the same.

### Hoshen Kopelman ###

* If there is time, write the Hoshen Kopelman algorithm for indexing clusters.
