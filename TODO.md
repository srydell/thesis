# Master thesis of Simon Rydell

## Todo list for exwork ##

### Questions for Mats ###

* What is the initial value of the correlation function G(i)?
* Have I interpreted bij the correct way? (As the weight of the link between sites i and j)
* Is there something special about G(0)?
* <L> in the pdf should be interpreted as the total length of the finished loop or the average length of all loops so far?

### isingWorm ###

* Write test code for isingWorm
* Let main() take arguments instead of setting them statically. Easier to test maybe. This would mean that in the C++ code, the main function would have to parse args from the command line, maybe using a bash script to store configs.

### graphFunctions ###

* Write test code for graphFunctions
* Add functionality for boundary conditions in (buildGraph, ) (Maybe done, needs more testing)

### plotGraph ###

* Write test code for plotGraph
* Rewrite save graphs to saving SQL table. (Call each table NxMboundaryCondition and add a row for each data point)
* Rewrite load graphs to load from SQL table
* plot graph (option for file or live)
* Put plotting configs into its own function and let it take graphSize as arg to set axis
