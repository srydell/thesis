'''
Brief:               Animations helper library

File name:           animate.py
Author:              Simon Rydell
Python Version:      3.7
'''

import re
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rc
from PyPDF2 import PdfFileMerger
from subprocess import call
from helpers.site import Site
from helpers.arrow3d import Arrow3D

rc('font', **{'family': 'serif', 'serif': ['DejaVu Sans']})
rc('text', usetex=True)

def draw_arrow(x0, x1, axis, color, style, weight):
    """Draw arrow from x0 to x1

    :x0: 1x3 array of ints
    :x1: 1x3 array of ints
    :axis: Plot Object
    :color: String - Valid hex color
    :style: String - Arrow style ["-", "-|>", ...]
    :weight: Int - Size of arrow
    :returns: None
    """

    if len(x0) == 3:
        arrow = Arrow3D([x0[0], x1[0]],
                        [x0[1], x1[1]],
                        [x0[2], x1[2]],
                        mutation_scale=10 * weight,
                        lw=weight, arrowstyle=style, color=color)
        axis.add_artist(arrow)
    elif len(x0) == 2:
        axis.plot([x0[0], x1[0]],
                  [x0[1], x1[1]],
                  c=color, linewidth=weight * 4)

def plot_lattice(limit, color, axis=None, size=20, d=3):
    """Plot a square lattice from the points x, y

    :limit: Int
    :color: String - Valid hex color
    :axis: Plot Object
    :size: Int - Size of each lattice point
    :returns: None
    """

    if axis is None:
        axis = plt

    if d == 3:
        for i in range(limit):
            for j in range(limit):
                for k in range(limit):
                    axis.scatter(i, j, k,
                                 color=color,
                                 s=size)
    elif d == 2:
        for i in range(limit):
            for j in range(limit):
                axis.scatter(i, j,
                             color=color,
                             s=size)

def process_graph_file(frame, dimension=3, draw_function=draw_arrow):
    """Read the graph specific file format

    :dimension: Int - dimension of sample
    :frame: String - Valid frame in ./animation, [1, 2, 3, ...]
    :returns: 1xn array of Sites
    """
    sites = []
    with open(f"./data/animation/{frame}", 'r') as _frame:
        for line in _frame:
            # Skip the first line containing the energy
            if line[0] != 'E':
                data = line.split()

                # print(f"Current line:\n{data}")

                index = int(data[0][:-1])
                data.remove(data[0])
                neighbours = []
                weights = []
                for d in data:
                    ds = d.split(',')

                    neighbours.append(int(ds[0]))
                    weights.append(int(ds[1]))

                sites.append(Site(index, neighbours, weights, dimension, draw_function=draw_function))

                # print(f"index:\n\t{index}")
                # print(f"neighbours:\n\t{neighbours}")
                # print(f"weights:\n\t{weights}")
                # print()

    return sites

def merge_pdfs(pdfs, output='result'):
    """Merge a list of pdfs

    :pdfs: 1xn array of strings - Valid paths to pdf files
    :output: String - Filename of merged pdf without .pdf extension
    :returns: None
    """
    merger = PdfFileMerger()

    for pdf in pdfs:
        merger.append(open(pdf, 'rb'))

    with open(f"{output}.pdf", 'wb') as fout:
        merger.write(fout)

def pdf2gif(pdf, output='result'):
    """Use ImageMagick command line utility to convert pdf input to a gif

    :pdf: String - Filename of pdf without the extension
    :output: String - Filename of gif output without the extension
    :returns: None
    """
    command = f"convert {pdf}.pdf {output}.gif"
    call(command.split())
