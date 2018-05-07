# Import files from relative path
import sys
sys.path.append("../")

import unittest
from graphs import *
from testGraph import *
 
class TestGraphs(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        """Runs at the start of the test and creates the graphs used for testing.
        NOTE: This assumes that the function buildGraph works.
        Also creates two graphs that were created by hand

        :cls: class
        """

        # Two quadratic graphs
        cls.graphDirichlet3x3 = buildGraph([3, 3], "dirichlet")
        cls.graphPeriodic3x3 = buildGraph([3, 3], "periodic")

        # Two nonquadratic graphs
        cls.graphDirichlet3x5 = buildGraph([3, 5], "dirichlet")
        cls.graphPeriodic3x5 = buildGraph([3, 5], "periodic")

        # Two quadratic graphs
        cls.graphNonRecDirichlet3x3 = buildNonRecursiveGraph(3, 3, "dirichlet")
        cls.graphNonRecPeriodic3x3 = buildNonRecursiveGraph(3, 3, "periodic")

        # Two nonquadratic graphs
        cls.graphNonRecDirichlet3x5 = buildNonRecursiveGraph(3, 5, "dirichlet")
        cls.graphNonRecPeriodic3x5 = buildNonRecursiveGraph(3, 5, "periodic")

        cls.rec = [cls.graphDirichlet3x3, cls.graphPeriodic3x3, cls.graphDirichlet3x5, cls.graphPeriodic3x5]
        cls.nonrec = [cls.graphNonRecDirichlet3x3, cls.graphNonRecPeriodic3x3, cls.graphNonRecDirichlet3x5, cls.graphNonRecPeriodic3x5]

    def test_buildGraph(self):
        for n, g in enumerate(self.nonrec):
            self.assertEqual(g, self.rec[n])

if __name__ == '__main__':
    unittest.main()
