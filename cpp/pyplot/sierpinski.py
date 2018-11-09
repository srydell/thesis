'''
Brief:                Draw the sierpinski triangle

File name:           sierpinski.py
Author:              Simon Rydell
Python Version:      3.7
'''

import random
import matplotlib.pyplot as plt
import helpers.illustrations as illu

def plot(points):
    """
    Plots the points using matplotlib.
    Points is a list of (x, y) pairs.
    """

    xx = [x for (x, y) in points]
    yy = [y for (x, y) in points]

    plt.plot(xx, yy, 'k,')

def sierpinski(n, vertices):
    """
    Generates positions for the Chaos Game Sierpinski
    triangle with 'n iterations in a square of size 1x1.
    """

    points = []

    # initial vertex
    x, y = random.choice(vertices)

    for i in range(n):
        # select new vertex
        vx, vy = random.choice(vertices)

        # get middle point
        x = (vx + x) / 2.0
        y = (vy + y) / 2.0

        points.append((x, y))

    plot(points)

plt.axis('off')
n = 50000
vertices = [(0.0, 0.0), (0.5, 1.0), (1.0, 0.0)]
sierpinski(n, vertices)
vertices = [(1.1, 0.0), (1.6, 1.0), (2.1, 0.0)]
sierpinski(n, vertices)
vertices = [(0.55, 1.1), (1.05, 2.1), (1.55, 1.1)]
sierpinski(n, vertices)
plt.show()
# illu.save_figure("sierpinski_divided")
