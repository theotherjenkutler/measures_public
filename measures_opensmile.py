#!/usr/bin/python3.8
import opensmile
import numpy as np
import matplotlib.pyplot as pl

#samplerate in hz in this case for the plot
Fs = 1.

smile = opensmile.Smile(
    feature_set=opensmile.FeatureSet.ComParE_2016,
    feature_level=opensmile.FeatureLevel.LowLevelDescriptors_Deltas,
)
y = smile.process_file('sleepnumbered.wav')
#for debugging
#x = y.to_markdown()
#print(x)
##############

tm = np.arange(1., len(y)+1.) / Fs
pl.plot(tm, y)
pl.show()
