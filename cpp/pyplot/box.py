'''
Brief:               Draw a box for presentation

File name:           box.py
Author:              Simon Rydell
Python Version:      3.7
'''

import matplotlib.pyplot as plt
import matplotlib.patches as patches
import helpers.illustrations as illu

fig = plt.figure()
ax = fig.add_subplot(111, aspect='equal')

ax.add_patch(patches.Rectangle(
              (0, 0),
              1, 1,
              facecolor="#202020",
              fill=True))

ax.axis('off')
illu.save_figure("square")
# plt.show()
