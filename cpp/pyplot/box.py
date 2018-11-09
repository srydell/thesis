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

# # Large box
# ax.add_patch(patches.Rectangle(
#               (0, 0),
#               1, 1,
#               facecolor="#202020",
#               fill=True))

# 4 small boxes
for xy in [(0, 0), (1.15, 0), (0, 1.15), (1.15, 1.15)]:
    ax.add_patch(patches.Rectangle(
                 xy,
                 1, 1,
                 facecolor="#202020",
                 fill=True))

ax.set_xlim([0, 2.15])
ax.set_ylim([0, 2.15])
ax.axis('off')
# illu.save_figure("square_divided")
plt.show()
