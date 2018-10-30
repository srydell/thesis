'''
Brief:               Plot a large 2d cluster

File name:           large_cluster_illu.py
Author:              Simon Rydell
Python Version:      3.7
'''


import helpers.calc as calc
import helpers.constants as const
import helpers.illustrations as illu
import helpers.animate as ani
import matplotlib.pyplot as plt
import numpy as np
from helpers.arrow3d import Arrow3D

def drawer(x0, x1, axis, color, style, weight):
    # print(f"Call to drawer(x0={x0}, x1={x1}, color={color}, style={style}, weight={weight}")
    axis.plot(x0, x1, c=color, linestyle='-', linewidth=0.1 * weight)

if __name__ == '__main__':
    # Setup
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    system_size = 128
    dimension = 3

    # Camera angle
    theta = 60

    # Color of flux
    c = const.COLOR_MAP["red"]

    # Load graph
    sites = ani.process_graph_file(f"xy/largest_cluster/largest_cluster", dimension, drawer)

    pdf_frames = []
    current_frame = -1
    max_angle = 20
    for angle in np.arange(0, max_angle, 5):
        current_frame += 1

        print("\n=========================")
        print(f"On frame number: {current_frame}")

        # Remove surrounding border
        ax.axis('off')

        # Set viewing angle
        ax.view_init(angle, 0)

        # Plot all flux
        for site in sites:
            site.plot_arrows_to_neighbours(ax, system_size, color=c)

        # print("Assuming a test run. Quitting...")
        # quit()

        illu.save_figure(f"frames/xy/largest_cluster/{current_frame:03}")
        pdf_frames.append(f"plots/frames/xy/largest_cluster/{current_frame:03}.pdf")
        ax.cla()

    output_file = "plots/3dxy_largest_cluster"

    print(f"Merging the pdfs into {output_file}.pdf")
    ani.merge_pdfs(pdf_frames, output_file)

    print(f"Creating the gif {output_file}.gif")
    ani.pdf2gif(output_file, output_file)
