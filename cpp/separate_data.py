'''
Brief:               Get data from data(4) data(4) ... -> data(4) data(8) ...

File name:           separate_data.py
Author:              Simon Rydell
Python Version:      3.7
'''

data = []
filename = "./susceptibility27515"
with open(filename, 'r') as data_file:
    # Create the data boxes
    current_length = 4
    # TODO: Change this to the correct size (128)
    max_length = 8
    while current_length <= max_length:
        data.append([])
        current_length *= 2

    for n, line in enumerate(data_file.readlines()):
        # Assume line is 'd0 d1 d2 ...'
        current_data = [float(d) for d in line.split()]
        for d in current_data:
            data[n % len(data)].append(d)

outname = "susceptibility"
with open(outname, 'w') as out_file:
    # Assume all data boxes have the same length
    for i in range(len(data[0])):
        out_string = ''
        for d in data:
            out_string += f"{d[i]} "
        out_file.write(f"{out_string}\n")
