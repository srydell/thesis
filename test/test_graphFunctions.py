# Import files from relative path
import sys
sys.path.append("../")

import unittest
import graphFunctions
 
class TestGraphFunctions(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        """Runs at the start of the test and creates the graphs used for testing.
        NOTE: This assumes that the function buildGraph works.

        :cls: class
        """

        cls.graphDirichlet = graphFunctions.buildGraph(3, 3, "dirichlet")
        cls.graphPeriodic = graphFunctions.buildGraph(3, 3, "periodic")

    def test_buildGraph(self):

        # Check that we have 3*3=9 sites
        self.assertEqual(len(self.graphPeriodic), 9)
        self.assertEqual(len(self.graphDirichlet), 9)

        # Check that all sites have weight 0 at the start in periodic graph
        for site in self.graphPeriodic:
            for neighbourData in self.graphPeriodic[site]:
                self.assertEqual(neighbourData[1], 0)

        # Check that all sites have weight 0 at the start in dirichlet graph
        for site in self.graphDirichlet:
            for neighbourData in self.graphDirichlet[site]:
                self.assertEqual(neighbourData[1], 0)

        # Check that buildGraph throws exception for unsupported bc strings
        with self.assertRaises(graphFunctions.unsupportedBoundaryCondition):
            graphFunctions.buildGraph(3, 3, "unsupported boundary condition")

    def test_colorLinkBetween(self):
        # Only calls flipSiteWeight
        pass
        
    def test_flipSiteWeight(self):
        pass

    def test_getLinkWeight(self):
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
