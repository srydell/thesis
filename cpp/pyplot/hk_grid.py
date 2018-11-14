'''
Brief:               Creating a grid of black and white squares

File name:           hk_grid.py
Author:              Simon Rydell
Python Version:      3.7
'''

import matplotlib.pyplot as plt
import helpers.illustrations as illu
import matplotlib.patches as patches
import random

fig = plt.figure()
ax = fig.add_subplot(111, aspect='equal')

ax.set_xticklabels([])
ax.set_yticklabels([])

# 4 small boxes
l = 30
for x in range(l):
    for y in range(l):
        xy = (x, y)
        c = random.choice(["#202020", "#202020", "#202020", "#FFFFFF", "#FFFFFF", "#FFFFFF", "#FFFFFF"])
        ax.add_patch(patches.Rectangle(
                     xy,
                     1, 1,
                     facecolor=c,
                     fill=True))

ax.set_xlim([-1, l + 1])
ax.set_ylim([-1, l + 1])
ax.axis('off')

# plt.grid(color='k', linestyle='-', linewidth=1)

illu.save_figure("hk_grid")
# plt.show()
