# Import files from relative path
import sys
sys.path.append("../")

import unittest
import graphFunctions
 
class TestGraphFunctions(unittest.TestCase):

    def test_buildGraph(self):
        """Creates a graph in the form of a dictionary. Assumes periodic boundary conditions.

        :N: Number of rows
        :M: Number of columns
        :bc: Boundary condition - String in ["periodic", "dirichlet"]
        :returns: graph dictionary in the form graph[site] = [[siteNeighbour0, weight0], [siteNeighbour1, weight1], ...] and graph size
        """
        pass

    def test_getLinkWeight(self):
        """
        :site0: 1x2 matrix [i, j]
        :site1: 1x2 matrix [i, j]
        :graph: dictionary
        :returns: link weight between site0 and site1
        """
        pass

    def test_flipSiteWeight(self):
        """Flips the site weight between 0 and 1

        :site0: 1x2 matrix [i, j]
        :site1: 1x2 matrix [i, j]
        :graph: dictionary
        :returns: None
        """
        pass

    def test_colorLinkBetween(self):
        """Changes the link weight between site0 and site1

        :site0: 1x2 matrix
        :site1: 1x2 matrix
        :graph: dictionary
        :returns: None
        """
        pass
        
    def test_getRandomNeighbour(self):
        """ Get random neighbour to site that is not exceptSite

        :site: 1x2 matrix [i, j]
        :exceptSite: 1x2 matrix [i, j], is None for firstSite
        :graph: dictionary
        :returns: 1x2 matrix, neighbour from site in graph [i, j]
        """
        pass

    def test_getLinkedNeighbours(self):
        """
        :site: 1x2 matrix
        :graph: dictionary
        :returns: Array of arrays - All neighbours to site in graph with weight 1
        """
        pass
