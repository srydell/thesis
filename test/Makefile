# -*- MakeFile -*-
#
# Written by Simon Rydell
# April 2018
#

.PHONY: all graphs isingWorm plotGraph

all: graphs isingWorm plotGraph

graphs:
	@printf "\nTesting graphs\n"
	@python -m unittest test_graphs.py

isingWorm:
	@printf "\nTesting isingWorm\n"
	@python -m unittest test_isingWorm.py

plotGraph:
	@printf "\nTesting plotGraph\n"
	@python -m unittest test_plotGraph.py
