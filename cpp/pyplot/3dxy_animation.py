'''
Brief:               Write frames of an animation to pyplot/plots/frames

File name:           plot_winding.py
Author:              Simon Rydell
Python Version:      3.7
'''

import helpers.calc as calc
import helpers.constants as const
import helpers.illustrations as illu
import helpers.animate as ani
import matplotlib.pyplot as plt
import numpy as np

if __name__ == '__main__':
    # Setup
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    system_size = 4

    # Camera angle
    theta = 30

    # Color of flux
    c = const.COLOR_MAP["red"]
    pdf_frames = []
    # TODO: Change here for making moving graph
    # current_frame = -1
    # max_angle = 20
    max_angle = 360
    # TODO: Change here for making moving graph
    # for angle in np.arange(0, max_angle, 0.5):
    angle = 30
    for frame in range(max_angle * 2):
    # for frame in range(1):

        # TODO: Change here for making moving graph
        # current_frame += 1
        current_frame = frame

        print("\n=========================")
        print(f"On frame number: {current_frame}")

        # Remove surrounding border
        ax.axis('off')

        # Redraw lattice
        ani.plot_lattice(system_size, const.COLOR_MAP["black"], ax, size=5)
        ax.view_init(theta, angle)

        # Load graph
        sites = ani.process_graph_file(f"xy/{current_frame}")

        # Plot all flux
        for site in sites:
            site.plot_arrows_to_neighbours(ax, system_size, color=c)

        print("Assuming a test run. Quitting...")
        quit()

        illu.save_figure(f"frames/xy/{current_frame:03}")
        pdf_frames.append(f"plots/frames/xy/{current_frame:03}.pdf")
        ax.cla()

    # TODO: Change here for making moving graph
    # output_file = "plots/3dxy_animation"
    output_file = "plots/3dxy_animation_still"

    print(f"Merging the pdfs into {output_file}.pdf")
    ani.merge_pdfs(pdf_frames, output_file)

    print(f"Creating the gif {output_file}.gif")
    ani.pdf2gif(output_file, output_file)
