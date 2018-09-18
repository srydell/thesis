# Master thesis of Simon Rydell

## Todo list for exwork ##

### Questions for Mats ###

* How good is the approximation of box dimension compared to the Hausdorff dimension with differing radii of balls?

### General ###

* Understand the Sudbo article

### Paper ###

* Add information about [Hausdorff dimension](https://en.wikipedia.org/wiki/Hausdorff_dimension)
* Add information about [Raster scan](https://en.wikipedia.org/wiki/Raster_scan)
* Add information about [worm algorithm](./literature/worm_algorithm_lecturenotes.pdf)
* Maybe add information about how the length of the loops are calculated, count each linked neighbour in a cluster and then divide by 2
* The periodic boundary conditions come from the fact that we are interested in a large system. This is a 'cheap' way of simulating this.
* Add a section describing Monte Carlo simulations chapter ~6
* Add a subsection describing detailed balance
* Write about choices made in this paper. 
* How the figure 8 can be interpreted as one or two loops. In this case I chose one. 

### Tips from Emil ###

* Append the state configuration to the data to avoid keeping two files. (std::copy)

### plotGraph.py ###

* Write test code for plotGraph
* Make it so that plotGraph does not reuse colors as much. Make a temporary array of colors to pop from and reset it every run.
* Add functionality to plot from file.
