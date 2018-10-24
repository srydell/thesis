'''
Brief:               Site class file

File name:           site.py
Author:              Simon Rydell
Python Version:      3.7
'''

class Site():
    """Site in a nD lattice"""

    def __init__(self, i, neighbours, weights, dimension=3, draw_function=None):
        """Create neighbours and links"""
        self.i = i
        self.neighbours = neighbours
        self.weights = weights
        self.dimension = dimension
        self.draw = draw_function

        # print(f"Created a Site:\n\t{i}: {neighbours}")

    def convert_to_xyz(self, index, length):
        """Convert index to [x, y, z] coordinates

        :index: Int
        :length: Int - system size
        :returns: 1xd array of ints
        """

        # print(f"Call to convert_to_xyz(index = {index}, length = {length})")

        xyz = []
        for i in range(self.dimension):
            xyz.append(index % length)
            index = index // length

        # print(f"Returning: {xyz}")

        return xyz

    def draw_tail_if_percolating(self, xyz, nxyz, weight, axis, length, color):
        """If percolating, draw tail and return new xyz, otherwise return untouched xyz

        :xyz: 1x3 array of ints
        :nxyz: 1x3 array of ints
        :axis: Plot Object
        :length: Int - system size
        :returns: 1x3 array of ints
        """

        # print(f"Call to draw_tail_if_percolating with:\n\t xyz = {xyz}, nxyz = {nxyz}")

        for i in range(len(xyz)):
            # If percolating from larger to smaller
            if xyz[i] - nxyz[i] == length - 1:
                xyz_old = xyz.copy()
                # Draw out of the graph
                xyz[i] += 1/2

                # print(f"Percolating! Draw tail with weight {weight}:\n\t{xyz_old} -> {xyz}")

                self.draw(xyz_old, xyz, axis, color, '-', weight)

                # Draw into the graph
                xyz[i] = nxyz[i] - 1/2
                return xyz

            # If percolating from smaller to larger
            if xyz[i] - nxyz[i] == -1 * length + 1:
                xyz_old = xyz.copy()
                # Draw into the graph
                xyz[i] -= 1/2

                # print(f"Percolating! Draw tail with weight {weight}:\n\t{xyz_old} -> {xyz}")

                self.draw(xyz_old, xyz, axis, color, '-', weight)

                # Draw out of the graph
                xyz[i] = nxyz[i] + 1/2
                return xyz
        return xyz

    def plot_arrows_to_neighbours(self, axis, length, color, style='-|>'):
        """Plot arrows from self.i to all neighbours

        :axis: Plot Object
        :length: Int - system size
        :returns: None
        """
        for n, w in zip(self.neighbours, self.weights):
            root_xyz = self.convert_to_xyz(self.i, length)
            xyz = root_xyz
            nxyz = self.convert_to_xyz(n, length)

            if w < 0:
                xyz, nxyz = nxyz, xyz
                w = -1 * w

            # print(f"Check if percolating:\n\t{self.i}         -> {n}\n\t{xyz} -> {nxyz}")

            # Create new xyz and draw the tail of the arrow if the step is percolating
            xyz = self.draw_tail_if_percolating(xyz, nxyz, w, axis, length, color)

            # print(f"Draw with weight {w}:\n\t{self.i}         -> {n}\n\t{xyz} -> {nxyz}")

            # self.draw(self, x0, x1, axis, color, style, weight):
            self.draw(xyz, nxyz, axis, color, style, w)

            # print()

    def __repr__(self):
        """Representation of a Site object

        :returns: String
        """
        out = f"({self.i}: ["
        for n, w in zip(self.neighbours, self.weights):
            out += f"{n},{w} "
        return f"{out[:-1]}])"
