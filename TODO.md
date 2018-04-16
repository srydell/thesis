# Master thesis of Simon Rydell

## Todo list for exwork ##

### Questions for Mats ###

### isingWorm ###

* Write test code for isingWorm
* Add check to see if all loops are closed
* Loop over all the heads of each worm and move one step for each
* Write more general code for adding new thing to dictionary (correlation function). I have written one in indexClusters called addIfNotExists. Adapt this to fit for the correlation function.

### graphs ###

* Write test code for graphs

### plotGraph ###

* Write test code for plotGraph
* Rewrite save graphs to saving SQL table. (Call each table NxMboundaryCondition and add a row for each data point)
* Rewrite load graphs to load from SQL table
* plot graph (option for file or live)
* Put plotting configs into its own function and let it take graphSize as arg to set axis

### indexClusters ###

* Write some function that finds all heads of each worm in clusters
