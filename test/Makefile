# -*- MakeFile -*-
#
# Written by Simon Rydell
# April 2018
#

.PHONY: all graphFunctions isingWorm plotGraph

all: graphFunctions isingWorm plotGraph

graphFunctions:
	@printf "\nTesting graphFunctions\n"
	@python -m unittest test_graphFunctions.py

isingWorm:
	@printf "\nTesting isingWorm\n"
	@python -m unittest test_isingWorm.py

plotGraph:
	@printf "\nTesting plotGraph\n"
	@python -m unittest test_plotGraph.py
