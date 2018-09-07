'''
Brief:               Plot illustrations of lattices

File name:           illustrations.py
Author:              Simon Rydell
Python Version:      3.7
'''

import helpers.illustrations as illu
from matplotlib import pyplot as plt
import random
import numpy as np

if __name__ == '__main__':
    x = 4
    y = 4
    pi = 3.1415
    form = "png"

    same_angle = pi/2
    tilt = list(np.linspace(0, pi/4, x))
    tilt_angles = [a - t for a, t in zip([same_angle]*len(tilt), tilt)]
    random_angles = [random.uniform(0, 2 * pi) for i in range(x * y)]
    random_tilt_angles = [a - t for a, t in zip(random_angles, tilt * y)]

    angles = [same_angle, tilt_angles, random_angles, random_tilt_angles]
    fignames = ["SameAngle", "PhaseShift", "RandomAngle", "RandomPhaseShift"]
    for angs, figname in zip(angles, fignames):
        plt.clf()
        illu.plot_lattice(x, y)

        illu.plot_spins(x, y, angs)

        # plt.show()

        ans = input(f"Save figure as: {figname}? ")

        if ans in 'yY':
            plt.savefig(f"./plots/{figname}.{form}",\
                        bbox_inches='tight', format=form, dpi=1200)
