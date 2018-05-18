# Master thesis of Simon Rydell

## Todo list for exwork ##

### Questions for Mats ###

### Paper ###

* Read one of the given example papers.
* Add information about [Hausdorff dimension](https://en.wikipedia.org/wiki/Hausdorff_dimension)
* Add information about [Raster scan](https://en.wikipedia.org/wiki/Raster_scan)
* Add information about [worm algorithm](./literature/worm_algorithm_lecturenotes.pdf)
* Add derivation of interesting quantities in Ising worm [derivations](./literature/worm_derivations.pdf)
* Maybe add information about how the length of the loops are calculated, count each linked neighbour in a cluster and then divide by 2
* The periodic boundary conditions come from the fact that we are interested in a large system. This is a 'cheap' way of simulating this.
* Add a section describing Monte Carlo simulations chapter ~6
* Add a subsection describing detailed balance

### Tips from Emil ###

* 'Flatten' out the graph by putting into a vector where the index of each site is (i * N + j) (this is always unique for 2D)
* Append the state configuration to the data to avoid keeping two files. (std::copy)

### isingWorm.py ###

* Write test code for isingWorm
* The correlation function does not seem to go down like r^(-1/4). Check if this is because I am using too small values.
* Recalculate the way <L>. It is a weighted average as sum{tanh^L(K) * L}/ sum{tanh^L(K)}
* Calulate the error bars with jackknife method. Can look at New Scaling Method for the Kosterlitz-Thouless Transititon in the section Pre- and post-processing (it is a master thesis given as an example by Mats).

### graphs.py ###

* Write test code for graphs
* Rewrite save graphs to saving SQL table. (Call each table NxMboundaryCondition and add a row for each data point)
* Rewrite load graphs to load from SQL table

### plotGraph.py ###

* Write test code for plotGraph
* plotGraph can now only use 2D. Let it take size as an input at depending on if it is len(size) 2 or 3 it uses different plotting techniques.
* Add so it plots loop lengths or perhaps energy.
* Make it so that plotGraph does not reuse colors as much. Make a temporary array of colors to pop from and reset it every run.
* Add functionality to plot from file.
* Add functionality to choose boundary conditions (now only supports dirichlet).

### indexClusters.py ###

* Test to make sure that indexClusters works for n dimensions.
* Some clusters seem to get the same index even though they are not part of the same cluster. This has showed up on some plots via the coloring (taken from the indices in clusters) being the same. Might be a problem with how the coloring is chosen.

### Hoshen Kopelman ###

* If there is time, write the Hoshen Kopelman algorithm for indexing clusters.

### utils.py ###
