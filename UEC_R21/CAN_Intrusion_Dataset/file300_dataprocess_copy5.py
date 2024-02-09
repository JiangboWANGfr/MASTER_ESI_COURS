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


def payloadLengthCount(input_file_list, ax):
    total_id_counts = []
    payload_length_counts = []
    file_label = []
    for input_file in input_file_list:
        with open(input_file, 'r') as file:
            lines = file.readlines()
            total_id_count = len(lines)
            request_id_count = 0
            payload_length_count = 0
            # for line in lines:
            # for line_index in range(len(lines)):
            index = -1
            for line in lines:
                index += 1
                line_parts = line.split()
                find_response = False
                if "100" in line_parts[4]:
                    request_id_count += 1
                    parts_current_line = lines[index].split()
                    current_line_id = parts_current_line[3]
                    for id_i in range(1, 7):
                        if index+id_i >= len(lines):
                            break
                        if current_line_id in lines[index+id_i].split()[3]:
                            request_id_count += 1
                            break
            total_id_count -= request_id_count
            # total_id_counts.append(total_id_count)

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
        total_id_counts.append(total_id_count)
        if "Attack_free" in input_file:
            file_label.append("attack free")
        elif "DoS_attack" in input_file:
            file_label.append("dos attack")
        elif "Fuzzy_attack" in input_file:
            file_label.append("fuzzy attack")
        elif "Impersonation_attack" in input_file:
            file_label.append("impersonation attack")

        payload_length_counts.append(payload_length_count /
                                     total_id_count)
    # plt.figure(figsize=(10, 6))

    bar_width = 0.35
    index = range(len(total_id_counts))
    print(index)
    print(total_id_counts)
    print(payload_length_counts)
    # total_id_counts = [685084, 640379, 530595, 616917]
    # total_id_counts = sorted(total_id_counts, reverse=False)

    ax.barh(index, total_id_counts, bar_width,
            label='Total ID Count', color='b', alpha=0.7)
    # ax.barh([i + bar_width for i in index], payload_length_counts,
    #         bar_width, label='Payload Length Count', color='g', alpha=0.7)

    for i in index:
        ax.text(total_id_counts[i] + 10000, i,
                str(total_id_counts[i]), color='black')
        # ax.text(payload_length_counts[i] + 10000, i + bar_width -
        #         0.1, str(payload_length_counts[i]), color='black')

    # file_label = ["impersonation attack",
    #               "fuzzy attack", "dos attack", "attack free"]
    ax.set_xlabel('Count')
    ax.set_title('Total ID Count')
    ax.set_xlim(0, max(total_id_counts) * 1.1)
    ax.set_yticks([i + bar_width / 2 for i in index], file_label)
    ax.legend()
    # output_file_directoy2 = output_file_directoy.replace("/txt", "/png")
    # ax.savefig(output_file_directoy2+"/payloadLengthCount.png")
    # ax.show()


debug = True
debug = False
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
        if debug:
            if id != "0153":
                continue
        index = -1
        time_spend_total = 0
        request_count = 0
        find_response = True
        for line in lines:
            index += 1
            line_parts = line.split()
            find_response = False
            if id in line_parts[3] and "100" in line_parts[4]:
                parts_current_line = lines[index].split()
                ID_response_count = 0
                for id_i in range(1, 8):
                    if index+id_i >= len(lines):
                        break
                    if "100" in lines[index+id_i].split()[4]:
                        ID_response_count = id_i
                        break
                    else:
                        continue
                for i in range(1, ID_response_count):
                    if id in lines[index+i].split()[3]:
                        find_response = True
                        parts_next_line = lines[index+i].split()
                        request_count += 1
                        break
                    if i == ID_response_count-1:
                        # find_response = False
                        find_response = False
                        # request_count += 1
                        # parts_next_line = lines[index+i].split()

                if not find_response:
                    continue
                # print("index ", index, end=" ")
                time_spend = float(
                    parts_next_line[1])-float(parts_current_line[1])
                time_spend_total += time_spend
                if debug:
                    # time.sleep(1)
                    print("line id: ", str(index).ljust(5), "|timestamp: ", parts_current_line[1],
                          "|next line id: ", str(index+i).ljust(5),    "|timestamp: ", parts_next_line[1], "|time_spend: ", str(time_spend).ljust(10), "|total_time_spend: ", time_spend_total, "|request_count: ", request_count)
        if time_spend_total != 0:
            with open(output_file, 'a') as file:
                file.write(
                    f"{id.ljust(5)}: {time_spend_total/request_count}\n")
    print(f"File {input_file} finished!")


def plotIDtimeSpendCompare(ax):
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

    # plt.figure(figsize=(10, 6))
    bar_width = 0.2
    index = np.arange(len(item_ids))

    # create the comparison chart
    file_labels = ["attack free", "dos attack",
                   "fuzzy attack", "impersonation attack"]
    for i, file_path in enumerate(input_filelist):
        file_label = file_labels[i]
        values = [file_data[file_path].get(item_id) for item_id in item_ids]
        values = [0 if x is None else x for x in values]
        print(values)
        ax.bar(index + i * bar_width, values, bar_width, label=file_label)

    ax.set_xlabel('Item ID')
    ax.set_ylabel('time/second')
    ax.set_title('Average Time Spend of Each ID')
    ax.set_xticks(index + bar_width * ((4 - 1) / 2),
                  item_ids, rotation=45)
    ax.legend()
    # output_file_directoy2 = output_file_directoy.replace("/txt", "/png")
    # ax.savefig(output_file_directoy2+"/IDtimeSpendCompare.png")
    # plt.show()


def dataProcess(ax1):
    # Define the input and output file paths
    input_files_path = getFilesFromDirectory(directory_path)
    # Change to your desired output file name
    for input_file in input_files_path:
        # get the output file path
        output_file_path = getOutputFilePath(
            input_file, output_file_directoy, file_extension)
        # save the result to output file
        if debug:
            # if input_file != "./raw_dataset300\DoS_attack_dataset.txt300.txt":
            if input_file != "./raw_dataset300\Fuzzy_attack_dataset.txt300.txt":
                continue
        # IDtimeSpendCount(input_file, output_file_path)

    payloadLengthCount(input_files_path, ax1)


if __name__ == "__main__":
    # fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(24, 10))  # 创建一个包含两个子图的图表
    fig, (ax1) = plt.subplots(1, 1, figsize=(12, 5))
    dataProcess(ax1)
    # plotIDtimeSpendCompare(ax2)
    output_file_directory2 = output_file_directoy.replace("/txt", "/png")
    if not os.path.exists(output_file_directoy):
        os.makedirs(output_file_directoy)
    if not debug:
        plt.savefig(output_file_directory2 + "/CombinedChart.png")
    plt.show()
    print("Data processing finished!")
