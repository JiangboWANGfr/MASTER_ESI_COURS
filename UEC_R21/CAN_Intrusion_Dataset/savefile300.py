import os
import time
import matplotlib.pyplot as plt
from getfilepath import getFilesFromDirectory, getOutputFilePath
# the raw data directory path
directory_path = "./raw_dataset"
output_file_directoy = "./raw_dataset300"
file_extension = "300"


def savetimestemps300tofile(input_file, output_file):
    lines_0_300 = []
    with open(input_file, 'r') as file:
        lines = file.readlines()
        for line in lines:
            parts = line.split()
            lines_0_300.append(line)
            if (float(parts[1])-300 < 0.001 and float(parts[1])-300 > -0.001):
                break
    if os.path.exists(output_file):
        os.remove(output_file)
    with open(output_file, 'w') as file:
        for line in lines_0_300:
            file.write(line)
    print("save file" + output_file + " finished!")


def saverequestansresponse6(input_file, output_file):
    lines_0_300 = []
    with open(input_file, 'r') as file:
        lines = file.readlines()
        line_index = 0
        for line in lines:
            parts = line.split()
            if "100" in parts[4]:
                lines_0_300.append(lines[line_index])
                for i in range(1, 7):
                    if "100" in lines[line_index+i].split()[4]:
                        break
                    # print("line ", lines[line_index+i], end="")
                    lines_0_300.append(lines[line_index+i])
            line_index += 1
            if (float(parts[1])-300 < 0.001 and float(parts[1])-300 > -0.001):
                break
    if os.path.exists(output_file):
        os.remove(output_file)
    with open(output_file, 'w') as file:
        for line in lines_0_300:
            file.write(line)
    print("save file" + output_file + " finished!")


debug = True
debug = False


def dataProcess():
    # Define the input and output file paths
    input_files_path = getFilesFromDirectory(directory_path)
    # Change to your desired output file name
    for input_file in input_files_path:
        # get the output file path
        output_file_path = getOutputFilePath(
            input_file, output_file_directoy, file_extension)
        # save the result to output file
        # savetimestemps300tofile(input_file, output_file_path)
        if debug:
            if input_file != "./raw_dataset\DoS_attack_dataset.txt":
                # if input_file != "./raw_dataset300\Fuzzy_attack_dataset.txt300.txt":
                continue
        savetimestemps300tofile(input_file, output_file_path)


if __name__ == "__main__":
    dataProcess()
    print("Data processing finished!")
