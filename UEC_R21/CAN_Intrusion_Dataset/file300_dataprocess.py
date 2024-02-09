# https://docs.google.com/spreadsheets/d/1YYlZ-IcTQlz-LzaYkHO-7a4SFM8QYs2BGNXiSU5_EwI/
# https://github.com/openvehicles/Open-Vehicle-Monitoring-System/blob/master/vehicle/OVMS.X/vehicle_kiasoul.c
import os
import numpy as np
import matplotlib.pyplot as plt
import time
from getfilepath import getFilesFromDirectory, getOutputFilePath
from getIdCounts import getIdCounts
# the raw data directory path
directory_path = "./raw_dataset300"
output_file_directoy = "./dataprocessed/timestemps300/txt"
file_extension = "_timeStemps"


def payloadLengthCount(input_file_list):
    total_id_counts = []
    payload_length_counts = []
    for input_file in input_file_list:
        with open(input_file, 'r') as file:
            lines = file.readlines()
            payload_length_count = 0
            for line in lines:
                parts = line.split()
                payload_length_count += int(parts[6])
        base_filename = os.path.basename(input_file)
        output_file_directoy2 = output_file_directoy + "/payloadLengthCount/"
        if not os.path.exists(output_file_directoy2):
            os.makedirs(output_file_directoy2)
        output_file = output_file_directoy2+"payloadLengthCount.txt"
        if os.path.exists(output_file):
            os.remove(output_file)
        max_time = lines[-1].split()[1]
        print(f"{base_filename.ljust(50)} in time:{max_time}    total ID count: {len(lines)}    payload length count: {payload_length_count}")
        with open(output_file, 'a') as file:
            file.write(
                f"{base_filename.ljust(50)} in time:{max_time}    total ID count: {len(lines)}    payload length count: {payload_length_count}\n")
        total_id_counts.append(len(lines))
        payload_length_counts.append(payload_length_count)
    plt.figure(figsize=(10, 6))

    bar_width = 0.35
    index = range(len(total_id_counts))
    print(index)
    print(total_id_counts)
    print(payload_length_counts)

    plt.barh(index, total_id_counts, bar_width,
             label='Total ID Count', color='b', alpha=0.7)
    plt.barh([i + bar_width for i in index], payload_length_counts,
             bar_width, label='Payload Length Count', color='g', alpha=0.7)

    for i in index:
        plt.text(total_id_counts[i] + 10000, i - 0.1,
                 str(total_id_counts[i]), color='black')
        plt.text(payload_length_counts[i] + 10000, i + bar_width -
                 0.1, str(payload_length_counts[i]), color='black')

    plt.xlabel('Count')
    plt.title('Total ID Count and Payload Length Count')
    file_label = ["attack free", "dos attack",
                  "fuzzy attack", "impersonation attack"]
    plt.yticks([i + bar_width / 2 for i in index], file_label)
    plt.legend()
    plt.tight_layout()
    output_file_directoy2 = output_file_directoy.replace("/txt", "/png")
    plt.savefig(output_file_directoy2+"/payloadLengthCount.png")
    plt.show()


# calculate the average time spend of each ID


def IDtimeSpendCount(input_file, output_file):

    if os.path.exists(output_file):
        os.remove(output_file)
    with open(input_file, 'r') as file:
        lines = file.readlines()
        id_count = getIdCounts(lines)
        id_count = dict(
            sorted(id_count.items(), key=lambda item: item[1], reverse=True))
    for id, count in id_count.items():
        index = -1
        time_spend_total = 0
        request_count = 0
        for line in lines:
            index += 1
            line_parts = line.split()
            if id in line_parts[3] and "100" in line_parts[4]:
                parts_current_line = lines[index].split()
                request_count += 1
                for i in range(1, 6):
                    if id in lines[index+i].split()[3]:
                        find_response = True
                        parts_next_line = lines[index+i].split()
                        break
                    else:
                        find_response = False

                if not find_response:
                    continue
                time_spend = round(float(
                    parts_next_line[1])-float(parts_current_line[1]), 6)
                time_spend_total += time_spend
                # print("line id: ", str(index).ljust(5), "|timestamp: ", parts_current_line[1],
                #       "|next line id: ", str(index+i).ljust(5),    "|timestamp: ", parts_next_line[1], "|time_spend: ", str(time_spend).ljust(10), "|total_time_spend: ", time_spend_total)
        if time_spend_total != 0:
            with open(output_file, 'a') as file:
                file.write(
                    f"{id.ljust(5)}: {time_spend_total/request_count}\n")
    print(f"File {input_file} finished!")


def plotIDtimeSpendCompare():
    file_data = {}
    input_filelist = getFilesFromDirectory(output_file_directoy)
    for input_file in input_filelist:
        with open(input_file, 'r') as file:
            lines = file.readlines()
            data = {}
            for line in lines:
                parts = line.split()
                item_id = parts[0]
                value = float(parts[2])
                data[item_id] = value

        file_data[input_file] = data
    # x axis is the item id
    item_ids = list(file_data[input_filelist[0]].keys())[0:40]
    print(item_ids)
    print("file data length: ", len(file_data))

    plt.figure(figsize=(10, 6))
    bar_width = 0.2
    index = np.arange(len(item_ids))

    # create the comparison chart
    for i, file_path in enumerate(input_filelist):
        file_name = os.path.basename(file_path)
        values = [file_data[file_path].get(item_id) for item_id in item_ids]
        values = [0 if x is None else x for x in values]
        print(values)
        plt.bar(index + i * bar_width, values, bar_width, label=file_name)

    plt.xlabel('Item ID')
    plt.ylabel('time/nanosecond')
    plt.title('Data Comparison')
    plt.legend()
    plt.xticks(index + bar_width * ((4 - 1) / 2),
               item_ids, rotation=45)
    plt.tight_layout()
    output_file_directoy2 = output_file_directoy.replace("/txt", "/png")
    plt.savefig(output_file_directoy2+"/IDtimeSpendCompare.png")
    plt.show()


def dataProcess():
    # Define the input and output file paths
    input_files_path = getFilesFromDirectory(directory_path)
    # Change to your desired output file name
    for input_file in input_files_path:
        # get the output file path
        output_file_path = getOutputFilePath(
            input_file, output_file_directoy, file_extension)
        # save the result to output file
        IDtimeSpendCount(input_file, output_file_path)
    # payloadLengthCount(input_files_path)


if __name__ == "__main__":
    dataProcess()
    plotIDtimeSpendCompare()
    print("Data processing finished!")
