#!/usr/bin/env python3

# This script calculates the binary tuning words for the AD9850 DDS clock generator
# It allows to generate tuning words for CW, FSK-CW and WSPR operation

refclk = 125*10**6

wsprOffset = 1400
wsprChannelWidth = 6
wsprToneSpacing = 1.4648
wsprDialFreqs = {"160m": 1836600, "80m": 3592600, "40m": 7038600,"30m": 10138700,
				"17m": 18104600, "15m": 21094600, "12m": 24924600, "10m": 28124600}

# Calculates the tuning word and converts it into a string containing
# the 40 bits (^= 5 bytes) needed, at the same time reverses the bit order
# to LSB first
def calcTuneWord(freq):
	delphi = int((2**32 * freq)/refclk)
	binary = bin(delphi)[2:].zfill(40)
	binary = binary[::-1]
	return binary

# generates a formatted output string that looks like a C array definition
# this allows the string to be directly copied into a C source file
def formatOutput(s):
	temp = "{"

	for i in range(0,len(s),8):
		temp += hex(int(s[i:i+8],2))+","

	temp = temp[:len(temp)-1] + "}"

	return temp

# calculate the two needed frequencies needed for FSK-CW transmission
# for standard (A1A) CW mode, only one frequency can be used
def getQRSSconf():
	freqlo = int(input("Enter base frequency (Hz): "))
	freqhi = freqlo + int(input("Enter frequency shift (Hz): "))

	tunelo = calcTuneWord(freqlo)
	tunehi = calcTuneWord(freqhi)

	print("\nconst uint8_t ad9850flo[5] = "+formatOutput(tunelo)+";")
	print("const uint8_t ad9850fhi[5] = "+formatOutput(tunehi)+";")

# calculates four tones needed for WSPR message transmission via 4-FSK
# tones are separated by wsprToneSpacing Hz
def calcWSPR(freq):
	freqWords = []

	for i in range(4):
		word = calcTuneWord((i*wsprToneSpacing)+freq)
		freqWords.append(word)

	return freqWords

# Requests more user input like band and channel information to generate the
# correct WSPR 4-FSK tones
def getWSPRconf():
	band = input("Enter WSPR band (e.g. 10m): ")
	dial = wsprDialFreqs[band]
	channel = int(input("Enter WSPR channel (0-32): "))

	while (channel < 0) or (channel > 32):
		print("Channel must be between 0 and 32!")
		channel = int(input("Enter WSPR channel (0-32): "))

	print("\n")

	choffs = wsprChannelWidth*channel
	baseFreq = dial+choffs+wsprOffset
	words = calcWSPR(baseFreq)

	print("// WSPR 4-FSK tones for",band,"channel " +str(channel)+" ("+str(baseFreq)+" Hz)")

	print("const uint8_t wsprTones[4][5] = {")

	for s in words:
		print("\t"+formatOutput(s)+",")

	print("};")


if __name__ == "__main__":
	print("g4lvanix MEPT configurator\n")

	choice = 0
	incorrect = 1
	while (incorrect):
		print("Options:")
		print("(1) Calculate QRSS tuning words")
		print("(2) Calculate WSPR tuning words")

		choice = int(input("Select option: "))

		print(" ")

		if (choice == 1) or (choice == 2):
			incorrect = 0

	if choice == 1:
		getQRSSconf()
	else:
		getWSPRconf()
