#!/usr/bin/python3.9

import sys
import numpy as np
import neurokit2 as nk2
import librosa
import matplotlib.pyplot as plt
import pandas as pd
import math


#fname = sys.argv[0]
#just for testing-
fname = "/Users/user/Desktop/A8B72F0A8AB4.txt"
identifier = str.split(fname,'.')
print(identifier[0])

######Automatically determine EDA SAMPLE RATE
#divide total number of samples by length of corresponding audio file in seconds
#fileLength = librosa.get_duration(filename = identifier[0] + '.wav')
#just for testing
musicFile = "/Users/user/Desktop/1666468820_541241F6B870.mp3"
fileLength = librosa.get_duration(filename="/Users/user/Desktop/1666468820_541241F6B870.mp3") #THIS BREAKS IT
#print(fileLength)
#count lines in the text file for number of samples
with open(musicFile, 'r', errors='ignore') as fp:
    for count, line in enumerate(fp):
        pass
numOfSamples= count + 1
#print(numOfSamples)
Fs = numOfSamples/fileLength
print(Fs)

###########
#### Y IS A NUMPY VECTOR OF SINGLE CHANNEL EDA
dirtyArray = np.genfromtxt(fname, dtype=float, skip_header=1)
y = dirtyArray[~np.isnan(dirtyArray)]
print(y)
########

plt.rcParams['figure.figsize'] = [15, 5]  # Bigger images
#nk2.read_acqknowledge() need to figure out how to use this instead of np.getfromtxt
# Process the raw EDA signal
signals, info = nk2.eda_process(y, sampling_rate=Fs)
#print(signals)
# Extract clean EDA and SCR features
cleaned = signals["EDA_Clean"]
#####
#####
##### how do you remove NaNs from a list of numbers such as features?
#####
#####
features = [info["SCR_Onsets"], info["SCR_Peaks"], info["SCR_Recovery"]]
print(features)

# Visualize SCR features in cleaned EDA signal
plot = nk2.events_plot(features, cleaned, color=['red', 'blue', 'orange'])
#print(plot)
# Filter phasic and tonic components
data = nk2.eda_phasic(nk2.standardize(y), sampling_rate=Fs)
#print(data)
#data["EDA_Raw"] = y  # Add raw signal
data.plot()

np.savetxt('testSignals.txt', signals, fmt='%s')
np.savetxt('testFeatures.txt', features, fmt='%s')
print("text saved")

