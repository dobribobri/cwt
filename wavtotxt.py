import numpy as np
import scipy.io.wavfile

rate, data = scipy.io.wavfile.read("A4.wav");

f = open("A4.txt", "w");

for i in range(len(data)):
	f.write(str(data[i][0]) + "\n");

f.close();
