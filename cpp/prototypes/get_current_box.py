'''
Brief:               Prototype for adding all sites in a box given dimension, start, sidelength

File name:           proto.py
Author:              Simon Rydell
Date created:        Jul 27, 2018
Date last modified:  Jul 27, 2018
Python Version:      3.7
'''

def get_box(start, side_length, system_size, dimension):
    """Calculate all the sites in the box corresponding to the inputs

    :start: Unsigned - The site where the box starts
    :side_length: Unsigned - Side length of the box being examined
    :system_size: Unsigned - Size of the total system (all boxes)
    :dimension: Unsigned
    :returns: 1xn array of unsigned - All sites in the box
    """
    # site_in_box = start + to_add .* [1, system_size, system_size^2, ..., system_size^(dimension-1)]
    # Each element of to_add goes from 0 to side_length-1
    # When to_add[0] gets to side_length-1, to_add[1] goes from 0 to 1,
    # when to_add[1] gets to side_length-1, to_add[2] goes from 0 to 1 and so on.
    to_add = [0]*dimension

    base_directions = []
    for exponent in range(dimension):
        base_directions.append(pow(system_size, exponent))

    all_sites_in_box = []
    # The start is always part of the current box
    all_sites_in_box.append(start)

    while not all([adders == (side_length-1) for adders in to_add]):
        # Get the next values to add
        to_add = next_to_add(to_add, side_length)

        # Calculate the next site that is within the box
        # site_in_box = start + i * system_size + j * system_size^2 + ... + w * system_size^(dimension-1)
        # where [i, j, ..., w] all are integers in the range 0 -> (side_length-1)
        site_in_box = start
        for i in range(dimension):
            site_in_box += to_add[i] * base_directions[i]

        # Add the calculated new site into all sites
        all_sites_in_box.append(site_in_box)

        # print(site_in_box)

    return all_sites_in_box

def next_to_add(to_add, side_length):
    """Create the next to_add

    :to_add: 1xdimension array of unsigneds
    :side_length: Unsigned
    :returns: 1xdimension array of unsigneds
    """
    for i, adder in enumerate(to_add):
        # Add one to first value if that value is less than side_length-1,
        # else set that value to zero until a value is encountered that is less than side_length-1 and add 1 to that
        if adder == (side_length-1):
            to_add[i] = 0
        else:
            to_add[i] += 1
            break
    return to_add

if __name__ == '__main__':
    start = 0
    side_length = 2
    system_size = 4
    dimension = 3
    print(get_box(start, side_length, system_size, dimension))

    start = 3
    side_length = 2
    system_size = 4
    dimension = 3
    print(get_box(start, side_length, system_size, dimension))

    start = 0
    side_length = 2
    system_size = 4
    dimension = 2
    print(get_box(start, side_length, system_size, dimension))
