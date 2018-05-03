# Master thesis of Simon Rydell

## Todo list for exwork ##

### Questions for Mats ###

### Paper ###

* Create a directory for latex sources.
* Read one of the given example papers.
* Write up the overall section structure.

### isingWorm ###

* Write test code for isingWorm
* The correlation function does not seem to go down like r^(-1/4). Check if this is because I am using too small values.
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

### plotGraph ###

* Write test code for plotGraph
* Make it so that plotGraph does not reuse colors as much. Make a temporary array of colors to pop from and reset it every run.
* Add functionality to plot from file.
* Add functionality to choose boundary conditions (now only supports dirichlet).

### indexClusters ###

* Some clusters seem to get the same index even though they are not part of the same cluster. This has showed up on some plots via the coloring (taken from the indices in clusters) being the same. Might be a problem with how the coloring is chosen.

### Hoshen Kopelman ###

* If there is time, write the Hoshen Kopelman algorithm for indexing clusters.

### utils ###
