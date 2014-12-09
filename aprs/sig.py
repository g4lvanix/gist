#!/usr/bin/env python3

import math

samp_rate = int(input("Enter sample rate: "))
freq = int(input("Enter frequency to generate: "))
steps = 2**int(input("Enter resolution in bits: "))-1
fname = input("Enter output file name: ")

# number of samples per period of the output frequency 
# at given sample rate
nSamplesPeriod = int(samp_rate / freq)

# difference in phase angle between samples
deltaPhi = 360/nSamplesPeriod

f = open(fname, "w")

f.write("int16_t cosLUT_" + str(freq) + "HZ[" + str(nSamplesPeriod) + "] = {\n\t")

for i in range(nSamplesPeriod):
	
	if i > 0 and (i % 10 == 0):
		f.write("\n\t")
	
	sine = (steps/2)*math.cos(math.radians( (math.pi/2) + (i*deltaPhi)))
	
	f.write(str(int(sine)) + ", ")

f.write("\n};")

f.close()
