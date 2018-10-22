import os, sys
current_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.dirname(current_dir)
sys.path.insert(0, parent_dir)

import unittest
import calc

class TestGraphs(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        """Runs at the start of the test

        :cls: class
        """

    def test_get_intersection(self):
        # x0, x1, y0, y1
        # y = 2x
        l0 = [1, 2, 2, 4]

        # y = -x + 6
        # x1, x2, y1, y2
        l1 = [1, 2, 5, 4]
        intersection = [2, 4]

        self.assertEqual(calc.get_intersection(l0, l1), intersection)


if __name__ == '__main__':
    unittest.main()
