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
import matplotlib.patches as patches

def plot_setup(size, pltlattice=True):
    """Setup 2d ising plot

    :size: Int - Size of graph
    :returns: None
    """
    # Remove surrounding border
    ax.axis('off')
    ax.set_xlim([-1, size])
    ax.set_ylim([-1, size])

    if pltlattice:
        # Redraw lattice
        ani.plot_lattice(size, const.COLOR_MAP["black"], ax, size=20, d=2)
        
def freeze_frames(num_frames, current_frame):
    """Freeze frames for num_frames

    :num_frames: Int - Number of frames frozen
    :current_frame: Int
    :returns: Int - current_frame
    """
    for _ in range(1, num_frames + 1):
        current_frame += 1
        print("\n====================================")
        print(f"On freeze frame: {current_frame}")

        illu.save_figure(f"frames/ising/{current_frame:03}")
        pdf_frames.append(f"plots/frames/ising/{current_frame:03}.pdf")

    return current_frame

def plot_faded_blocks(current_blocks, faded, axis):
    """TODO: Docstring for fade_box.

    :current_blocks: 1xn array of arrays
    :faded: 1xn array of bools
    :axis: Plot Object
    :returns: TODO
    """
    length = 8
    dimension = 2
    for cb, is_faded in zip(current_blocks, faded):
        for site in cb:

            xy = []
            for i in range(dimension):
                xy.append(site % length)
                site = site // length

            current_color = 'lightgrey' if is_faded else const.COLOR_MAP["black"]
            axis.scatter(xy[0], xy[1], c=current_color, s=20)

def get_blocks(list_of_blocks, current_block_size, system_size):
    """Return list of lists of blocks

    :list_of_blocks: 1xn array of ints
    :current_block_size: float
    :returns: 1xn array of arrays of ints - list of all blocks corresponding to this size
    """
    dimension = 2
    num_sites_per_block = int(current_block_size) ** dimension
    num_blocks = system_size ** dimension / num_sites_per_block

    num_sites_per_block = int(num_sites_per_block)
    num_blocks = int(num_blocks)
    output = [[] for i in range(num_blocks)]

    index = 0
    for i in range(num_blocks):
        for j in range(num_sites_per_block):
            output[i].append(list_of_blocks[j + index])
        index += num_sites_per_block

    return output

def plot_largest_worm(worm, sites, system_size, axis):
    """Plot worm

    :worm: TODO
    :returns: TODO
    """
    for site in [s for s in sites if s.i in largest_worm[0][0]]:
        c = const.COLOR_MAP["green"]
        site.plot_arrows_to_neighbours(axis, system_size, color=c, style='-')

    for site in [s for s in sites if s.i not in largest_worm[0][0]]:
        c = const.GRADIENT["black"][1]
        site.plot_arrows_to_neighbours(axis, system_size, color=c, style='--')

if __name__ == '__main__':
    # Setup
    fig, ax = plt.subplots()
    system_size = 8
    dimension = 2

    # Color of flux
    c = const.COLOR_MAP["red"]
    pdf_frames = []
    # For some reason the ising files start at 1
    last_simulation_frame = 349
    stop_frames = 10
    for frame in range(227, last_simulation_frame + 1):
    # for frame in range(348, last_simulation_frame + 1):

        print("\n====================================")
        print(f"On frame: {frame}")

        plot_setup(system_size)

        # Load graph
        sites = ani.process_graph_file(f"ising/{frame}", dimension)

        # Plot all flux
        for site in sites:
            site.plot_arrows_to_neighbours(ax, system_size, color=c, style='-')

        illu.save_figure(f"frames/ising/{frame:03}")
        pdf_frames.append(f"plots/frames/ising/{frame:03}.pdf")
        if frame == last_simulation_frame:
            frame = freeze_frames(stop_frames, frame)

        # Clear axis for next plot
        ax.cla()

    largest_worm = calc.process_file("data/animation/ising/largest_worm", "xxxx", r"(\d*)")

    plot_setup(system_size)

    plot_largest_worm(largest_worm, sites, system_size, ax)

    print("Freeze frames for largest_worm")
    frame = freeze_frames(stop_frames, frame)

    # Draw separating lines
    lines = [
                [
                    [[system_size / 2 - 0.5, system_size / 2 - 0.5], [-0.5, system_size - 0.5]],
                    [[- 0.5, system_size - 0.5], [system_size / 2 - 0.5, system_size / 2 - 0.5]],
                ], [
                    [[system_size / 2 - 0.5, system_size / 2 - 0.5], [-0.5, system_size - 0.5]],
                    [[system_size / 4 - 0.5, system_size / 4 - 0.5], [-0.5, system_size - 0.5]],
                    [[3 * system_size / 4 - 0.5, 3 * system_size / 4 - 0.5], [-0.5, system_size - 0.5]],
                    [[-0.5, system_size - 0.5], [system_size / 4 - 0.5, system_size / 4 - 0.5]],
                    [[-0.5, system_size - 0.5], [3 * system_size / 4 - 0.5, 3 * system_size / 4 - 0.5]],
                ]
            ]

    # for l in lines[0]:
    #     ax.plot(*l, c='darkgrey', linewidth=1)

    # print(f"Freeze frames for line:")
    # frame = freeze_frames(stop_frames, frame)

    blocks = calc.process_file("data/animation/ising/blocks", r"block_number=(\d*)", r"(\d*)")
    # Largest blocks first
    block_sizes = list(reversed(list(blocks.keys())))
    faded_or_not = [[1, 0, 0, 0], [1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0]]
    for bs, faded, separations in zip(block_sizes, faded_or_not, lines):

        for l in separations:
            ax.plot(*l, c='darkgrey', linewidth=1)

        print(f"Freeze frames for blocks")
        frame = freeze_frames(stop_frames, frame)

        # Get the block and plot it
        current_blocks = get_blocks(blocks[bs][0], bs, system_size)
        plot_faded_blocks(current_blocks, faded, ax)

        frame = freeze_frames(stop_frames, frame)

    # Freeze one extra time on the last frame
    frame = freeze_frames(stop_frames, frame)

    # TODO: Change this after testing
    # quit()

    # output_file = "plots/2dising_animation"
    output_file = "plots/2dising_animation"

    print(f"Merging the pdfs into {output_file}.pdf")
    ani.merge_pdfs(pdf_frames, output_file)

    print(f"Creating the gif {output_file}.gif")
    ani.pdf2gif(output_file, output_file)
