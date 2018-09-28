'''
Brief:               Plot illustration of acceptance ratio

File name:           plot_acceptance_ratio.py
Author:              Simon Rydell
Python Version:      3.7
'''

import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rc

rc('font', **{'family': 'serif', 'serif': ['DejaVu Sans']})
rc('text', usetex=True)

def plot_dist(J, dJ, T_range, label=""):
    """Plot the distribution p in e^{-1/T(+-2J + 1}

    :J: Int - The current before update
    :dJ: Int - The update in J
    :T_range: 1x2 array - Temperature range to plot
    :label: String - label of plot
    :returns: None
    """
    T = np.linspace(T_range[1], T_range[0])
    dist = lambda x, y, dy: np.exp(-1 / x * ( dy * 2 * y + 1))
    p = dist(T, J, dJ)
    plt.plot(T, p, label=label)

if __name__ == '__main__':
    T = [1.8, 2.6]
    Tc = 2.2
    for J in range(0, 3):
        for dJ in [-1, 1]:
            plot_dist(J, dJ, T, f"J = {J}, dJ = {dJ}")
    plt.title(r"$p = e^{-\frac{1}{T} ( dJ \cdot J + 1)}$")
    plt.xlabel("Temperature")
    plt.ylabel("Acceptance probability")
    plt.legend(loc=1)
    plt.show()
