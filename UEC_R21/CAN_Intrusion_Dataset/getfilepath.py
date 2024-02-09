import os
import re

def getFilesFromDirectory(directory_path):
    # get the list of files in the directory
    filesPath = []
    if os.path.exists(directory_path) and os.path.isdir(directory_path):
        files = os.listdir(directory_path)
        for file in files:
            file_path = os.path.join(directory_path, file)
            if os.path.isfile(file_path):
                filesPath.append(file_path)
        print("Found files path:", filesPath)
        return filesPath
    else:
        print(
            f"Directory '{directory_path}' does not exist or is not a directory.")


def getOutputFilePath(input_file_path,output_file_directoy,file_extension):
    # Input file path: ./RawData\\bt.W.x_out.1
    # Output file path: ./Dataprocessed\\bt.W.x_out.1.mop.s.thread.txt
    file_name = os.path.basename(input_file_path)
    # create the folder
    if not os.path.exists(output_file_directoy):
        os.makedirs(output_file_directoy)
    output_file_path = os.path.join(
        output_file_directoy, file_name + file_extension+".txt")
    

    return output_file_path

