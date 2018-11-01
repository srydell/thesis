'''
Brief:               Drawing of a koch curve mockup

File name:           koch_curve_mockup.py
Author:              Simon Rydell
Python Version:      3.7
'''

import matplotlib.pyplot as plt

# plt.plot([0, 1], [0, 0], c='k')
plt.plot([0, 1/3, 1/2, 2/3, 1], [0, 0, 1/3, 0, 0], c='k')
# plt.xlabel([], [])
# plt.ylabel([], [])
plt.show()
