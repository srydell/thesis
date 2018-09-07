'''
Brief:               Helper functions for drawing nice illustration figures

File name:           illustrations.py
Author:              Simon Rydell
Python Version:      3.7
'''

from matplotlib import pyplot as plt
import random
import numpy as np

def plot_lattice(x, y, size=20):
    """Plot a square lattice from the points x, y

    :x: Int
    :y: Int
    :size: Int - Size of each lattice point
    :returns: None
    """
    # Remove surrounding border
    plt.axis('off')
    plt.xlim([-.5, x - .5])
    plt.ylim([-.5, y - .5])
    for i in range(x):
        for j in range(y):
            plt.scatter(i, j, color='#181414', s=size)

def _plot_spin(x, y, angle):
    """Plot arrow centering around x, y in the direction of angle

    :x: Int
    :y: Int
    :returns: None
    """
    cosa = np.cos(angle)
    sina = np.sin(angle)
    # To compensate for head length
    hl = .1
    plt.arrow(x - cosa/4, y - sina/4,\
            cosa/2 - hl * cosa, sina/2 - hl * sina,\
            head_width=0.05, head_length=hl, fc='k', ec='k')

def plot_spins(x, y, angles):
    """Plot spins as arrows
       NOTE: If there aren't enough angles,
             it will still plot the whole system,
             but repeating the angles

    :x: Int
    :y: Int
    :angles: 1xn array of Floats
    :returns: None
    """
    if type(angles) in [float, int]:
        angles = [angles]
    counter = 0
    for j in range(y):
        for i in range(x):
            _plot_spin(i, j, angles[counter % len(angles)])
            counter += 1

def plot_arrow(x, y, dx, dy, size=1, color="#181414"):
    """Plot arrow adjusted for headsize

    :x: Int
    :y: Int
    :dx: Float
    :dy: Float
    :size: Int - Size of the arrow
    :color: String - Hex color of the arrow
    :returns: None
    """
    hl = 0.1 + (.05 * size)
    hw = hl / 2
    w = .010 * size
    c = np.sqrt(np.power(dx, 2) + np.power(dy, 2))
    cosa = dx / c
    sina = dy / c

    plt.arrow(x, y,\
            dx - hl * cosa, dy - hl * sina,\
            width=w, head_width=hw, head_length=hl, fc=color, ec=color)

def fill_with_arrows(p0, p1, size, color):
    """Plot arrows with length 1 between p0 = [x0, y0] and p1 = [x1, y1]
       assuming that either x0 == x1 or y0 == y1

    :p0: 1x2 array of int - Starting point
    :p1: 1x2 array of int - End point
    :size: Int
    :color: String - Hex color for each arrow
    :returns: None
    """
    x0, y0 = p0
    x1, y1 = p1
    if x0 == x1:
        x = x0
        sign = 1 if y0 - y1 < 0 else -1
        for j in range(0, abs(y0 - y1)):
            # print(f"Plotting between [{x}, {y0 + sign * j}] and [{x}, {y0 + sign * j + sign}]")
            plot_arrow(x, y0 + sign * j, 0, sign,\
                    size=size, color=color)
    elif y0 == y1:
        y = y0
        sign = 1 if x0 - x1 < 0 else -1
        for i in range(0, abs(x0 - x1)):
            # print(f"Plotting between [{x0 + sign * i}, {y}] and [{x0 + sign * i + sign}, {y}]")
            plot_arrow(x0 + sign * i, y, sign, 0,\
                    size=size, color=color)
    else:
        print("Error! Function 'fill_with_arrows' requires the points to be parallell on either x-axis or y-axis")
        print(f"       Provided points were: {p0}, {p1}")
