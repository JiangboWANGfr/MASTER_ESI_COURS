from collections import defaultdict
import re
import os
import matplotlib.pyplot as plt
from getIdCounts import getIdCounts
from getfilepath import getFilesFromDirectory, getOutputFilePath
# the raw data directory path
directory_path = "./raw_dataset"
output_file_directoy = "./Dataprocessed/getIDreceiveInterval/png"
file_extension = "_IDreceiveInterval"


# Function to parse a line and extract timestamp and ID

def parse_line(line):
    # Split the line into parts
    parts = line.split()
    # Extract timestamp and ID
    timestamp = float(parts[1])
    id_hex = parts[3]
    return timestamp, id_hex


def getIDtimeinterval(input_file, output_file):

    # Read the file and parse all lines
    timestamps_by_id = defaultdict(list)
    find_line_request = False
    with open(input_file, 'r') as file:
        lines = file.readlines()
        for line in lines[0:50000]:
            parts = line.split()
            if "100" in parts[4]:
                find_line_request = True
                request_id = parts[3]
                response_line_index = 0
                continue
            # 在request line 下面的6行中找到相同ID的response line
            # if find_line_request:
            #     if response_line_index <6:
            #         if request_id in line:
            #             find_line_request = False
            #             continue
            #         else:
            #             response_line_index += 1
            #     else:
            #         find_line_request = False

            timestamp, id_hex = parse_line(line)
            timestamps_by_id[id_hex].append(timestamp)

    # Calculate time intervals for each ID
    time_intervals_by_id = {id_hex: [t - s for s, t in zip(ts[:-1], ts[1:])]
                            for id_hex, ts in timestamps_by_id.items() if len(ts) > 1}
    return time_intervals_by_id


def plotIDreceiveInterval1(time_intervals_by_id, output_file):
    # Extract the time intervals for ID '0153'
    ID = "0153"
    time_intervals_0153 = time_intervals_by_id[ID]

    # Plot the time intervals for ID '0153' as a scatter plot
    plt.figure(figsize=(12, 6))
    plt.scatter(range(len(time_intervals_0153)),
                time_intervals_0153, marker='o')
    # plt.plot(time_intervals_0153, marker='o')
    plt.title('Time Intervals for ID ' + ID)
    plt.xlabel('Occurrence Index')
    plt.ylabel('Time Interval (seconds)')
    plt.ylim(0, 0.08)
    plt.grid(True)
    output_file = output_file + ID + ".png"
    plt.savefig(output_file)


def plotIDreceiveInterval(time_intervals_by_id, ID, output_file):
    # Extract the time intervals for ID '0153'
    base_name = os.path.basename(output_file)
    if "Attack_free" in output_file:
        file_label = "attack free"
    elif "DoS_attack" in output_file:
        file_label = "dos attack"
    elif "Fuzzy_attack" in output_file:
        file_label = "fuzzy attack"
    elif "Impersonation_attack" in output_file:
        file_label = "impersonation attack"
    base_name_without_extention = output_file.replace(base_name, "")
    output_file_dir = base_name_without_extention + "/" + ID + "/"
    if not os.path.exists(output_file_dir):
        os.makedirs(output_file_dir)
    time_intervals_0153 = time_intervals_by_id[ID]

    # Plot the time intervals for ID '0153' as a scatter plot
    plt.figure(figsize=(12, 6))
    plt.scatter(range(len(time_intervals_0153)),
                time_intervals_0153, marker='o')
    # plt.plot(time_intervals_0153, marker='o')
    plt.title('Time Intervals for ID ' + ID + " in " + file_label)
    plt.xlabel('Occurrence Index')
    plt.ylabel('Time Interval (seconds)')
    plt.ylim(0, 0.08)
    plt.grid(True)
    output_file_name = output_file_dir + base_name + ID + ".png"
    print(output_file_name)
    plt.savefig(output_file_name)


def dataProcess():
    # Define the input and output file paths
    input_files_path = getFilesFromDirectory(directory_path)
    # Change to your desired output file name
    # 164
    # 1F1
    # 220
    # 2C0
    # 4B0
    # 4B1
    # 5A0
    ids = ["0153", "0164", "01f1", "0220", "02c0", "04b0", "04b1", "05a0"]
    for ID in ids:
        for input_file in input_files_path:
            # get the output file path
            output_file_path = getOutputFilePath(
                input_file, output_file_directoy, file_extension)
            # save the result to output file
            IDtimeinterval = getIDtimeinterval(input_file, output_file_path)
            # plot the result
            plotIDreceiveInterval(IDtimeinterval, ID, output_file_path)


if __name__ == "__main__":
    dataProcess()
    print("Data processing finished!")
