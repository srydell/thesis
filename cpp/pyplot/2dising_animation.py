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
    fig, ax = plt.subplots()
    system_size = 8
    dimension = 2

    # Color of flux
    c = const.COLOR_MAP["red"]
    pdf_frames = []
    # For some reason the ising files start at 1
    for frame in range(1, 700):
    # for frame in range(220, 222):

        print("\n================")
        print(f"On frame: {frame}")

        # Remove surrounding border
        ax.axis('off')
        ax.set_xlim([-1, system_size])
        ax.set_ylim([-1, system_size])

        # Redraw lattice
        ani.plot_lattice(system_size, const.COLOR_MAP["black"], ax, size=20, d=2)

        # Load graph
        sites = ani.process_graph_file(f"ising/{frame}", dimension)

        # Plot all flux
        for site in sites:
            site.plot_arrows_to_neighbours(ax, system_size, color=c, style='-')

        illu.save_figure(f"frames/ising/{frame:03}")
        pdf_frames.append(f"plots/frames/ising/{frame:03}.pdf")
        ax.cla()

    # # TODO: Change this after testing
    # quit()

    # output_file = "plots/2dising_animation"
    output_file = "plots/2dising_animation"

    print(f"Merging the pdfs into {output_file}.pdf")
    ani.merge_pdfs(pdf_frames, output_file)

    print(f"Creating the gif {output_file}.gif")
    ani.pdf2gif(output_file, output_file)
