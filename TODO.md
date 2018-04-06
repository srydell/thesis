# Todo list for exwork #

## isingWorm ##

* Write test code for isingWorm
* Maybe let main() take arguments instead of setting them statically. Easier to test maybe.
* Ask Mats:
	* What is the initial value of the correlation function G(i)?
	* Have I interpreted bij the correct way? (As the weight of the link between sites i and j)
	* Is there something special about G(0)?
	* <L> in the pdf should be interpreted as the total length of the finished loop or the average length of all loops so far?

## graphFunctions ##

* Write test code for graphFunctions
* Add functionality for boundary conditions in [buildGraph, ]

## plotGraph ##

* Write test code for plotGraph
* Save graphs (pickle, SQL, ...)
* Load graphs (from file)
* plot graph (option from file or live)
* Put plotting configs into its own function and let it take graphSize as arg to set axis
