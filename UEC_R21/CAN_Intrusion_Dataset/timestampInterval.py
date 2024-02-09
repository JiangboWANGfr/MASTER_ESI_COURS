import matplotlib.pyplot as plt
import os

# input file and output file
input_file = "./raw_dataset/Attack_free_dataset.txt"
output_file = "./dataprocessed/Attack_free_dataset_timestamp_interval.txt"

# 打开文件并读取内容
with open(input_file, 'r') as file:
    lines = file.readlines()

timestamps = []  # the list to store timestamps
intervals = []  # the list to store intervals
Ids = []  # the list to store IDs
ControlBits = []  # the list to store ControlBits

# get timestamps from lines
for line in lines:
    parts = line.split()
    if len(parts) >= 2:
        # save 06f as float
        timestamp = float(parts[1])
        timestamps.append(timestamp)
        Ids.append(parts[3])
        ControlBits.append(parts[4])

# plot the result
id_plot = []
for i in range(len(Ids)-1):
    if Ids[i] == '0153' and ControlBits[i] == '100':
        for j in range(1, 7):
            if Ids[i+j] == '0153' and ControlBits[i+j] == '000':
                time_interval = round(
                    float(timestamps[i+j])-float(timestamps[i]), 6)
                # print("timestamp2：{} - timestamp1：{} = {}".format(timestamps[i+j], timestamps[i], time_interval))
                # format interval to 6 decimal places
                # time_interval = '{:12f}'.format(time_interval)
                id_plot.append(time_interval)

# write the output file
# if output file exists, don't overwrite it,if not, create a new one
if os.path.exists(output_file):
    print("File exists!")
else:
    with open(output_file, 'w') as file:
        for i in range(len(intervals)):
            file.write(f'ID:{Ids[i+1]}  Interval: {intervals[i]}\n')


plt.figure(figsize=(13, 6))
plt.plot(id_plot)
plt.xlabel('Timestamp')
plt.ylabel('Interval')
plt.title('ID:0153 Interval')
plt.savefig('./dataprocessed/Attack_free_dataset_ID_0153_interval.png')
# plt.show()
