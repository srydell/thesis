'''
Brief:               Draw a grid of boxes with a disk inside for presentation

File name:           box.py
Author:              Simon Rydell
Python Version:      3.7
'''

import matplotlib.pyplot as plt
import helpers.illustrations as illu

fig = plt.figure()
ax = fig.add_subplot(111, aspect='equal')

circle = plt.Circle((0.5, 0.5), 0.3, color="#202020")

ax.add_artist(circle)
plt.grid(color='k', linestyle='-', linewidth=1)

ax.set_xticklabels([])
ax.set_yticklabels([])

plt.annotate(r"$\}$", fontsize=60, rotation=-90, xy=(0.2154, 0.00), xycoords='figure fraction')
plt.annotate(r"$\epsilon$", fontsize=20, xy=(0.265, 0.), xycoords='figure fraction')

illu.save_figure("circle_within_boxes")
# plt.show()
