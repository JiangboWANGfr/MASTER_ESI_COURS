import os
import matplotlib.pyplot as plt
from getfilepath import getFilesFromDirectory, getOutputFilePath
# the raw data directory path
directory_path = "./raw_dataset300"
output_file_directoy = "./Dataprocessed/numberID300/txt"
file_extension = "_number_id"

request_id = []


def getIdCounts(lines):
    # create a dictionary to store ID and its count
    id_count = {}
    global request_id
    request_id = []
    # iterate through each line
    for line in lines:
        # split line into parts and get ID
        parts = line.split()
        if len(parts) >= 4:
            # if len(parts) >= 4 and parts[4] == "100":
            if parts[4] == "100":
                request_id.append(parts[3])
            id = parts[3]
            # add ID to dictionary
            id_count[id] = id_count.get(id, 0) + 1

    return id_count


def saveIdCountsToFile(input_file, output_file):
    # read the file
    with open(input_file, 'r') as file:
        lines = file.readlines()
    # read the lines and get the ID and its count
    id_count = getIdCounts(lines)
    # sort the dictionary by value
    id_count = dict(
        sorted(id_count.items(), key=lambda item: item[1], reverse=True))

    # save the result to output file
    with open(output_file, 'w') as file:
        for id, count in id_count.items():
            file.write(f'ID: {id}   Count: {count}\n')
        id_total = sum(id_count.values())
        file.write(f'TotalID: {id_total}\n')
    return id_count


def PlotIdCounts(id_count, output_file):
    # List of specific IDs to color in red
    red_ids = request_id

    # plot the result\
    file_name = os.path.basename(output_file)
    file_name_without_extention = file_name.replace(".txt", "")
    plt.figure(figsize=(13, 6))
    # if "0000" in id_count.keys():
    #     id_count.pop("0000")
    bars = plt.bar(id_count.keys(), id_count.values())
    # Color specific IDs in red on the x-axis
    plt.xticks(rotation=45, color='black')  # Default color for all x-ticks
    for i, tick_label in enumerate(plt.gca().get_xticklabels()):
        if tick_label.get_text() in red_ids:
            tick_label.set_color('red')
    plt.xlabel('ID')
    plt.ylabel('Count')
    plt.title(file_name_without_extention+'_Count')
    plt.xticks(rotation=45)
    # Adding numbers on top of each bar
    for bar in bars:
        yval = bar.get_height()
        # plt.text(bar.get_x() + bar.get_width()/2, yval, int(yval), ha='center', va='bottom')
        # 将显示的字体旋转90度并加粗
        plt.text(bar.get_x() + bar.get_width()/2, yval, int(yval),
                 ha='center', va='bottom', rotation=90)


    # save the plot picture
    output_file = output_file.replace("/txt", "/png")
    output_file = output_file.replace(".txt", ".png")
    if not os.path.exists(os.path.dirname(output_file)):
        os.makedirs(os.path.dirname(output_file))
    plt.savefig(output_file)
    print("Plot saved to:", output_file)
    # plt.show()


def getIDDifference():
    ID = []
    IDs = []
    input_files_path = getFilesFromDirectory(output_file_directoy)
    for input_file in input_files_path:
        with open(input_file, 'r') as file:
            lines = file.readlines()
            for line in lines:
                parts = line.split()
                ID.append(parts[1])
            IDs.append(ID)
            ID = []
    for i in range(len(IDs)):
        i = i+1
        if i < len(IDs):
            print("The difference between", i, "and", 0, "is:")
            print(set(IDs[i]) ^ set(IDs[0]))
            print("The difference between", i, "and", 0, "is:",
                  len(set(IDs[i]) ^ set(IDs[0])))


def dataProcess():
    # Define the input and output file paths
    input_files_path = getFilesFromDirectory(directory_path)
    # Change to your desired output file name
    for input_file in input_files_path:
        # get the output file path
        output_file_path = getOutputFilePath(
            input_file, output_file_directoy, file_extension)
        # save the result to output file
        id_count = saveIdCountsToFile(input_file, output_file_path)
        # plot the result
        PlotIdCounts(id_count, output_file_path)


if __name__ == "__main__":
    dataProcess()
    getIDDifference()
    print("Data processing finished!")
