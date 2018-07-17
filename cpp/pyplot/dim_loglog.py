'''
Brief:               Plot to see if the worms have expected scaling behaviour

File name:           plottest.py
Author:              Simon Rydell
Date created:        Jun 29, 2018
Date last modified:  Jun 29, 2018
Python Version:      3.6
'''

from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
import numpy as np

with open("./t2.txt") as f:
    num_occupied_boxes = []
    one_over_size = []
    for n, line in enumerate(f.readlines()):
        boxes_and_size = [float(i) for i in line.split(" ")]
        num_occupied_boxes.append(boxes_and_size[0])
        one_over_size.append(1 / boxes_and_size[1])
        print(f"{boxes_and_size[1]} : {1 / boxes_and_size[1]}")
        # one_over_size.append(1 / np.power(2, n))
    # one_over_size.reverse()

print(num_occupied_boxes)
print(one_over_size)

one_over_size = np.array(one_over_size)
num_occupied_boxes = np.array(num_occupied_boxes)

# The range on which to plot the fitted function
newX = np.logspace(-3, 0, base=10)

# Let's fit an exponential function.  
# This looks like a line on a lof-log plot.
def myExpFunc(x, a, b):
    return a * np.power(x, b)

popt, pcov = curve_fit(myExpFunc, one_over_size[0:-2], num_occupied_boxes[0:-2])
plt.plot(newX, myExpFunc(newX, *popt), 'r-', 
                 label="({0:.3f}*x**{1:.3f})".format(*popt))

plt.loglog(one_over_size, num_occupied_boxes, 'bo')
plt.grid(True, which="both", ls="-")
plt.legend()

plt.xlabel("1 / s")
plt.ylabel("n(s)")
plt.title("")

plt.show()
