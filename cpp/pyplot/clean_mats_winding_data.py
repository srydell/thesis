'''
Brief:               Get Mats data on the same type as mine

File name:           plot_winding.py
Author:              Simon Rydell
Python Version:      3.7
'''

import re
import numpy as np
import matplotlib.pyplot as plt
import helpers.calc as calc
import helpers.constants as const
import helpers.illustrations as illu

def parse_data(filename, regex, size_regex):
    """Parse out temperature, winding_number and correct if there are exponents

    :filename: String - valid path
    :regex: Regex String -
                  First group temperature, second group winding_number, third group exponent
    :size_regex: Regex String - Parse out the size
    :returns: Float - winding number
    """
    winding_number = []
    temperature = []
    data_dict = {}
    with open(filename, 'r') as f:
        for line in f.readlines():
            match = re.match(regex, line)
            size_match = re.match(size_regex, line)
            if match:
                temp = float(match.group(1))
                temperature.append(temp)

                wind_unexp = float(match.group(2))
                exponent = float(match.group(3))
                winding_number.append(wind_unexp * (10 ** exponent))
                # print(f"Temperature is: {temp}")
                # print(f"Winding number is: {wind_unexp}")
                # print(f"Exp is: {exponent}")
                # print(f"Expanded: {wind_unexp * (10 ** exponent)}")
                # print(f"")
            elif size_match:
                data_dict[int(size_match.group(1))] = [temperature, winding_number]
                temperature = []
                winding_number = []

    return data_dict

if __name__ == '__main__':
    data_dict = parse_data("./datafrommats/test.out", r"\s*(\d*\.?\d*)\s* (\d*\.?\d*)E?([-|+]?0?\d*)", r"L=(\d+):")

    with open("cleaned_mats_data.txt", 'w') as f:
        for size in data_dict:
            f.write(f"L={size}:\n")
            for temperature, winding_number in zip(data_dict[size][0], data_dict[size][1]):
                f.write(f"{winding_number * temperature} {temperature}\n")
