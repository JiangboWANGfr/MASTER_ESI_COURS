# open file
input_file = "./raw_dataset/Impersonation_attack_dataset.txt"
output_file = "./raw_dataset/Impersonation_attack_dataset_timestamp.txt"

# read file and get lines
with open(input_file, 'r') as file:
    lines = file.readlines()

# split lines and get timestamp and data
timestamps = []
data = []

for line in lines:
    parts = line.strip().split()
    timestamp = float(parts[1])
    timestamps.append(timestamp)
    data_hex = parts[7:]
    data_bytes = bytes.fromhex(''.join(data_hex))
    data.append(data_bytes)

# calculate time delta
first_timestamp = timestamps[0]
time_deltas = [(timestamp - first_timestamp) for timestamp in timestamps]

# write the output file
with open(output_file, 'w') as output_file:
    for i in range(len(time_deltas)):
        timestamp_str = '{:12f}'.format(time_deltas[i])
        data_hex = ' '.join('{:02x}'.format(b) for b in data[i])
        output_line = f"{lines[i].split()[0]}      {timestamp_str}        {lines[i].split()[2]} {lines[i].split()[3]}    {lines[i].split()[4]}    {lines[i].split()[5]} {lines[i].split()[6]}    {data_hex}\n"
        output_file.write(output_line)

print("Impersonation attack dataset timestamped successfully!")