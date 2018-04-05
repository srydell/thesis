# Import files from relative path
import sys
sys.path.append("../")

import unittest
import isingWorm

class TestIsingWorm(unittest.TestCase):

    def test_isAccepted(self):
        """Check if the link is accepted

        :K: J/T
        :site0: 1x2 matrix [i, j]
        :site1: 1x2 matrix [i, j]
        :gitter: dictionary
        :returns: True if accepted and False otherwise
        """
        pass

    def test_main(self):
        """Main simulation function

        :returns: gitter
        """
        pass

if __name__ == '__main__':
    unittest.main()
