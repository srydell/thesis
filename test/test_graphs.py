# Import files from relative path
import sys
sys.path.append("../")

import unittest
from graphs import *
 
class TestGraphFunctions(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        """Runs at the start of the test and creates the graphs used for testing.
        NOTE: This assumes that the function buildGraph works.
        Also creates two graphs that were created by hand

        :cls: class
        """

        # Two quadratic graphs
        cls.graphDirichlet3x3 = buildGraph(3, 3, "dirichlet")
        cls.graphPeriodic3x3 = buildGraph(3, 3, "periodic")

        # Two nonquadratic graphs
        cls.graphDirichlet3x5 = buildGraph(3, 5, "dirichlet")
        cls.graphPeriodic3x5 = buildGraph(3, 5, "periodic")

        # Two 2x2 edge case graphs created by hand
        cls.edgeDirichlet2x2 = {(0, 0): {(0, 1): 0, (1, 0): 0}, (0, 1): {(0, 0): 0, (1, 1): 0}, (1, 0): {(1, 1): 0, (0, 0): 0}, (1, 1): {(0, 1): 0, (1, 0): 0}}
        cls.edgePeriodic2x2 = {(0, 0): {(0, 1): 0, (1, 0): 0, (0, 1): 0, (1, 0): 0}, (0, 1): {(0, 0): 0, (1, 1): 0, (0, 0): 0, (1, 1): 0}, (1, 0): {(1, 1): 0, (0, 0): 0, (1, 1): 0, (0, 0): 0}, (1, 1): {(1, 0): 0, (0, 1): 0, (1, 0): 0, (0, 1): 0}}

        # This way we can iterate over the graphs
        cls.graphs = [cls.graphDirichlet3x3, cls.graphPeriodic3x3,
                cls.graphDirichlet3x5, cls.graphPeriodic3x5, cls.edgeDirichlet2x2, cls.edgePeriodic2x2]


    def compareAllWeights(self, graph, weight):
        # Check that all sites have weight weight in graph
        for site in graph:
            for neighbour in graph[site]:
                self.assertEqual(graph[site][neighbour], weight)
        
    def test_buildGraph(self):

        # Check that we have 3*3=9 sites
        self.assertEqual(len(self.graphPeriodic3x3), 9)
        self.assertEqual(len(self.graphDirichlet3x3), 9)

        # Check that we have 3*5=15 sites
        self.assertEqual(len(self.graphPeriodic3x5), 15)
        self.assertEqual(len(self.graphDirichlet3x5), 15)

        # Check that all sites have weight 0 at the start in the graphs
        self.compareAllWeights(self.graphPeriodic3x3, 0)
        self.compareAllWeights(self.graphDirichlet3x3, 0)
        self.compareAllWeights(self.graphPeriodic3x5, 0)
        self.compareAllWeights(self.graphDirichlet3x5, 0)

        # Check that buildGraph throws exception for unsupported bc strings
        with self.assertRaises(unsupportedBoundaryCondition):
            buildGraph(3, 3, "some unsupported boundary condition")

    def test_colorLinkBetween(self):
        # Only calls flipSiteWeight twice
        pass

    def test_flipSiteWeight(self):
        # Test sites
        cornerSite = (0, 0)
        cornerNeighbour = (0, 1)
        middleSite = (1, 1)
        middleNeighbour = (1, 0)

        # Go through all graph cases
        for graph in self.graphs:
            # Flip corner and assert weight is now 1
            flipSiteWeight(cornerSite, cornerNeighbour, graph)
            self.assertEqual(graph[cornerSite][cornerNeighbour], 1)
            # Flip corner and assert weight is now 0
            flipSiteWeight(cornerSite, cornerNeighbour, graph)
            self.assertEqual(graph[cornerSite][cornerNeighbour], 0)

            # Flip middle and assert weight is now 1
            flipSiteWeight(middleSite, middleNeighbour, graph)
            self.assertEqual(graph[middleSite][middleNeighbour], 1)
            # Flip middle and assert weight is now 0
            flipSiteWeight(middleSite, middleNeighbour, graph)
            self.assertEqual(graph[middleSite][middleNeighbour], 0)

    def test_getWeight(self):
        pass

    def test_getLinkedNeighbours(self):
        pass

    def test_getRandomNeighbour(self):
        pass

    def test_isOnBorder(self):
        pass

    def test_removeOnBorder(self):
        pass

if __name__ == '__main__':
    unittest.main()
