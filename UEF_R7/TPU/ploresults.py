import matplotlib.pyplot as plt
import numpy as np

# Data
devices = ["CPU", "GPU", "TPU"]
training_time = [804.77, 74.047, 17.82]  # in seconds
testing_time = [5.41, 2.853, 1.002]  # in seconds
final_loss = [0.0043, 0.0044, 0.0016]
accuracy = [0.9987, 0.9947, 0.9937]


def add_value_labels(ax, spacing=5):
    """
    Add labels to the end of each bar in a bar chart.

    Arguments:
    ax -- matplotlib axes object
    spacing -- distance between the label and the bar
    """
    for rect in ax.patches:
        y_value = rect.get_height()
        x_value = rect.get_x() + rect.get_width() / 2

        # Number of points between bar and label
        space = spacing
        va = 'bottom'

        # If value of bar is negative: place label below bar
        if y_value < 0:
            space *= -1
            va = 'top'

        # Use y value as label and format number with one decimal place
        label = "{:.3f}".format(y_value)

        # Create annotation
        ax.annotate(
            label,                      # Use `label` as label
            (x_value, y_value),         # Place label at end of the bar
            xytext=(0, space),          # Vertically shift label by `space`
            textcoords="offset points",  # Interpret `xytext` as offset in points
            ha='center',                # Horizontally center label
            va=va)                      # Vertically align label differently for
        # positive and negative values.


# Creating subplots with value labels
fig, axs = plt.subplots(2, 2, figsize=(12, 8))

# Bar plots with value labels
axs[0, 0].bar(devices, training_time, color='skyblue')
axs[0, 0].set_title('Training Time (s)')
axs[0, 0].set_ylabel('Seconds')
add_value_labels(axs[0, 0])

axs[0, 1].bar(devices, testing_time, color='lightgreen')
axs[0, 1].set_title('Testing Time (s)')
axs[0, 1].set_ylabel('Seconds')
add_value_labels(axs[0, 1])

axs[1, 0].bar(devices, final_loss, color='salmon')
axs[1, 0].set_title('Final Loss')
axs[1, 0].set_ylabel('Loss Value')
add_value_labels(axs[1, 0])

axs[1, 1].bar(devices, accuracy, color='lightcoral')
axs[1, 1].set_title('Accuracy')
axs[1, 1].set_ylabel('Accuracy Value')
axs[1, 1].set_ylim([0.99, 1])
add_value_labels(axs[1, 1])

# Adjust layout
plt.tight_layout()

# Show plot
plt.show()
